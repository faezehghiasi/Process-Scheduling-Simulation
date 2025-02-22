#!/usr/bin/env bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root."
    exit 1
fi

make clean
make build

io_ratios=(0.1 0.6)
scheduling_policies=("SCHED_OTHER" "SCHED_RR" "SCHED_FIFO")
output_file="simulation_results.txt"

echo "Simulation Results" > "$output_file"
echo "Starting simulations..."

for io_ratio in "${io_ratios[@]}"; do
    for policy in "${scheduling_policies[@]}"; do
        echo "Running simulation with IO_RATIO=$io_ratio and POLICY=$policy..."
        echo "IO_RATIO=$io_ratio, POLICY=$policy" >> "$output_file"
        ./main "$io_ratio" "$policy" >> "$output_file"
        echo "-----------------------------------------" >> "$output_file"
    done
done

echo "Simulations completed. Results saved in $output_file."
