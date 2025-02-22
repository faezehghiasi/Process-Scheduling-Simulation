# Simulation Scheduling Project

## Overview
This project simulates process scheduling policies by running CPU-bound and I/O-bound tasks under different scheduling strategies. The simulation results are recorded for further analysis.

## Files Overview
- **cpu_bound.c**: Implements a CPU-intensive computation to simulate CPU-bound tasks.
- **io_bound.c**: Simulates an I/O-heavy process by performing repeated I/O operations.
- **main.c**: Manages process creation, applies scheduling policies, and executes the simulations.
- **simulation.sh**: Compiles the source code, runs the simulations with different scheduling policies and I/O ratios, and saves the results in `simulation_results.txt`.
- **simulation_results.txt**: Stores the output of the simulations for later review.

## Prerequisites
Ensure you have GCC installed before running the script. You can install it using:
```bash
sudo apt update && sudo apt install gcc
```

## Usage
### 1. Grant Execute Permission
Before running the script, ensure it has execution permission:
```bash
chmod +x simulation.sh
```

### 2. Run the Script with Root Privileges
Since the script modifies scheduling policies, it requires root access:
```bash
sudo ./simulation.sh
```

### 3. View the Results
After execution, the simulation results will be stored in `simulation_results.txt`. You can view them using:
```bash
cat simulation_results.txt
```

## How It Works
1. The script first checks if it is run as root. If not, it exits with an error.
2. It compiles `cpu_bound.c`, `io_bound.c`, and `main.c`.
3. It removes any previous compiled binaries and recompiles them.
4. The script then iterates through different I/O ratios and scheduling policies:
   - **I/O Ratios**: `0.1`, `0.6`
   - **Scheduling Policies**: `SCHED_OTHER`, `SCHED_RR`, `SCHED_FIFO`
5. It runs `main` with each combination of I/O ratio and scheduling policy, appending the results to `simulation_results.txt`.


