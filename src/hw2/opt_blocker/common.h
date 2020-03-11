#include <string>

typedef void(*comp_func)(double *, double *, double*, double *, int);

void add_function(comp_func f, std::string name, int flop);
double compute(double, double, double);
