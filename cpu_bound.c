#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOOP_LIMIT 1000000000ULL  

void cpu_bound_task() {
    unsigned long long int i = 0;
    while (i < LOOP_LIMIT) {
        i++;
    }
}

int main() {
    cpu_bound_task();
    return 0;
}
