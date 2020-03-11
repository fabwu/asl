#include <stdio.h>
#include <stdlib.h>

#include "tsc_x86.h"

#define NUM_RUNS 1
#define CYCLES_REQUIRED 1e8
#define CALIBRATE
#define NUM_MEASUREMENTS 30

void fill_vector(double *x, int n) {
  for(int i = 0; i < n; i++) {
    x[i] = rand() / (double)RAND_MAX;
  }
}

void print_vector(double *x, int n) {
  printf("[");
  for(int i=0; i<n; i++) {
    printf("%.2f ", x[i]);
  }
  printf("]\n");
}

/**
 * The vector z overflows even for small numbers. We could store the 
 * result in a different vector, but this wouldn't be the same calculation.
 */
void compute(double x[], double y[], double u[], double z[], int n) {
  for(int i = 0; i < n; i++) {
    z[i] = z[i] + u[i]*u[i]*u[i] + x[i]*y[i]*z[i];
  }  
}

/* 
 * Timing function based on the TimeStep Counter of the CPU.
 */
double rdtsc(double x[], double y[], double u[], double z[], int n) {
    int i, num_runs;
    myInt64 cycles;
    myInt64 start;
    num_runs = NUM_RUNS;

    /* 
     * The CPUID instruction serializes the pipeline.
     * Using it, we can create execution barriers around the code we want to time.
     * The calibrate section is used to make the computation large enough so as to 
     * avoid measurements bias due to the timing overhead.
     */
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        start = start_tsc();
        for (i = 0; i < num_runs; ++i) {
            compute(x, y, u, z, n);
        }
        cycles = stop_tsc(start);

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif
    start = start_tsc();

    for (i = 0; i < num_runs; ++i) {
        compute(x, y, u, z, n);
    }

    cycles = stop_tsc(start)/num_runs;
    return (double) cycles;
}

int compare(const void *pa, const void *pb)
{
    const double a = *(double *)pa;
    const double b = *(double *)pb;

    if(a < b) return -1;
    if(a > b) return 1;

    return 0;
}


int main(int argc, char **argv) {
    for(int i = 4; i <= 23; ++i) {
        int n = (1<<i);
    
        double* x = (double *)malloc(n*sizeof(double));
        double* y = (double *)malloc(n*sizeof(double));
        double* u = (double *)malloc(n*sizeof(double));
        double* z = (double *)malloc(n*sizeof(double));
                        
        double r[NUM_MEASUREMENTS];
        double num_ops = 6*n;

        for(int j = 0; j < NUM_MEASUREMENTS; ++j) {
            fill_vector(x, n);
            fill_vector(y, n);
            fill_vector(u, n);
            fill_vector(z, n);

            double cycles = rdtsc(x, y, u, z, n);

            r[j] = num_ops / cycles;
        }

        free(x);
        free(y);
        free(u);
        free(z);
        
        qsort(r, NUM_MEASUREMENTS, sizeof(double), compare);

        printf("%d %lf\n", n, r[NUM_MEASUREMENTS/2]);
    }
}

