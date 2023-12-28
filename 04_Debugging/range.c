#include <stdio.h>

int
main(int argc, char **argv) {
    int start = 1;
    int step = 1;
    int stop = 0;

    if (argc == 1) {
        printf("Usage: %s [start] stop [step]\n", argv[0]);
    } else if (argc == 2) {
        sscanf(argv[1], "%d", &stop);
    } else if (argc == 3) {
        sscanf(argv[1], "%d", &start);
        sscanf(argv[2], "%d", &stop); 
    } else if (argc == 4) {
        sscanf(argv[1], "%d", &start);
        sscanf(argv[2], "%d", &stop);
        sscanf(argv[3], "%d", &step);
        if (step <= 0) {
            return 1;
        }
    } 

    int i = start;
    for (; i < stop; i += step) {
        printf("%d\n", i);
    }
    return 0;
}
