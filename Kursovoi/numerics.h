#ifndef NUMERICS_H
#define NUMERICS_H

#include <stddef.h>
#include "funcs.h"

int root(func_t f, func_t g, double a, double b, double eps1, double *x_root, int *iters);

int integral(func_t f, double a, double b, double eps2, double *result, int *iters);

#endif 
