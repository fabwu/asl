#include "include/microbenchmark.h"
#include "include/tsc_x86.h"
#include "include/sigmoid.h"
#include <stdio.h>

void initialize_microbenchmark_data (microbenchmark_mode_t mode) {
    switch (mode) {
        case SQRT_LAT_HI:
        case SQRT_GAP_HI:
        case SIGMOID1_LAT_HI:
        case SIGMOID1_GAP_HI:
        case SIGMOID2_LAT_HI:
        case SIGMOID2_GAP_HI:
        case SQRT_LAT_LO:
        case SQRT_GAP_LO:
        case SIGMOID1_LAT_LO:
        case SIGMOID1_GAP_LO:
        case SIGMOID2_LAT_LO:
        case SIGMOID2_GAP_LO:
        default: break;
    }
}

// latency is 18
// min is 13
double microbenchmark_get_sqrt_latency() {
    /* Implement your microbenchmark benchmark here */
    int num_runs = 100;
    volatile double x = 1.0;

    myInt64 start = start_tsc();
    for (int i = 1; i < num_runs; ++i) {
        x = sqrtsd(x);
    }

    myInt64 cycles = stop_tsc(start)/num_runs;
    
    return (double) cycles;
}

// gap is 6
double microbenchmark_get_sqrt_gap() {
    /* Implement your microbenchmark benchmark here */
    int num_runs = 1000;

    myInt64 start = start_tsc();
    for (int i = 1; i < num_runs; ++i) {
        volatile double y1 = sqrtsd(1.0);
        volatile double y2 = sqrtsd(2.0);
        volatile double y3 = sqrtsd(3.0);
        volatile double y4 = sqrtsd(4.0);
        volatile double y5 = sqrtsd(5.0);
        volatile double y6 = sqrtsd(6.0);
        volatile double y7 = sqrtsd(6.0);
        volatile double y8 = sqrtsd(6.0);
        volatile double y9 = sqrtsd(6.0);
        volatile double y10 = sqrtsd(6.0);
        volatile double y11 = sqrtsd(6.0);
    }

    myInt64 cycles = stop_tsc(start)/num_runs/11;
    
    return (double) cycles;
}

double microbenchmark_get_sigmoid1_latency() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

double microbenchmark_get_sigmoid1_gap() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

// sigmoid 2 1.0 is 95 cycles
double microbenchmark_get_sigmoid2_latency() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

double microbenchmark_get_sigmoid2_gap() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}
