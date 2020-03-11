#include <math.h>

static double sqrtsd (double x) {
    double r;
    __asm__ ("sqrtsd %1, %0" : "=x" (r) : "x" (x));
    return r;
}

static double sigmoid1(double x) {
  return x / sqrtsd(1.0 + x*x);
}

static double sigmoid2(double x) {
  return M_2_PI * atan(M_PI_2 * x);
}
