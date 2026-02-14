#include "numerics.h"
#include <math.h>

static double diff_fg(func_t f, func_t g, double x) {
    return f(x) - g(x);
}

/* Метод деления пополам */
int root(func_t f, func_t g, double a, double b, double eps1,
         double *x_root, int *iters)
{
    if (!x_root || eps1 <= 0.0)
		return 2;

    double fa = diff_fg(f, g, a);
    double fb = diff_fg(f, g, b);

    if (fabs(fa) < eps1)
    {
        *x_root = a;
        if (iters) *iters = 0;
        return 0;
    }
    
    if (fabs(fb) < eps1)
    {
        *x_root = b;
        if (iters) *iters = 0;
        return 0;
    }

    if (fa * fb > 0.0)
        return 1;
        
    int k = 0;
    double left = a, right = b;

    while (fabs(right - left) > eps1)
    {
        double mid = 0.5 * (left + right);
        double fm = diff_fg(f, g, mid);

        k++;

        if (fabs(fm) < eps1)
        {
            left = right = mid;
            break;
        }

        double fl = diff_fg(f, g, left);
        if (fl * fm <= 0.0)
           right = mid;
       else
           left = mid;
        
    }

    *x_root = 0.5 * (left + right);
    if (iters) *iters = k;
    return 0;
}

static double simpson_composite(func_t f, double a, double b, size_t n)
{
    const double h = (b - a) / (double)n;
    double s = f(a) + f(b);

    for (size_t i = 1; i < n; i += 2)
    {
        double x = a + h * (double)i;
        s += 4.0 * f(x);
    }

    for (size_t i = 2; i < n; i += 2)
    {
        double x = a + h * (double)i;
        s += 2.0 * f(x);
    }

    return (h / 3.0) * s;
}

int integral(func_t f, double a, double b, double eps2, double *result, int *iters)
{
    if (!result || eps2 <= 0.0) return 2;
    if (a == b)
    {
        *result = 0.0;
        if (iters) *iters = 0;
        return 0;
    }

    double sign = 1.0;
    if (b < a) { double t = a; a = b; b = t; sign = -1.0; }

    size_t n = 2; 
    double I1 = simpson_composite(f, a, b, n);
    int k = 0;

    while (1)
    {
        n *= 2;
        double I2 = simpson_composite(f, a, b, n);
        k++;

        double err = fabs(I2 - I1) / 15.0;

        if (err < eps2)
        {
            *result = sign * I2;
            if (iters) *iters = k;
            return 0;
        }

        if (n > (1u << 26)) 
            return 1;
        
        I1 = I2;
    }
}
