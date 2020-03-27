#include "common.h"
#include <immintrin.h>
#include <iostream>

using namespace std;
#define NR 8   // Size of one column vector of A and C
#define MR 75  // Number of column vectors to apply WHT

/* Matrix A and C are stored in colum-major order.
   Thus, the first column of A has the elements A[0]..A[7].
   Further, A and C are of size NR * MR*/
void wht_fast(double* A, double* C) {
  __m256d v1,v2,v3,v4;
  for (int i = 0; i < MR; i++) {
    v1 = _mm256_load_pd(A + (NR*i));
    v2 = _mm256_load_pd(A + (NR*i+4));
    
    v3 = _mm256_add_pd(v1,v2);
    v4 = _mm256_sub_pd(v1,v2);
    
    v1 = _mm256_insertf128_pd(v3,_mm256_castpd256_pd128(v4),1);
    v3 = _mm256_permute4x64_pd(v3, 78);
    v2 = _mm256_blend_pd(v3,v4, 12);

    v3 = _mm256_add_pd(v1,v2);
    v4 = _mm256_sub_pd(v1,v2);

    v1 = _mm256_unpacklo_pd(v3,v4);
    v2 = _mm256_unpackhi_pd(v3,v4);
    
    v3 = _mm256_add_pd(v1,v2);
    v4 = _mm256_sub_pd(v1,v2);

    v3 = _mm256_permute4x64_pd(v3, 216);
    v4 = _mm256_permute4x64_pd(v4, 216);

    v1 = _mm256_unpacklo_pd(v3,v4);
    v2 = _mm256_unpackhi_pd(v3,v4);
    
    _mm256_store_pd(C + (NR*i), v1);
    _mm256_store_pd(C + (NR*i + 4), v2);
  }
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions()
{
  add_function(&wht_fast, "fast WHT",1);
}
