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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IO_RATIO> <SCHED_POLICY>\n", argv[0]);
        return EXIT_FAILURE;
    }

    float io_ratio = atof(argv[1]);
    if (io_ratio < 0 || io_ratio > 1) {
        fprintf(stderr, "IO_RATIO must be between 0 and 1\n");
        return EXIT_FAILURE;
    }

    int policy;
    if (strcmp(argv[2], "SCHED_RR") == 0) {
        policy = SCHED_RR;
    } else if (strcmp(argv[2], "SCHED_FIFO") == 0) {
        policy = SCHED_FIFO;
    } else if (strcmp(argv[2], "SCHED_OTHER") == 0) {
        policy = SCHED_OTHER;
    } else {
        fprintf(stderr, "Invalid scheduling policy\n");
        return EXIT_FAILURE;
    }

    pid_t pids[NUM_PROCESSES];
    int io_count = (int)(NUM_PROCESSES * io_ratio);

    struct timespec start_times[NUM_PROCESSES], end_times[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start_times[i]); 
        if ((pids[i] = fork()) == 0) {
            set_scheduler_policy(getpid(), policy);
            if (i < io_count) {
                execl("./io_bound", "io_bound", NULL);
            } else {
                execl("./cpu_bound", "cpu_bound", NULL);
            }
            perror("execl");
            exit(EXIT_FAILURE);
        } else if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_times[i]); 
    }

    printf("%-10s %-10s %-15s %-15s %-15s\n", "PID", "Type", "Start Time", "End Time", "Turnaround Time");

    double total_turnaround = 0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        double start = start_times[i].tv_sec + start_times[i].tv_nsec / 1e9;
        double end = end_times[i].tv_sec + end_times[i].tv_nsec / 1e9;
        double turnaround = end - start;

        total_turnaround += turnaround;
        printf("%-10d %-10s %-15f %-15f %-15f\n", pids[i], (i < io_count ? "IO" : "CPU"), start, end, turnaround);
    }

    printf("Average Turnaround Time: %f\n", total_turnaround / NUM_PROCESSES);

    return 0;
}
