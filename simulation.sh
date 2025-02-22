#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root."
    exit 1
fi
make_programs() {
    gcc cpu_bound.c -o cpu_bound
    gcc io_bound.c -o io_bound
    gcc main.c -o main -lrt
}
clean_and_build() {
    rm -f cpu_bound io_bound main
    make_programs
}
clean_and_build
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
