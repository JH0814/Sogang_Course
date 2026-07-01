#!/usr/bin/env bash
set -euo pipefail

BROKERS="${BROKERS:-localhost:9092}"
TOPIC="${TOPIC:-security_logs}"
BATCH_SIZE="${BATCH_SIZE:-50000}"
BATCHES="${BATCHES:-5}"
MESSAGES="${MESSAGES:-500000}"
RATE="${RATE:-0}"
THREADS="${THREADS:-1 2 4 8}"
WORK_FACTOR="${WORK_FACTOR:-64}"
OUT="${OUT:-results/thread_scaling.csv}"

mkdir -p "$(dirname "$OUT")"
rm -f "$OUT"

make all

./producer/producer \
  --brokers "$BROKERS" \
  --topic "$TOPIC" \
  --messages "$MESSAGES" \
  --rate "$RATE" \
  --malicious-percent 5

for t in $THREADS; do
  GROUP_ID="thread_scaling_${t}_$(date +%s)"
  ./consumer/consumer \
    --brokers "$BROKERS" \
    --topic "$TOPIC" \
    --group-id "$GROUP_ID" \
    --batch-size "$BATCH_SIZE" \
    --max-batches "$BATCHES" \
    --threads "$t" \
    --work-factor "$WORK_FACTOR" \
    --compare-sequential \
    --csv "$OUT"
done

echo "wrote $OUT"
