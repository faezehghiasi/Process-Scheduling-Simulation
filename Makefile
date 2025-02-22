# Compiler
CC = gcc
CFLAGS = -lrt

# Targets
TARGETS = cpu_bound io_bound main

# Default target: Build all
all: build

# Compile the programs
cpu_bound: cpu_bound.c
	$(CC) $< -o $@

io_bound: io_bound.c
	$(CC) $< -o $@

main: main.c
	$(CC) $< -o $@ $(CFLAGS)

build: $(TARGETS)

# Clean target
clean:
	rm -f $(TARGETS) simulation_results.txt

.PHONY: all build clean
