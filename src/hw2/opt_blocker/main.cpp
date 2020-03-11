/**
*      _________   _____________________  ____  ______
*     / ____/   | / ___/_  __/ ____/ __ \/ __ \/ ____/
*    / /_  / /| | \__ \ / / / /   / / / / / / / __/
*   / __/ / ___ |___/ // / / /___/ /_/ / /_/ / /___
*  /_/   /_/  |_/____//_/  \____/\____/_____/_____/
*
*  http://www.inf.ethz.ch/personal/markusp/teaching/
*  How to Write Fast Numerical Code 263-2300 - ETH Zurich
*  Copyright (C) 2017  Alen Stojanov      (astojanov@inf.ethz.ch)
*                      Georg Ofenbeck     (ofenbeck@inf.ethz.ch)
*                      Singh Gagandeep    (gsingh@inf.ethz.ch)
*                  Markus Pueschel    (pueschel@inf.ethz.ch)
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
#include<random>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "tsc_x86.h"
#include <cfloat>

/* prototype of the function you need to optimize */
typedef void(*comp_func)(double *, double *, double*, double *, int);

#define CYCLES_REQUIRED 1e7
#define REP 10
#define MAX_FUNCS 32
#define FLOPS (4.*n)
#define EPS (1e-3)

using namespace std;

double compute(double x, double y, double z);

//headers
double get_perf_score(comp_func f);
void register_functions();
double perf_test(comp_func f, string desc, int flops);
void add_function(comp_func f, string name, int flop);
void build_x(double ** m, unsigned n1, unsigned n2);

/* Global vars, used to keep track of student functions */
vector<comp_func> userFuncs;
vector<string> funcNames;
vector<int> funcFlops;
int numFuncs = 0;

void rands(double * m, size_t row, size_t col)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<double> dist(1.0, 5.0);
  for (size_t i = 0; i < row*col; ++i)  
    m[i] = dist(gen);
}

void build(double ** m, unsigned n)
{
  *m = static_cast<double *>(malloc(n * sizeof(double)));
  rands(*m, 1, n);
}


void rands_x(double * m, size_t row, size_t col)
{
  static double err = 0.00001;
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<double> dist(0, row);
  for (size_t i = 0; i < row*col; ++i) {
    if (dist(gen) > row/2) {
      m[i] = 1.0 + err;
    }
    else {
      m[i] = 1.0 - err;
    }
  }
  
  std::uniform_real_distribution<double> dist2(2.0, 5.0);
  for (size_t i = 0; i < col; ++i) {
    int index = (int) dist(gen);
    m[i*row + index] = dist2(gen);
  }
}

void build_x(double ** m, unsigned n1, unsigned n2)
{
  *m = static_cast<double *>(malloc(n1*n2 * sizeof(double)));
  rands_x(*m, n1, n2);
}


void destroy(double * m)
{
  free(m);
}

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
* Main driver routine - calls register_funcs to get student functions, then
* tests all functions registered, and reports the best performance
*/
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
  int n = 1000;
  double *w, *x, *y, *z, *z_old, *z_correct;
  build(&w, n);
  build_x(&x, n, n);
  build(&y, n*n);
  build(&z, n);
  
  z_old = static_cast<double *>(malloc(n * sizeof(double)));
  z_correct = static_cast<double *>(malloc(n * sizeof(double)));
  memcpy(z_old, z, n*sizeof(double));
  comp_func f = userFuncs[0];
  f(w, x, y, z, n);
  memcpy(z_correct, z, n*sizeof(double));

  for (i = 0; i < numFuncs; i++) {
    memcpy(z, z_old, n*sizeof(double));
    comp_func f = userFuncs[i];
    f(w, x, y, z, n);
    double error = nrm_sqr_diff(z, z_correct, n);
    
    if (error > EPS) {
      cout << error << endl;
      cout << "ERROR!!!!  the results for the " << i << "th function are different to the previous" << std::endl;
      return 0;
    }
  }
  destroy(w);
  destroy(x);
  destroy(y);
  destroy(z);


  for (i = 0; i < numFuncs; i++)
  {
    perf = perf_test(userFuncs[i], funcNames[i], funcFlops[i]);
    cout << endl << "Running: " << funcNames[i] << endl;
    cout << perf << " cycles" << endl;
  }

  return 0;
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
  double perf = 0.0;
  long num_runs = 16;
  double multiplier = 1;
  myInt64 start, end;

  double *w, *x, *y, *z;
  int n = 1000;

  build(&w, n);
  build_x(&x, n, n);
  build(&y, n*n);
  build(&z, n);

  // Warm-up phase: we determine a number of executions that allows
  // the code to be executed for at least CYCLES_REQUIRED cycles.
  // This helps excluding timing overhead when measuring small runtimes.
  do {
    num_runs = num_runs * multiplier;
    start = start_tsc();
    for (size_t i = 0; i < num_runs; i++) {
      f(w,x,y,z,n);      
    }
    end = stop_tsc(start);

    cycles = (double)end;
    multiplier = (CYCLES_REQUIRED) / (cycles);
    
  } while (multiplier > 2);

  list< double > cyclesList, perfList;

  // Actual performance measurements repeated REP times.
  // We simply store all results and compute medians during post-processing.
  for (size_t j = 0; j < REP; j++) {

    start = start_tsc();
    for (size_t i = 0; i < num_runs; ++i) {
      f(w,x,y,z,n);
    }
    end = stop_tsc(start);

    cycles = ((double)end) / num_runs;

    cyclesList.push_back(cycles);
    perfList.push_back(FLOPS / cycles);
  }

  destroy(w);
  destroy(x);
  destroy(y);
  destroy(z);
  cyclesList.sort();
  cycles = cyclesList.front();  
  return  cycles;
}

