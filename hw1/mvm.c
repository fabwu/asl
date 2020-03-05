//#error Please comment out the next two lines under linux, then comment this error
//#include "stdafx.h"  //Visual studio expects this line to be the first one, comment out if different compiler
//#include <windows.h> // Include if under windows

#ifndef WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "tsc_x86.h"

#define NUM_RUNS 1
#define CYCLES_REQUIRED 1e8
#define FREQUENCY 1.9e9
#define CALIBRATE

/*
 *	Initialize the input
 */
void fill_matrix(double * A, int n) {
    for(int i=0; i < n; i++) {
        for(int j=0; j < n; j++) {
            A[n*i+j] = (double) i / (j+1) + 1.0;
        }
    }
}

void fill_vector(double * x, int n) {
    for(int i=0; i < n; i++) {
        x[i] = (double) i + 1.0;
    }
}

/* 
 * Straightforward implementation of matrix-vector multiplication
 */
void compute(double A[], double x[], double y[], int n) {
    for(int i = 0; i < n; i++) {
        double sum = 0.0;
        for(int j = 0; j < n; j++) {
            sum = sum + A[n*i+j]*x[j];
        }
        y[i] = sum;
    }
}

/* 
 * Timing function based on the TimeStep Counter of the CPU.
 */
double rdtsc(double A[], double x[], double y[], int n) {
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
            compute(A, x, y, n);
        }
        cycles = stop_tsc(start);

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    start = start_tsc();
    for (i = 0; i < num_runs; ++i) {
        compute(A, x, y, n);
    }

    cycles = stop_tsc(start)/num_runs;
    return (double) cycles;
}

double c_clock(double A[], double x[], double y[], int n) {
    int i, num_runs;
    double cycles;
    clock_t start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        start = clock();
        for (i = 0; i < num_runs; ++i) {
            compute(A, x, y, n);
        }
        end = clock();

        cycles = (double)(end-start);

        // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of CLOCKS_PER_SEC
        if(cycles >= CYCLES_REQUIRED/(FREQUENCY/CLOCKS_PER_SEC)) break;

        num_runs *= 2;
    }
#endif

    start = clock();
    for(i=0; i<num_runs; ++i) { 
        compute(A, x, y, n);
    }
    end = clock();

    return (double)(end-start)/num_runs;
}

#ifndef WIN32
double timeofday(double A[], double x[], double y[], int n) {
    int i, num_runs;
    double cycles;
    struct timeval start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        gettimeofday(&start, NULL);
        for (i = 0; i < num_runs; ++i) {
            compute(A, x, y, n);
        }
        gettimeofday(&end, NULL);

        cycles = (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6)*FREQUENCY;

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    gettimeofday(&start, NULL);
    for(i=0; i < num_runs; ++i) {
        compute(A, x, y, n);
    }
    gettimeofday(&end, NULL);

    return (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6)/ num_runs;
}

#else

double gettickcount(double A[], double x[], double y[], int n) {
    int i, num_runs;
    double cycles, start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        start = (double)GetTickCount();
        for (i = 0; i < num_runs; ++i) {
            compute(A, x, y, n);
        }
        end = (double)GetTickCount();

        cycles = (end-start)*FREQUENCY/1e3; // end-start provides a measurement in the order of milliseconds

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    start = (double)GetTickCount();
    for(i=0; i < num_runs; ++i) {
        compute(A, x, y, n); 
    }
    end = (double)GetTickCount();

    return (end-start)/num_runs;
}

double queryperfcounter(double A[], double x[], double y[], int n, LARGE_INTEGER f) {
    int i, num_runs;
    double cycles;
    LARGE_INTEGER start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        QueryPerformanceCounter(&start);
        for (i = 0; i < num_runs; ++i) {
            compute(A, x, y, n);
        }
        QueryPerformanceCounter(&end);

        cycles = (double)(end.QuadPart - start.QuadPart);

        // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of f
        if(cycles >= CYCLES_REQUIRED/(FREQUENCY/f.QuadPart)) break; 

        num_runs *= 2;
    }
#endif

    QueryPerformanceCounter(&start);
    for(i=0; i < num_runs; ++i) {
        compute(A, x, y, n); 
    }
    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart)/num_runs;
}

#endif

int main(int argc, char **argv) {
//  if (argc!=2) {printf("usage: FW <n>\n"); return -1;}
//  int n = atoi(argv[1]);
//  printf("n=%d \n",n);
    for(int n = 200; n <= 4000; n += 200) {
      double* A = (double *)malloc(n*n*sizeof(double));
      double* x = (double *)malloc(n*sizeof(double));
      double* y = (double *)calloc(n,sizeof(double));

      fill_matrix(A, n);
      fill_vector(x, n);
      double r = rdtsc(A, x, y, n);
      double flops_per_cycle = (2*n*n)/r;
      printf("%d %lf\n", n, flops_per_cycle);
    }
/*    printf("RDTSC instruction:\n %lf cycles measured => %lf seconds, assuming frequency is %lf MHz. (change in source file if different)\n\n", r, r/(FREQUENCY), (FREQUENCY)/1e6);


    double c = c_clock(A, x, y, n);
    printf("C clock() function:\n %lf cycles measured. On some systems, this number seems to be actually computed from a timer in seconds then transformed into clock ticks using the variable CLOCKS_PER_SEC. Unfortunately, it appears that CLOCKS_PER_SEC is sometimes set improperly. (According to this variable, your computer should be running at %lf MHz). In any case, dividing by this value should give a correct timing: %lf seconds. \n\n",c, (double) CLOCKS_PER_SEC/1e6, c/CLOCKS_PER_SEC);


#ifndef WIN32
    double t = timeofday(A, x, y, n);
    printf("C gettimeofday() function:\n %lf seconds measured\n\n",t);
#else
    LARGE_INTEGER f;
    double t = gettickcount(A, x, y, n);
    printf("Windows getTickCount() function:\n %lf milliseconds measured\n\n",t);

    QueryPerformanceFrequency((LARGE_INTEGER *)&f);

    double p = queryperfcounter(A, x, y, n, f);
    printf("Windows QueryPerformanceCounter() function:\n %lf cycles measured => %lf seconds, with reported CPU frequency %lf MHz\n\n",p,p/f.QuadPart,(double)f.QuadPart/1000);
#endif*/
    
    return 0;
}

