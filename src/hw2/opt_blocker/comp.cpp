#include <cmath>
#include "common.h"

//
//The slow base version
//
void slowperformance1(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ( (i + j) % 2) {
                z[i] += 1.0 / (x[i*n + j] * sqrt(w[i]));
            }
            else {
                z[i] = compute(w[i], y[i*n + j], z[i]);
            }
            z[i] *= x[i*n + j];
        }
    }
}

//
// inline compute()
//
void inline_compute(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ( (i + j) % 2) {
                z[i] += 1.0 / (x[i*n + j] * sqrt(w[i]));
            }
            else {
                z[i] = w[i]*w[i]/2.0 + y[i*n + j]/5.0 + z[i];
            }
            z[i] *= x[i*n + j];
        }
    }
}

//
// strength reduction (assume no aliasing) & inlining
// 
// moving the indicies out doesn't improve performance
//
void strength_red(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i++) {
        double inv_sqrt_w = 1.0 / sqrt(w[i]);
        double w_pow = w[i]*w[i]*0.5;
        
        for (int j = 0; j < n; j++) {
            if ( (i + j) % 2) {
                z[i] = z[i] * x[n*i + j] + inv_sqrt_w;
            }
            else {
                z[i] = (w_pow + y[n*i + j]*0.2 + z[i]) * x[n*i + j];
            }
        }
        
    }
}

//
// remove aliasing (including prev. optimizations)
//
void remove_aliasing(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i++) {
        double val_w = w[i];
        double inv_sqrt_w = 1.0 / sqrt(val_w);
        double w_pow = val_w*val_w*0.5;
        
        double val_z = z[i];
        for (int j = 0; j < n; j++) {
            if ( (i + j) % 2) {
                val_z = val_z * x[n*i + j] + inv_sqrt_w;
            }
            else {
                val_z = (w_pow + y[n*i + j]*0.2 + val_z) * x[n*i + j];
            }
        }
        z[i] = val_z;
    }
}

//
// loop unrolling
//
void loop_unrolling(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i+=2) {
        double val_w_1 = w[i];
        double inv_sqrt_w_1 = 1.0 / sqrt(val_w_1);
        double w_pow_1 = val_w_1*val_w_1*0.5;
        
        double val_w_2 = w[i+1];
        double inv_sqrt_w_2 = 1.0 / sqrt(val_w_2);
        double w_pow_2 = val_w_2*val_w_2*0.5;
        
        //printf("%d: ", i);
        double val_z_1 = z[i];
        double val_z_2 = z[i+1];
        
        for (int j = 0; j < n; j+=2) {
            //printf("z_2(%d) ", n*i + j);
            val_z_1 = (w_pow_1 + y[n*i + j]*0.2 + val_z_1) * x[n*i + j];
            
            //printf("z_1(%d) ", n*(i+1) + j);
            val_z_2 = val_z_2 * x[n*(i+1) + j] + inv_sqrt_w_2;
    
            //printf("z_1(%d) ", n*i + j + 1);
            val_z_1 = val_z_1 * x[n*i + j + 1] + inv_sqrt_w_1;
            
            //printf("z_2(%d) ", n*(i+1) + j + 1);
            val_z_2 = (w_pow_2 + y[n*(i+1) + j + 1]*0.2 + val_z_2) * x[n*(i+1) + j + 1];
        }
        
        z[i] = val_z_1;
        z[i+1] = val_z_2;
        //printf("\n");
    }
}

//
// more loop unrolling
//
void more_loop_unrolling(double* w, double *x, double *y, double *z, int n) {
    for(int i = 0; i < n; i+=8) {
        double val_w_1 = w[i];
        double inv_sqrt_w_1 = 1.0 / sqrt(val_w_1);
        double w_pow_1 = val_w_1*val_w_1*0.5;
        
        double val_w_2 = w[i+1];
        double inv_sqrt_w_2 = 1.0 / sqrt(val_w_2);
        double w_pow_2 = val_w_2*val_w_2*0.5;
        
        double val_w_3 = w[i+2];
        double inv_sqrt_w_3 = 1.0 / sqrt(val_w_3);
        double w_pow_3 = val_w_3*val_w_3*0.5;
        
        double val_w_4 = w[i+3];
        double inv_sqrt_w_4 = 1.0 / sqrt(val_w_4);
        double w_pow_4 = val_w_4*val_w_4*0.5;
        
        double val_w_5 = w[i+4];
        double inv_sqrt_w_5 = 1.0 / sqrt(val_w_5);
        double w_pow_5 = val_w_5*val_w_5*0.5;
        
        double val_w_6 = w[i+5];
        double inv_sqrt_w_6 = 1.0 / sqrt(val_w_6);
        double w_pow_6 = val_w_6*val_w_6*0.5;
        
        double val_w_7 = w[i+6];
        double inv_sqrt_w_7 = 1.0 / sqrt(val_w_7);
        double w_pow_7 = val_w_7*val_w_7*0.5;
        
        double val_w_8 = w[i+7];
        double inv_sqrt_w_8 = 1.0 / sqrt(val_w_8);
        double w_pow_8 = val_w_8*val_w_8*0.5;
        
        double val_z_1 = z[i];
        double val_z_2 = z[i+1];
        double val_z_3 = z[i+2];
        double val_z_4 = z[i+3];
        double val_z_5 = z[i+4];
        double val_z_6 = z[i+5];
        double val_z_7 = z[i+6];
        double val_z_8 = z[i+7];
        
        for (int j = 0; j < n; j += 2) {
            val_z_1 = (w_pow_1 + y[n*i + j]*0.2 + val_z_1) * x[n*i + j];
            val_z_2 = val_z_2 * x[n*(i+1) + j] + inv_sqrt_w_2;
            val_z_3 = (w_pow_3 + y[n*(i+2) + j]*0.2 + val_z_3) * x[n*(i+2) + j];
            val_z_4 = val_z_4 * x[n*(i+3) + j] + inv_sqrt_w_4;
            val_z_5 = (w_pow_5 + y[n*(i+4) + j]*0.2 + val_z_5) * x[n*(i+4) + j];
            val_z_6 = val_z_6 * x[n*(i+5) + j] + inv_sqrt_w_6; 
            val_z_7 = (w_pow_7 + y[n*(i+6) + j]*0.2 + val_z_7) * x[n*(i+6) + j];
            val_z_8 = val_z_8 * x[n*(i+7) + j] + inv_sqrt_w_8; 
            val_z_1 = val_z_1 * x[n*i + j + 1] + inv_sqrt_w_1;
            val_z_2 = (w_pow_2 + y[n*(i+1) + j + 1]*0.2 + val_z_2) * x[n*(i+1) + j + 1];
            val_z_3 = val_z_3 * x[n*(i+2) + j + 1] + inv_sqrt_w_3;
            val_z_4 = (w_pow_4 + y[n*(i+3) + j + 1]*0.2 + val_z_4) * x[n*(i+3) + j + 1];
            val_z_5 = val_z_5 * x[n*(i+4) + j + 1] + inv_sqrt_w_5;
            val_z_6 = (w_pow_6 + y[n*(i+5) + j + 1]*0.2 + val_z_6) * x[n*(i+5) + j + 1];
            val_z_7 = val_z_7 * x[n*(i+6) + j + 1] + inv_sqrt_w_7;
            val_z_8 = (w_pow_8 + y[n*(i+7) + j + 1]*0.2 + val_z_8) * x[n*(i+7) + j + 1];
        }
        
        z[i] = val_z_1;
        z[i+1] = val_z_2;
        z[i+2] = val_z_3;
        z[i+3] = val_z_4;
        z[i+4] = val_z_5;
        z[i+5] = val_z_6;
        z[i+6] = val_z_7;
        z[i+7] = val_z_8;
    }
}

void maxperformance(double* w, double *x, double *y, double *z, int n) {
  /* Replace this with your super optimized code */
      for(int i = 0; i < n; i+=8) {
        double val_w_1 = w[i];
        double inv_sqrt_w_1 = 1.0 / sqrt(val_w_1);
        double w_pow_1 = val_w_1*val_w_1*0.5;
        
        double val_w_2 = w[i+1];
        double inv_sqrt_w_2 = 1.0 / sqrt(val_w_2);
        double w_pow_2 = val_w_2*val_w_2*0.5;
        
        double val_w_3 = w[i+2];
        double inv_sqrt_w_3 = 1.0 / sqrt(val_w_3);
        double w_pow_3 = val_w_3*val_w_3*0.5;
        
        double val_w_4 = w[i+3];
        double inv_sqrt_w_4 = 1.0 / sqrt(val_w_4);
        double w_pow_4 = val_w_4*val_w_4*0.5;
        
        double val_w_5 = w[i+4];
        double inv_sqrt_w_5 = 1.0 / sqrt(val_w_5);
        double w_pow_5 = val_w_5*val_w_5*0.5;
        
        double val_w_6 = w[i+5];
        double inv_sqrt_w_6 = 1.0 / sqrt(val_w_6);
        double w_pow_6 = val_w_6*val_w_6*0.5;
        
        double val_w_7 = w[i+6];
        double inv_sqrt_w_7 = 1.0 / sqrt(val_w_7);
        double w_pow_7 = val_w_7*val_w_7*0.5;
        
        double val_w_8 = w[i+7];
        double inv_sqrt_w_8 = 1.0 / sqrt(val_w_8);
        double w_pow_8 = val_w_8*val_w_8*0.5;
        
        double val_z_1 = z[i];
        double val_z_2 = z[i+1];
        double val_z_3 = z[i+2];
        double val_z_4 = z[i+3];
        double val_z_5 = z[i+4];
        double val_z_6 = z[i+5];
        double val_z_7 = z[i+6];
        double val_z_8 = z[i+7];
        
        for (int j = 0; j < n; j += 2) {
            val_z_1 = (w_pow_1 + y[n*i + j]*0.2 + val_z_1) * x[n*i + j];
            val_z_2 = val_z_2 * x[n*(i+1) + j] + inv_sqrt_w_2;
            val_z_3 = (w_pow_3 + y[n*(i+2) + j]*0.2 + val_z_3) * x[n*(i+2) + j];
            val_z_4 = val_z_4 * x[n*(i+3) + j] + inv_sqrt_w_4;
            val_z_5 = (w_pow_5 + y[n*(i+4) + j]*0.2 + val_z_5) * x[n*(i+4) + j];
            val_z_6 = val_z_6 * x[n*(i+5) + j] + inv_sqrt_w_6; 
            val_z_7 = (w_pow_7 + y[n*(i+6) + j]*0.2 + val_z_7) * x[n*(i+6) + j];
            val_z_8 = val_z_8 * x[n*(i+7) + j] + inv_sqrt_w_8; 
            val_z_1 = val_z_1 * x[n*i + j + 1] + inv_sqrt_w_1;
            val_z_2 = (w_pow_2 + y[n*(i+1) + j + 1]*0.2 + val_z_2) * x[n*(i+1) + j + 1];
            val_z_3 = val_z_3 * x[n*(i+2) + j + 1] + inv_sqrt_w_3;
            val_z_4 = (w_pow_4 + y[n*(i+3) + j + 1]*0.2 + val_z_4) * x[n*(i+3) + j + 1];
            val_z_5 = val_z_5 * x[n*(i+4) + j + 1] + inv_sqrt_w_5;
            val_z_6 = (w_pow_6 + y[n*(i+5) + j + 1]*0.2 + val_z_6) * x[n*(i+5) + j + 1];
            val_z_7 = val_z_7 * x[n*(i+6) + j + 1] + inv_sqrt_w_7;
            val_z_8 = (w_pow_8 + y[n*(i+7) + j + 1]*0.2 + val_z_8) * x[n*(i+7) + j + 1];
        }
        
        z[i] = val_z_1;
        z[i+1] = val_z_2;
        z[i+2] = val_z_3;
        z[i+3] = val_z_4;
        z[i+4] = val_z_5;
        z[i+5] = val_z_6;
        z[i+6] = val_z_7;
        z[i+7] = val_z_8;
    }
}


/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions()
{
  add_function(&slowperformance1, "base line",1);
  add_function(&inline_compute, "inline compute()",1);
  add_function(&strength_red, "strength_red & inlining",1);
  add_function(&remove_aliasing, "remove aliasing",1);
  add_function(&loop_unrolling, "loop unrolling",1);
  add_function(&more_loop_unrolling, "more loop unrolling",1);
  add_function(&maxperformance, "Max Performance",1);
}
