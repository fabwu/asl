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

using namespace std;

#define NR 32
#define CYCLES_REQUIRED 1e8
#define REP 50
#define EPS (1e-3)


void kernel_base(double* A, double *x, double *y) {
  for (int i = 0; i < NR; ++i) {
    for (int j = 0; j < NR; ++j) {
      y[i] += (j + 2) * A[i*NR + j] * x[j];
    }
  }
  
  for (int i = 0; i < NR; ++i) {
    if (y[i] >= 0) {
      y[i] += 1;
    }
  }
}

/* prototype of the function you need to optimize */
typedef void(*comp_func)(double *, double *, double*);

//headers
void   register_functions();
double get_perf_score(comp_func f);
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
    *a = static_cast<double *>(aligned_alloc(32, m * n * sizeof(double))); //TODO: align this to 32
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

    double *A, *x, *y;
    int n = NR;

    build(&A, n, n);
    build(&x, 1, n);
    build(&y, 1, n);

    // Warm-up phase: we determine a number of executions that allows
    // the code to be executed for at least CYCLES_REQUIRED cycles.
    // This helps excluding timing overhead when measuring small runtimes.
    do {
        num_runs = num_runs * multiplier;
        start = start_tsc();
        for (size_t i = 0; i < num_runs; i++) {
            f(A, x, y);           
        }
        end = stop_tsc(start);

        cycles = (double)end;
        multiplier = (CYCLES_REQUIRED) / (cycles);
        
    } while (multiplier > 2);

    list<double> cyclesList;

    // Actual performance measurements repeated REP times.
    // We simply store all results and compute medians during post-processing.
    double total_cycles = 0;
    for (size_t j = 0; j < REP; j++) {

        start = start_tsc();
        for (size_t i = 0; i < num_runs; ++i) {
            f(A,x,y);           
        }
        end = stop_tsc(start);

        cycles = ((double)end) / num_runs;
        total_cycles += cycles;

        cyclesList.push_back(cycles);
    }
    total_cycles /= REP;

    destroy(A);
    destroy(x);
    destroy(y);
    cyclesList.sort();
    cycles = total_cycles;//cyclesList.front();
    return  round((100.0 * flops) / cycles) / 100.0;
}

int main(int argc, char **argv)
{
  cout << "Starting program. ";
  double perf;
  int i;

  register_functions();

  if (numFuncs == 0){
    cout << endl;
    cout << "No functions registered - nothing for driver to do" << endl;
    cout << "Register functions by calling register_func(f, name)" << endl;
    cout << "in register_funcs()" << endl;

    return 0;
  }
  cout << numFuncs << " functions registered." << endl;
   
    //Check validity of functions. 
  int n = NR;
  double *A, *x, *y, *y_old, *y_base;
  build(&A, n, n);
  build(&x, 1, n);
  build(&y, 1, n);
  y_base = static_cast<double *>(malloc(n * sizeof(double)));
  y_old  = static_cast<double *>(malloc(n * sizeof(double)));
  
  memcpy(y_old,  y, n*sizeof(double));
  kernel_base(A, x, y);
  memcpy(y_base, y, n*sizeof(double));

  for (i = 0; i < numFuncs; i++) {
    memcpy(y, y_old, n*sizeof(double));
    comp_func f = userFuncs[i];
    f(A, x, y);
    double error = nrm_sqr_diff(y, y_base, n);
    
    if (error > EPS) {
      cout << error << endl;
      cout << "ERROR!!!!  the results for the " << i+1 << "th function are different to the previous" << std::endl;
      // return 0;
    }
  }
  destroy(A);
  destroy(x);
  destroy(y);
  destroy(y_base);


  for (i = 0; i < numFuncs; i++)
  {
    perf = perf_test(userFuncs[i], funcNames[i], 3*NR*NR+NR);
    cout << endl << "Running: " << funcNames[i] << endl;
    cout << perf << " flops / cycles" << endl;
  }

  return 0;
}
