#include "common.h"
#include <immintrin.h>

#define NR 32

void kernel_fast(double* A, double *x, double *y) {
  __m256d
    A0,
    A1,
    A2,
    A3,
    x_vec, 
    y_vec, 
    t0,
    t1,
    t2,
    t3,
    v0,
    v1,
    v2,
    v3,
    zeros, 
    ones,
    mask,
    ind, 
    inc;
  
  zeros = _mm256_setzero_pd();
  ones = _mm256_set1_pd(1.0);
  inc = _mm256_set1_pd(4);
  
  double temp[NR];
  ind = _mm256_set_pd(5, 4, 3, 2);
  for (int i = 0; i < NR; i+=4) {
    x_vec = _mm256_load_pd(x+i);
    x_vec = _mm256_mul_pd(ind, x_vec);
    _mm256_store_pd(temp+i, x_vec);
    ind = _mm256_add_pd(ind, inc);
  }

  for (int i = 0; i < NR; i+=4) {
    t0 = _mm256_setzero_pd();
    t1 = _mm256_setzero_pd();
    t2 = _mm256_setzero_pd();
    t3 = _mm256_setzero_pd();
    
    for (int j = 0; j < NR; j+=4) {
      x_vec = _mm256_load_pd(temp + j);
      
      A0 = _mm256_load_pd(A + i*NR + j);
      A1 = _mm256_load_pd(A + (i+1)*NR + j);
      A2 = _mm256_load_pd(A + (i+2)*NR + j);
      A3 = _mm256_load_pd(A + (i+3)*NR + j);

      t0 = _mm256_fmadd_pd(x_vec, A0, t0);
      t1 = _mm256_fmadd_pd(x_vec, A1, t1);
      t2 = _mm256_fmadd_pd(x_vec, A2, t2);
      t3 = _mm256_fmadd_pd(x_vec, A3, t3);
    }

    v0 = _mm256_unpacklo_pd(t0, t2);
    v1 = _mm256_unpackhi_pd(t0, t2);

    v2 = _mm256_unpacklo_pd(t1, t3);
    v3 = _mm256_unpackhi_pd(t1, t3);

    v0 = _mm256_add_pd(v0,v1);
    v1 = _mm256_add_pd(v2,v3);

    v0 = _mm256_permute4x64_pd(v0, 216);
    v1 = _mm256_permute4x64_pd(v1, 216);
      
    v2 = _mm256_unpacklo_pd(v0, v1);
    v3 = _mm256_unpackhi_pd(v0, v1);

    v0 = _mm256_add_pd(v2,v3);
    
    y_vec = _mm256_load_pd(y + i);
    y_vec = _mm256_add_pd(y_vec,v0);

    mask = _mm256_cmp_pd(y_vec, zeros, _CMP_GE_OQ);
    mask = _mm256_and_pd(mask, ones);
    y_vec = _mm256_add_pd(y_vec, mask);

    _mm256_store_pd(y+i, y_vec);
  }
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions()
{
  add_function(&kernel_fast, "Fast kernel",1);
  add_function(&kernel_base, "Base kernel",1);
}
