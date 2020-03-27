#include <string>

#define NR 8
#define MR 75

typedef void(*comp_func)(double *, double*);

void add_function(comp_func f, std::string name, int flop);
void wht_base(double* x, double* y);
