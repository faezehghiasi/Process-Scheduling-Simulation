# Simulation Scheduling Project

## Overview
This project simulates different process scheduling policies by running CPU-bound and I/O-bound tasks under various scheduling strategies. The objective is to analyze how different scheduling algorithms impact the turnaround time of processes.

## Files Overview

### Source Code Files
- **cpu_bound.c**: Implements a CPU-intensive computation to simulate CPU-bound tasks.
- **io_bound.c**: Simulates an I/O-heavy process by performing repeated I/O operations.
- **main.c**:
  - Manages process creation and execution.
  - Sets scheduling policies (`SCHED_OTHER`, `SCHED_RR`, `SCHED_FIFO`).
  - Assigns tasks based on the specified I/O ratio.
  - Measures turnaround time for analysis.

### Supporting Files
- **Makefile**:
  - Defines rules for compiling the project.
  - Provides `make build` and `make clean` targets.
- **simulation.sh**:
  - Automates compilation and execution.
  - Runs simulations with different scheduling policies and I/O ratios.
  - Stores results in `simulation_results.txt`.
- **simulation_results.txt**: Stores the output logs of the simulations for later analysis.

## Prerequisites
Ensure that `gcc` is installed on your system. This project is intended for Linux-based operating systems. To install the necessary dependencies, run:
```bash
sudo apt update && sudo apt install gcc make
```

## Compilation & Execution

### 1. Build the Project
Compile the source code using:
```bash
make build
```
This will generate the following binaries:
- `cpu_bound`
- `io_bound`
- `main`

### 2. Run the Simulation
Since modifying scheduling policies requires root privileges, execute the script with:
```bash
sudo ./simulation.sh
```

### 3. View Results
After execution, check the simulation results using:
```bash
cat simulation_results.txt
```

## How It Works
1. The script first ensures it is executed with root privileges.
2. It compiles the project using `make build`.
3. The script iterates through different **I/O ratios** and **scheduling policies**:
   - **I/O Ratios Tested**: `0.1`, `0.6`
   - **Scheduling Policies**: `SCHED_OTHER`, `SCHED_RR`, `SCHED_FIFO`
4. For each combination, the `main` program is executed, simulating 50 processes.
5. The turnaround time for each process is recorded and stored in `simulation_results.txt`.

## Customization
You can modify the simulation parameters in `simulation.sh`:
- Adjust the `io_ratios` array to test different I/O ratios.
- Change the `scheduling_policies` array to include or exclude policies.

## Example Output Format
The results file (`simulation_results.txt`) will contain entries like:
```
IO_RATIO=0.1, POLICY=SCHED_RR
PID       Type       Start Time      End Time        Turnaround Time
12345     IO         0.123456        0.567890        0.444434
...
Average Turnaround Time: 0.345678
-----------------------------------------
```

## Notes
- The `main.c` file creates **50 child processes**, assigning them as CPU-bound or I/O-bound based on the specified ratio.
- `cpu_bound.c` executes a loop-intensive task, while `io_bound.c` performs periodic file writes and sleeps.
- The scheduling policy affects process execution order and turnaround time.

## Cleaning Up
To remove compiled binaries and reset the project, run:
```bash
make clean
```

## Conclusion
This simulation provides insights into how different scheduling strategies impact CPU-bound and I/O-bound processes. The recorded results help analyze the efficiency of scheduling algorithms under varying workloads.


