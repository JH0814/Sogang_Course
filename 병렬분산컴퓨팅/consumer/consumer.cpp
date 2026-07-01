#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <librdkafka/rdkafkacpp.h>
#include <omp.h>

namespace {

std::atomic_bool g_running{true};

const std::vector<std::string> THREAT_SIGNATURES = {
    "\x90\x90\x90\x90\x90",
    "%x%x%x%x%n",
    "/etc/shadow",
    "OR '1'='1",
    "../",
    "<script>",
};

struct Options {
    std::string brokers = "localhost:9092";
    std::string topic = "security_logs";
    std::string group_id = "threat_hunter_group";
    int batch_size = 50000;
    int max_batches = 5;
    int poll_timeout_ms = 100;
    int threads = 0;
    int work_factor = 1;
    bool compare_sequential = false;
    std::string csv_path;
};

struct ScanResult {
    int threats = 0;
    double elapsed_ms = 0.0;
};

volatile long long g_scan_sink = 0;

void handle_signal(int) {
    g_running = false;
}

void print_usage(const char *program) {
    std::cout
        << "Usage: " << program << " [options]\n"
        << "  --brokers HOSTS          Kafka bootstrap servers (default: localhost:9092)\n"
        << "  --topic TOPIC            Kafka topic (default: security_logs)\n"
        << "  --group-id ID            Kafka consumer group id (default: threat_hunter_group)\n"
        << "  --batch-size N           Logs per analysis batch (default: 50000)\n"
        << "  --max-batches N          Number of batches to process, 0 means infinite (default: 5)\n"
        << "  --poll-timeout-ms N      Kafka poll timeout (default: 100)\n"
        << "  --threads N              OpenMP threads, 0 uses runtime default (default: 0)\n"
        << "  --work-factor N          Repeat signature scan per log for stable experiments (default: 1)\n"
        << "  --compare-sequential     Measure sequential scan and report speedup\n"
        << "  --csv PATH               Append batch metrics as CSV\n"
        << "  --help                   Show this help\n";
}

int parse_int(const std::string &name, const std::string &value, int min_value, int max_value) {
    char *end = nullptr;
    long parsed = std::strtol(value.c_str(), &end, 10);
    if (*end != '\0' || parsed < min_value || parsed > max_value) {
        throw std::runtime_error("invalid value for " + name + ": " + value);
    }
    return static_cast<int>(parsed);
}

Options parse_args(int argc, char **argv) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto require_value = [&](const std::string &name) -> std::string {
            if (i + 1 >= argc) {
                throw std::runtime_error("missing value for " + name);
            }
            return argv[++i];
        };

        if (arg == "--brokers") {
            options.brokers = require_value(arg);
        } else if (arg == "--topic") {
            options.topic = require_value(arg);
        } else if (arg == "--group-id") {
            options.group_id = require_value(arg);
        } else if (arg == "--batch-size") {
            options.batch_size = parse_int(arg, require_value(arg), 1, 100000000);
        } else if (arg == "--max-batches") {
            options.max_batches = parse_int(arg, require_value(arg), 0, 1000000);
        } else if (arg == "--poll-timeout-ms") {
            options.poll_timeout_ms = parse_int(arg, require_value(arg), 1, 60000);
        } else if (arg == "--threads") {
            options.threads = parse_int(arg, require_value(arg), 0, 1024);
        } else if (arg == "--work-factor") {
            options.work_factor = parse_int(arg, require_value(arg), 1, 100000);
        } else if (arg == "--compare-sequential") {
            options.compare_sequential = true;
        } else if (arg == "--csv") {
            options.csv_path = require_value(arg);
        } else if (arg == "--help") {
            print_usage(argv[0]);
            std::exit(0);
        } else {
            throw std::runtime_error("unknown option: " + arg);
        }
    }
    return options;
}

bool scan_payload(const std::string &payload, int work_factor, long long &score) {
    bool found = false;
    long long local_score = 0;

    for (int repeat = 0; repeat < work_factor; ++repeat) {
        for (const auto &sig : THREAT_SIGNATURES) {
            const std::size_t pos = payload.find(sig);
            if (pos != std::string::npos) {
                found = true;
                local_score += static_cast<long long>(pos + 1) * (repeat + 1);
            } else {
                local_score += static_cast<long long>(sig.size() + repeat);
            }
        }
    }

    score += local_score;
    return found;
}

ScanResult scan_parallel(const std::vector<std::string> &logs, int work_factor) {
    int total_threats = 0;
    long long total_score = 0;
    const double start = omp_get_wtime();

#pragma omp parallel for reduction(+ : total_threats, total_score) schedule(static)
    for (int i = 0; i < static_cast<int>(logs.size()); ++i) {
        long long score = 0;
        if (scan_payload(logs[i], work_factor, score)) {
            ++total_threats;
        }
        total_score += score;
    }

    const double end = omp_get_wtime();
    g_scan_sink = total_score;
    return {total_threats, (end - start) * 1000.0};
}

ScanResult scan_sequential(const std::vector<std::string> &logs, int work_factor) {
    int total_threats = 0;
    long long total_score = 0;
    const auto start = std::chrono::steady_clock::now();
    for (const auto &log : logs) {
        long long score = 0;
        if (scan_payload(log, work_factor, score)) {
            ++total_threats;
        }
        total_score += score;
    }
    const auto end = std::chrono::steady_clock::now();
    g_scan_sink = total_score;
    return {total_threats, std::chrono::duration<double, std::milli>(end - start).count()};
}

void write_csv_header_if_needed(const std::string &path) {
    std::ifstream in(path);
    if (in.good() && in.peek() != std::ifstream::traits_type::eof()) {
        return;
    }
    std::ofstream out(path, std::ios::app);
    out << "batch,threads,batch_size,threats,parallel_ms,parallel_logs_per_sec,sequential_ms,speedup\n";
}

void append_csv(const std::string &path, int batch_no, int threads, int batch_size, const ScanResult &parallel,
                const ScanResult *sequential) {
    std::ofstream out(path, std::ios::app);
    const double throughput = parallel.elapsed_ms > 0.0 ? batch_size / (parallel.elapsed_ms / 1000.0) : 0.0;
    const double sequential_ms = sequential ? sequential->elapsed_ms : 0.0;
    const double speedup = sequential && parallel.elapsed_ms > 0.0 ? sequential->elapsed_ms / parallel.elapsed_ms : 0.0;
    out << batch_no << ',' << threads << ',' << batch_size << ',' << parallel.threats << ','
        << parallel.elapsed_ms << ',' << throughput << ',' << sequential_ms << ',' << speedup << '\n';
}

} // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    Options options;
    try {
        options = parse_args(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n\n";
        print_usage(argv[0]);
        return 2;
    }

    if (options.threads > 0) {
        omp_set_num_threads(options.threads);
    }
    const int active_threads = options.threads > 0 ? options.threads : omp_get_max_threads();

    if (!options.csv_path.empty()) {
        write_csv_header_if_needed(options.csv_path);
    }

    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", options.brokers, errstr);
    conf->set("group.id", options.group_id, errstr);
    conf->set("auto.offset.reset", "earliest", errstr);
    conf->set("enable.auto.commit", "true", errstr);

    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    delete conf;
    if (!consumer) {
        std::cerr << "failed to create consumer: " << errstr << '\n';
        return 1;
    }

    RdKafka::ErrorCode err = consumer->subscribe({options.topic});
    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "failed to subscribe to " << options.topic << ": " << RdKafka::err2str(err) << '\n';
        delete consumer;
        return 1;
    }

    std::vector<std::string> batch;
    batch.reserve(options.batch_size);
    int processed_batches = 0;

    std::cout << "consumer started: brokers=" << options.brokers
              << ", topic=" << options.topic
              << ", group=" << options.group_id
              << ", batch_size=" << options.batch_size
              << ", max_batches=" << (options.max_batches == 0 ? std::string("infinite") : std::to_string(options.max_batches))
              << ", openmp_threads=" << active_threads
              << ", work_factor=" << options.work_factor << '\n';

    while (g_running && (options.max_batches == 0 || processed_batches < options.max_batches)) {
        RdKafka::Message *msg = consumer->consume(options.poll_timeout_ms);
        if (msg->err() == RdKafka::ERR_NO_ERROR) {
            batch.emplace_back(static_cast<const char *>(msg->payload()), msg->len());
        } else if (msg->err() != RdKafka::ERR__TIMED_OUT && msg->err() != RdKafka::ERR__PARTITION_EOF) {
            std::cerr << "consume warning: " << msg->errstr() << '\n';
        }
        delete msg;

        if (static_cast<int>(batch.size()) < options.batch_size) {
            continue;
        }

        ++processed_batches;
        ScanResult sequential_result{};
        ScanResult *sequential_ptr = nullptr;
        if (options.compare_sequential) {
            sequential_result = scan_sequential(batch, options.work_factor);
            sequential_ptr = &sequential_result;
        }

        ScanResult parallel_result = scan_parallel(batch, options.work_factor);
        const double throughput = parallel_result.elapsed_ms > 0.0
                                      ? options.batch_size / (parallel_result.elapsed_ms / 1000.0)
                                      : 0.0;
        const double speedup = sequential_ptr && parallel_result.elapsed_ms > 0.0
                                   ? sequential_ptr->elapsed_ms / parallel_result.elapsed_ms
                                   : 0.0;

        std::cout << "batch=" << processed_batches
                  << ", threats=" << parallel_result.threats
                  << ", parallel_ms=" << parallel_result.elapsed_ms
                  << ", throughput_logs_sec=" << throughput;
        if (sequential_ptr) {
            std::cout << ", sequential_ms=" << sequential_result.elapsed_ms
                      << ", speedup=" << speedup;
        }
        std::cout << '\n';

        if (!options.csv_path.empty()) {
            append_csv(options.csv_path, processed_batches, active_threads, options.batch_size, parallel_result, sequential_ptr);
        }
        batch.clear();
    }

    consumer->close();
    delete consumer;
    std::cout << "consumer finished: batches=" << processed_batches << '\n';
    return 0;
}
