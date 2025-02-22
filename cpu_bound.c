#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cpu_bound_task() {
    unsigned long long int i = 0;
    while (i < 1e9) {
        i++;
    }
}

int main() {
    cpu_bound_task();
    return 0;
}