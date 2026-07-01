# High-Throughput Log Analytics Engine

Kafka + OpenMP 기반의 보안 로그 분석 파이프라인입니다. Kafka producer가 정상/악성 로그를 스트리밍하고, consumer가 배치 단위로 로그를 받아 OpenMP 병렬 시그니처 매칭을 수행합니다.

## 프로젝트 구성

- Distributed middleware: Apache Kafka, Docker Compose localhost 배포
- Parallel acceleration: OpenMP `parallel for reduction`
- Application scenario: High-Throughput Log Analytics Engine
- Structural decoupling: Producer, Kafka broker, Consumer/analytics 단계 분리
- Performance evaluation: batch throughput, sequential baseline, OpenMP speedup, CSV 저장 지원
- Reproducibility: `Makefile`, `docker-compose.yml`, 실험 스크립트 제공

제출물 구성:

```text
report_20211522.pdf
README.md
Makefile
docker-compose.yml
producer/producer.cpp
consumer/consumer.cpp
scripts/thread_scaling.sh
results/thread_scaling.csv
```

## 환경 준비

Ubuntu/WSL 기준으로 다음 패키지가 필요합니다. OpenMP는 별도 애플리케이션을 설치하는 방식이 아니라 GCC의 `-fopenmp` 옵션으로 활성화하며, 런타임은 `libgomp1`을 사용합니다.

```bash
sudo apt update
sudo apt install -y build-essential make g++ libgomp1 librdkafka-dev
```

Docker Compose로 Kafka와 Zookeeper를 실행하므로 Docker도 필요합니다. Docker가 설치되어 있지 않다면 Docker Engine 또는 Docker Desktop을 먼저 설치합니다. Ubuntu 패키지로 설치하는 경우 다음 명령을 사용할 수 있습니다.

```bash
sudo apt install -y docker.io docker-compose-v2
```

WSL에서는 Docker Desktop 연동을 사용하는 방식도 가능합니다. 설치 후 다음 명령으로 Docker Compose 사용 가능 여부를 확인합니다.

```bash
docker compose version
```

Docker 권한이 제한된 일부 Linux/WSL 환경에서는 `docker` 명령 앞에 `sudo`가 필요할 수 있습니다. Kafka broker는 기본적으로 `localhost:9092`를 사용하므로, 실행 전에 해당 포트가 다른 프로세스에서 사용 중이지 않아야 합니다.

설치 확인:

```bash
g++ --version
make --version
ldconfig -p | grep rdkafka
docker compose version
```

## 빌드

Producer와 Consumer를 빌드합니다.

```bash
make all
```

## Kafka 실행

Kafka와 Zookeeper를 Docker Compose로 실행합니다.

```bash
docker compose up -d
```

Compose는 `security_logs` 토픽을 4개 파티션으로 자동 생성합니다.

상태 확인:

```bash
docker compose ps
```

Kafka 로그 확인:

```bash
docker compose logs -f kafka
```

종료:

```bash
docker compose down
```

## 기본 실행

터미널 1에서 consumer를 먼저 실행합니다.

```bash
./consumer/consumer \
  --batch-size 20000 \
  --max-batches 5 \
  --threads 4 \
  --work-factor 64 \
  --compare-sequential \
  --csv results/metrics.csv
```

터미널 2에서 producer를 실행합니다.

```bash
./producer/producer \
  --messages 120000 \
  --rate 5000 \
  --malicious-percent 5
```

Producer 옵션:

- `--messages N`: 생성할 로그 수, `0`은 무한 실행
- `--rate N`: 초당 목표 메시지 수, `0`은 제한 없음
- `--malicious-percent N`: 악성 로그 비율
- `--brokers HOSTS`, `--topic TOPIC`: Kafka 접속 설정

Consumer 옵션:

- `--batch-size N`: 한 번에 분석할 로그 수
- `--max-batches N`: 처리할 배치 수, `0`은 무한 실행
- `--threads N`: OpenMP 스레드 수
- `--work-factor N`: 로그 한 줄당 시그니처 스캔 반복 횟수. 실험에서는 너무 짧은 측정 시간을 피하기 위해 `64` 권장
- `--compare-sequential`: 순차 스캔 기준 성능도 측정
- `--csv PATH`: 보고서 그래프용 CSV 저장

## 실험 예시

본 보고서에서는 OpenMP thread 수에 따른 Consumer 분석 단계의 throughput과 speedup을 측정했습니다.

실험 조건:

- Kafka topic: `security_logs`
- Kafka partitions: 4
- Batch size: 50,000 logs
- Number of batches: 5
- OpenMP threads: 1, 2, 4, 8
- Work factor: 64
- Producer messages: 500,000
- Malicious log ratio: 5%
- Output CSV: `results/thread_scaling.csv`

스레드 수에 따른 throughput/speedup CSV를 생성하려면 Kafka를 먼저 실행한 뒤 스크립트를 실행합니다.

```bash
docker compose up -d
bash scripts/thread_scaling.sh
```

환경변수로 실험 조건을 바꿀 수 있습니다.

```bash
THREADS="1 2 4 8" BATCH_SIZE=50000 BATCHES=5 MESSAGES=500000 WORK_FACTOR=64 bash scripts/thread_scaling.sh
```

기본 스크립트는 위 실험 조건과 동일한 설정으로 실행됩니다. 기존 CSV는 스크립트 실행 시 새로 생성됩니다.

CSV 컬럼:

```text
batch,threads,batch_size,threats,parallel_ms,parallel_logs_per_sec,sequential_ms,speedup
```

보고서에는 `parallel_logs_per_sec` 평균을 thread count별로 묶어 `Throughput vs. thread count` 그래프로, `speedup` 평균을 `Speedup vs. thread count` 그래프로 사용하면 됩니다.

보고서에 사용한 평균 결과:

| Threads | Avg. Processing Time (ms) | Avg. Throughput (logs/sec) | Avg. Speedup |
|---:|---:|---:|---:|
| 1 | 178.677 | 281,121 | 1.00x |
| 2 | 92.102 | 545,741 | 1.79x |
| 4 | 73.251 | 684,965 | 2.66x |
| 8 | 59.664 | 877,821 | 2.81x |

주의: 여기서 throughput은 Kafka end-to-end 처리량이 아니라 Consumer가 batch를 수집한 이후 수행하는 OpenMP 분석 단계의 처리량입니다.

실험 후 Kafka/Zookeeper를 종료합니다.

```bash
docker compose down
```

## 아키텍처

```text
producer process
  normal/malicious log generator
        |
        v
Kafka topic: security_logs, 4 partitions
        |
        v
consumer process
  batch buffer
        |
        v
OpenMP parallel signature scan
        |
        v
metrics stdout + CSV
```

## 병목 분석 포인트

- Producer 전송률이 낮으면 consumer/OpenMP 단계가 충분히 포화되지 않습니다.
- 작은 batch size에서는 OpenMP 스레드 생성/동기화 오버헤드가 커져 speedup이 낮을 수 있습니다.
- batch size를 크게 하면 throughput은 개선될 수 있지만 end-to-end latency가 증가합니다.
- Kafka 단일 broker localhost 구성은 네트워크 병목보다 broker 큐잉과 consumer poll 속도가 더 크게 보일 수 있습니다.
