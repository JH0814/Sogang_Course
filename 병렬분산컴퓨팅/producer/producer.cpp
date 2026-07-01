#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include <librdkafka/rdkafkacpp.h>

namespace {

std::atomic_bool g_running{true};

const std::vector<std::string> NORMAL_LOGS = {
    "[INFO] 192.168.1.10 - GET /index.html 200",
    "[INFO] 10.0.0.5 - POST /login 302",
    "[DEBUG] Database connection established successfully.",
    "[INFO] 172.16.0.4 - GET /images/logo.png 200",
    "[INFO] api-gateway - request completed in 18 ms",
    "[INFO] auth-service - JWT refresh accepted",
};

const std::vector<std::string> MALICIOUS_LOGS = {
    "[ERROR] Buffer overflow attempt: \x90\x90\x90\x90\x90\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68",
    "[WARN] Invalid input detected: %x%x%x%x%n",
    "[ERROR] Command execution failed: ping -c 1 8.8.8.8; cat /etc/shadow",
    "[WARN] SQL injection payload: admin' OR '1'='1",
    "[WARN] Suspicious traversal payload: ../../../../etc/passwd",
    "[ERROR] XSS probe detected: <script>alert(1)</script>",
};

struct Options {
    std::string brokers = "localhost:9092";
    std::string topic = "security_logs";
    int messages = 100000;
    int rate_per_second = 1000;
    int malicious_percent = 5;
};

class DeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message &message) override {
        if (message.err()) {
            std::cerr << "delivery failed: " << message.errstr() << '\n';
        }
    }
};

void handle_signal(int) {
    g_running = false;
}

void print_usage(const char *program) {
    std::cout
        << "Usage: " << program << " [options]\n"
        << "  --brokers HOSTS          Kafka bootstrap servers (default: localhost:9092)\n"
        << "  --topic TOPIC            Kafka topic (default: security_logs)\n"
        << "  --messages N             Number of logs to produce, 0 means infinite (default: 100000)\n"
        << "  --rate N                 Target messages per second, 0 means unlimited (default: 1000)\n"
        << "  --malicious-percent N    Malicious payload ratio, 0-100 (default: 5)\n"
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
        } else if (arg == "--messages") {
            options.messages = parse_int(arg, require_value(arg), 0, 1000000000);
        } else if (arg == "--rate") {
            options.rate_per_second = parse_int(arg, require_value(arg), 0, 10000000);
        } else if (arg == "--malicious-percent") {
            options.malicious_percent = parse_int(arg, require_value(arg), 0, 100);
        } else if (arg == "--help") {
            print_usage(argv[0]);
            std::exit(0);
        } else {
            throw std::runtime_error("unknown option: " + arg);
        }
    }
    return options;
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

    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    if (conf->set("bootstrap.servers", options.brokers, errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << "failed to set bootstrap.servers: " << errstr << '\n';
        delete conf;
        return 1;
    }

    DeliveryReportCb delivery_cb;
    conf->set("dr_cb", &delivery_cb, errstr);
    conf->set("queue.buffering.max.messages", "1000000", errstr);
    conf->set("batch.num.messages", "10000", errstr);

    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    delete conf;
    if (!producer) {
        std::cerr << "failed to create producer: " << errstr << '\n';
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> percent_dist(1, 100);
    std::uniform_int_distribution<int> normal_dist(0, static_cast<int>(NORMAL_LOGS.size()) - 1);
    std::uniform_int_distribution<int> malicious_dist(0, static_cast<int>(MALICIOUS_LOGS.size()) - 1);

    const auto start = std::chrono::steady_clock::now();
    const auto delay = options.rate_per_second > 0
                           ? std::chrono::microseconds(1000000 / options.rate_per_second)
                           : std::chrono::microseconds(0);

    int sent = 0;
    int malicious_sent = 0;
    std::cout << "producer started: brokers=" << options.brokers
              << ", topic=" << options.topic
              << ", messages=" << (options.messages == 0 ? std::string("infinite") : std::to_string(options.messages))
              << ", rate=" << (options.rate_per_second == 0 ? std::string("unlimited") : std::to_string(options.rate_per_second))
              << "/sec, malicious=" << options.malicious_percent << "%\n";

    while (g_running && (options.messages == 0 || sent < options.messages)) {
        const bool malicious = percent_dist(gen) <= options.malicious_percent;
        const std::string &payload = malicious ? MALICIOUS_LOGS[malicious_dist(gen)] : NORMAL_LOGS[normal_dist(gen)];

        while (g_running) {
            RdKafka::ErrorCode err = producer->produce(
                options.topic,
                RdKafka::Topic::PARTITION_UA,
                RdKafka::Producer::RK_MSG_COPY,
                const_cast<char *>(payload.data()),
                payload.size(),
                nullptr,
                0,
                0,
                nullptr,
                nullptr);

            if (err == RdKafka::ERR_NO_ERROR) {
                break;
            }
            if (err != RdKafka::ERR__QUEUE_FULL) {
                std::cerr << "produce failed: " << RdKafka::err2str(err) << '\n';
                break;
            }
            producer->poll(100);
        }

        producer->poll(0);
        ++sent;
        malicious_sent += malicious ? 1 : 0;

        if (sent % 10000 == 0) {
            std::cout << "sent=" << sent << ", malicious=" << malicious_sent << '\n';
        }
        if (delay.count() > 0) {
            std::this_thread::sleep_for(delay);
        }
    }

    producer->flush(10000);
    const auto end = std::chrono::steady_clock::now();
    const double seconds = std::chrono::duration<double>(end - start).count();
    std::cout << "producer finished: sent=" << sent
              << ", malicious=" << malicious_sent
              << ", elapsed_sec=" << seconds
              << ", throughput_logs_sec=" << (seconds > 0.0 ? sent / seconds : 0.0)
              << '\n';

    delete producer;
    return 0;
}
