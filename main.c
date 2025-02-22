#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <time.h>
#include <string.h>

#define NUM_PROCESSES 50

void set_scheduler_policy(pid_t pid, int policy) {
    struct sched_param param;
    param.sched_priority = (policy == SCHED_RR || policy == SCHED_FIFO) ? 1 : 0;
    
    if (sched_setscheduler(pid, policy, &param) < 0) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
}

int get_scheduling_policy(const char *policy_str) {
    if (strcmp(policy_str, "SCHED_RR") == 0) {
        return SCHED_RR;
    } else if (strcmp(policy_str, "SCHED_FIFO") == 0) {
        return SCHED_FIFO;
    } else if (strcmp(policy_str, "SCHED_OTHER") == 0) {
        return SCHED_OTHER;
    } else {
        fprintf(stderr, "Invalid scheduling policy\n");
        exit(EXIT_FAILURE);
    }
}

void validate_input(int argc, char *argv[], float *io_ratio, int *policy) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IO_RATIO> <SCHED_POLICY>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    *io_ratio = atof(argv[1]);
    if (*io_ratio < 0 || *io_ratio > 1) {
        fprintf(stderr, "IO_RATIO must be between 0 and 1\n");
        exit(EXIT_FAILURE);
    }

    *policy = get_scheduling_policy(argv[2]);
}

pid_t create_child_process(int index, int io_count, int policy) {
    pid_t pid = fork();
    
    if (pid == 0) {
        set_scheduler_policy(getpid(), policy);

        if (index < io_count) {
            execl("./io_bound", "io_bound", NULL);
        } else {
            execl("./cpu_bound", "cpu_bound", NULL);
        }

        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return pid;
}

void measure_turnaround_time(pid_t *pids, struct timespec *start_times, struct timespec *end_times, int io_count) {
    double total_turnaround = 0;

    printf("%-10s %-10s %-15s %-15s %-15s\n", "PID", "Type", "Start Time", "End Time", "Turnaround Time");

    for (int i = 0; i < NUM_PROCESSES; i++) {
        double start = start_times[i].tv_sec + start_times[i].tv_nsec / 1e9;
        double end = end_times[i].tv_sec + end_times[i].tv_nsec / 1e9;
        double turnaround = end - start;

        total_turnaround += turnaround;
        printf("%-10d %-10s %-15f %-15f %-15f\n", pids[i], (i < io_count ? "IO" : "CPU"), start, end, turnaround);
    }

    printf("Average Turnaround Time: %f\n", total_turnaround / NUM_PROCESSES);
}

int main(int argc, char *argv[]) {
    float io_ratio;
    int policy;

    validate_input(argc, argv, &io_ratio, &policy);

    pid_t pids[NUM_PROCESSES];
    int io_count = (int)(NUM_PROCESSES * io_ratio);
    struct timespec start_times[NUM_PROCESSES], end_times[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start_times[i]);
        pids[i] = create_child_process(i, io_count, policy);
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_times[i]);
    }

    measure_turnaround_time(pids, start_times, end_times, io_count);

    return 0;
}
