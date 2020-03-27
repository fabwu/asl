/**
*      _________   _____________________  ____  ______
*     / ____/   | / ___/_  __/ ____/ __ \/ __ \/ ____/
*    / /_  / /| | \__ \ / / / /   / / / / / / / __/
*   / __/ / ___ |___/ // / / /___/ /_/ / /_/ / /___
*  /_/   /_/  |_/____//_/  \____/\____/_____/_____/
*
*  http://www.acl.inf.ethz.ch/teaching/fastcode
*  How to Write Fast Numerical Code 263-2300 - ETH Zurich
*  Copyright (C) 2019 
*                   Tyler Smith        (smitht@inf.ethz.ch) 
*                   Alen Stojanov      (astojanov@inf.ethz.ch)
*                   Gagandeep Singh    (gsingh@inf.ethz.ch)
*                   Markus Pueschel    (pueschel@inf.ethz.ch)
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program. If not, see http://www.gnu.org/licenses/.
*/
//#include "stdafx.h"

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <random>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "tsc_x86.h"
#include "common.h"

using namespace std;


#define CYCLES_REQUIRED 1e8
#define REP 100
#define EPS (1e-3)

double T1[NR*NR] = {0};
double T2[NR*NR] = {0};
double T3[NR*NR] = {0};

void init_matrices() {
    for (int i = 0; i < NR/2; ++i) {
        T1[(i+0)*NR + i+0] =  1;
        T1[(i+0)*NR + i+4] =  1;
        T1[(i+4)*NR + i+0] =  1;
        T1[(i+4)*NR + i+4] = -1;
    }

    for (int j = 0; j < NR; j+=4) {
        for (int i = 0; i < NR / 4; i+=1) {
            T2[(i + j + 0)*NR + i + j + 0] = 1;
            T2[(i + j + 0)*NR + i + j + 2] = 1;
            T2[(i + j + 2)*NR + i + j + 0] = 1;
            T2[(i + j + 2)*NR + i + j + 2] = -1;
        }
    }

    for (int i = 0; i < NR; i+=2) {
        T3[(i+0)*NR + i+0] =   1;
        T3[(i+0)*NR + i+1] =   1;
        T3[(i+1)*NR + i+0] =   1;
        T3[(i+1)*NR + i+1] =  -1;
    }
}

void wht_base_row(double* x, double* y) {

    double t1[NR];
    double t2[NR];

    /* First transform */
    for (int i = 0; i < NR; ++i) {
        double sum = 0.0;
        for (int j = 0; j < NR; ++j) {
            sum += T1[i*NR + j] * x[j];
        }
        t1[i] = sum;
    }

    /* Second transform */
    for (int i = 0; i < NR; ++i) {
        double sum = 0.0;
        for (int j = 0; j < NR; ++j) {
            sum += T2[i*NR + j] * t1[j];
        }
        t2[i] = sum;
    }

    /* Third transform */
    for (int i = 0; i < NR; ++i) {
        double sum = 0.0;
        for (int j = 0; j < NR; ++j) {
            sum += T3[i*NR + j] * t2[j];
        }
        y[i] = sum;
    }
}

void wht_base(double* x, double* y) {

    for (int i = 0; i < MR; ++i) {
        wht_base_row(x + NR*i, y + NR*i);
    }
}

/* prototype of the function you need to optimize */
typedef void(*comp_func)(double *, double *);

//headers
void   register_functions();
double perf_test(comp_func f, string desc, int flops);
void add_function(comp_func f, string name, int flop);

/* Global vars, used to keep track of student functions */
vector<comp_func> userFuncs;
vector<string> funcNames;
vector<int> funcFlops;
int numFuncs = 0;


void rands(double * m, size_t row, size_t col)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (size_t i = 0; i < row*col; ++i)  
        m[i] = dist(gen);
}

void build(double **a, int m, int n)
{
    *a = static_cast<double *>(aligned_alloc(32, m * n * sizeof(double)));
    rands(*a, m, n);
}

void destroy(double * m)
{
    free(m);
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions();


double nrm_sqr_diff(double *x, double *y, int n) {
    double nrm_sqr = 0.0;
    for(int i = 0; i < n; i++) {
        nrm_sqr += (x[i] - y[i]) * (x[i] - y[i]);
    }
    
    if (isnan(nrm_sqr)) {
      nrm_sqr = INFINITY;
    }
    
    return nrm_sqr;
}

/*
* Registers a user function to be tested by the driver program. Registers a
* string description of the function as well
*/
void add_function(comp_func f, string name, int flops)
{
    userFuncs.push_back(f);
    funcNames.emplace_back(name);
    funcFlops.push_back(flops);

    numFuncs++;
}


/*
* Checks the given function for validity. If valid, then computes and
* reports and returns the number of cycles required per iteration
*/
double perf_test(comp_func f, string desc, int flops)
{
    double cycles = 0.;
    long num_runs = 100;
    double multiplier = 1;
    myInt64 start, end;

    double *x,*y;
    int n = MR*NR;

    build(&x, 1, n);
    build(&y, 1, n);

    // Warm-up phase: we determine a number of executions that allows
    // the code to be executed for at least CYCLES_REQUIRED cycles.
    // This helps excluding timing overhead when measuring small runtimes.
    do {
        num_runs = num_runs * multiplier;
        start = start_tsc();
        for (size_t i = 0; i < num_runs; i++) {
            f(x, y);           
        }
        end = stop_tsc(start);

        cycles = (double)end;
        multiplier = (CYCLES_REQUIRED) / (cycles);
        
    } while (multiplier > 2);

    list<double> cyclesList;

    // Actual performance measurements repeated REP times.
    // We simply store all results and compute medians during post-processing.
    for (size_t j = 0; j < REP; j++) {

        start = start_tsc();
        for (size_t i = 0; i < num_runs; ++i) {
            f(x, y);
        }
        end = stop_tsc(start);

        cycles = ((double)end) / num_runs;

        cyclesList.push_back(cycles);
    }

    destroy(x);
    destroy(y);
    cyclesList.sort();
    cycles = cyclesList.front();
    return  (1.0 * flops) / cycles;
}

int main(int argc, char **argv)
{
  cout << "Starting program. ";
  double perf;
  int i;

  register_functions();
  init_matrices();

  if (numFuncs == 0){
    cout << endl;
    cout << "No functions registered - nothing for driver to do" << endl;
    cout << "Register functions by calling register_func(f, name)" << endl;
    cout << "in register_funcs()" << endl;

    return 0;
  }
  cout << numFuncs << " functions registered." << endl;
   
    //Check validity of functions. 
  int n = MR*NR;
  double *x, *y, *y_old, *y_base;
  build(&x, 1, n);
  build(&y, 1, n);
  y_base = static_cast<double *>(malloc(n * sizeof(double)));
  y_old  = static_cast<double *>(malloc(n * sizeof(double)));
  
  memcpy(y_old,  y, n*sizeof(double));
  wht_base(x, y);
  memcpy(y_base, y, n*sizeof(double));

  for (i = 0; i < numFuncs; i++) {
    memcpy(y, y_old, n*sizeof(double));
    comp_func f = userFuncs[i];
    f(x, y);
    double error = nrm_sqr_diff(y, y_base, n);

    if (error > EPS) {
      cout << error << endl;
      cout << "ERROR!!!!  the results for the " << i+1 << "th function are different to the previous" << std::endl;
    }
  }
  destroy(x);
  destroy(y);
  destroy(y_base);


  for (i = 0; i < numFuncs; i++)
  {
    perf = perf_test(userFuncs[i], funcNames[i], 24*MR);
    cout << endl << "Running: " << funcNames[i] << endl;
    cout << perf << " flops / cycles" << endl;
  }

  return 0;
}
