#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 10; i++) {
       
        FILE *f = fopen("/dev/null", "w");
        if (f) {
            fprintf(f, "Simulating I/O operation %d\n", i);
            fclose(f);
        }
        usleep(100000);
    }
    return 0;
}
