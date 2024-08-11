#!/bin/bash

make rebuild

# Parameters
NUM_INSTANCES=1000
DELAY=0.1  # 10 instances per second (0.1 seconds delay between each instance)

run_client() {
  local instance_id=$1
  echo "$(date '+%Y-%m-%d %H:%M:%S') - Starting client instance $instance_id"
  ./build-debug/client-manual
  echo "$(date '+%Y-%m-%d %H:%M:%S') - Client instance $instance_id finished"
}

# Start instances
for i in $(seq 1 $NUM_INSTANCES); do
  run_client "$i" &
  sleep "$DELAY"
done

# Wait for all instances to complete
wait
echo "$(date '+%Y-%m-%d %H:%M:%S') - All client instances have completed" >> "$LOGFILE"
