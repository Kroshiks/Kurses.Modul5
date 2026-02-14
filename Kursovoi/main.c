#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

#include "funcs.h"
#include "numerics.h"

/* Точность для площади (). eps1 и eps2 берём как доли eps для гарантии */
static double g_eps  = 1e-4;
static double g_eps1 = 1e-6;
static double g_eps2 = 1e-6;

/* Флаги командной строки */
static int g_print_points = 0;
static int g_print_iters  = 0;
static int g_test_root    = 0;
static int g_test_integral= 0;

static void print_help()
{
    printf("Опции:\n");
    printf("  -h           Показать эту справку\n");
    printf("  -p           Вывести координаты x точек пересечения\n");
    printf("  -i           Вывести количество итераций при поиске корней (и уточнении интеграла)\n");
    printf("  -eps 	       <значение>  Задать точность площади (по умолчанию: 1e-4)\n");
    printf("  -tr          Запустить тесты функции root()\n");
    printf("  -ti          Запустить тесты функции integral()\n");
    printf("  -t           Запустить все тесты\n");
}

static int parse_double(const char *s, double *out)
{
    if (!s || !out)
		return 0;
		
    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s || *end != '\0')
		return 0;
		
    *out = v;
    return 1;
}

static int test_root_func(void)
{
    double fx(double x) {return x;}
		
    double gz(double x) {(void)x; return 0.0;} 

    double r = 0.0;
    int it = 0;
    int rc = root(fx, gz, -1.0, 1.0, 1e-8, &r, &it);
    
    if (rc != 0)
		return 0;
		
    if (fabs(r - 0.0) > 1e-6)
		return 0;

    return 1;
}

static int test_integral_func(void)
{
    double c2(double x) { (void)x; return 2.0; }

    double I = 0.0;
    int it = 0;
    int rc = integral(c2, 0.0, 2.0, 1e-8, &I, &it);
    if (rc != 0)
		return 0;
		
    if (fabs(I - 4.0) > 1e-6)
		return 0;

    return 1;
}

static double g_x13 = 0.0, g_x23 = 0.0, g_x12 = 0.0;
static int g_it13 = 0, g_it23 = 0, g_it12 = 0;

static double f1_minus_f3(double x) { return f1(x) - f3(x); }
static double f1_minus_f2(double x) { return f1(x) - f2(x); }

static int solve_intersections(void)
{
    int rc;

    rc = root(f1, f3, 0.5, 1.5, g_eps1, &g_x13, &g_it13);
    if (rc != 0)
		return 0;

    rc = root(f2, f3, 3.0, 3.5, g_eps1, &g_x23, &g_it23);
    if (rc != 0)
		return 0;

    rc = root(f1, f2, 3.5, 4.5, g_eps1, &g_x12, &g_it12);
    if (rc != 0)
		return 0;

    return 1;
}

static int solve_area(double *S, int *int_iters_total)
{
    double I1 = 0.0, I2 = 0.0;
    int it1 = 0, it2 = 0;

    double eps2_each = g_eps2 * 0.5;

    if (integral(f1_minus_f3, g_x13, g_x23, eps2_each, &I1, &it1) != 0)
		return 0;
		
    if (integral(f1_minus_f2, g_x23, g_x12, eps2_each, &I2, &it2) != 0)
		return 0;

    *S = I1 + I2;
    if (int_iters_total)
		*int_iters_total = it1 + it2;
		
    return 1;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Rus");

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            print_help();
            return 0;
        }
        else if (strcmp(argv[i], "-p") == 0) 
            g_print_points = 1;
        else if (strcmp(argv[i], "-i") == 0) 
            g_print_iters = 1;
        else if (strcmp(argv[i], "-eps") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Ошибка: -eps требует указания значения\n");
                return 2;
            }
            double v = 0.0;
            if (!parse_double(argv[i + 1], &v) || v <= 0.0)
            {
                fprintf(stderr, "Ошибка: недопустимое значение -eps\n");
                return 2;
            }
            g_eps = v;
            i++;
        }
        else if (strcmp(argv[i], "-tr") == 0) 
            g_test_root = 1;
        else if (strcmp(argv[i], "-ti") == 0) 
            g_test_integral = 1;
        else if (strcmp(argv[i], "-t") == 0)
        {
            g_test_root = 1;
            g_test_integral = 1;
        }
        else
        {
            fprintf(stderr, "Неизвестная опция: %s\n", argv[i]);
            fprintf(stderr, "Используйте -h\n");
            return 2;
        }
    }

    g_eps1 = g_eps * 1e-2; /* точнее для корней */
    g_eps2 = g_eps * 1e-2; /* точнее для интеграла */
    if (g_eps1 < 1e-12)
		g_eps1 = 1e-12;
		
    if (g_eps2 < 1e-12)
		g_eps2 = 1e-12;

    /* Тесты */
    if (g_test_root || g_test_integral)
    {
        int ok = 1;
        if (g_test_root)
        {
            int r = test_root_func();
            printf("[тест-root] %s\n", r ? "OK" : "ОШИБКА");
            ok = ok && r;
        }
        if (g_test_integral)
        {
            int r = test_integral_func();
            printf("[тест-integral] %s\n", r ? "OK" : "ОШИБКА");
            ok = ok && r;
        }
        return ok ? 0 : 1;
    }

    if (!solve_intersections())
    {
        fprintf(stderr, "Ошибка: не удалось найти точки пересечения (проверьте интервалы)\n");
        return 1;
    }

    if (g_print_points)
    {
        printf("Точки пересечения x:\n");
        printf("  x13 (f1=f3) = %.12f\n", g_x13);
        printf("  x23 (f2=f3) = %.12f\n", g_x23);
        printf("  x12 (f1=f2) = %.12f\n", g_x12);
    }

    if (g_print_iters)
    {
        printf("Итерации поиска корней:\n");
        printf("  it13 (f1=f3) = %d\n", g_it13);
        printf("  it23 (f2=f3) = %d\n", g_it23);
        printf("  it12 (f1=f2) = %d\n", g_it12);
    }

    double S = 0.0;
    int int_iters = 0;
    if (!solve_area(&S, g_print_iters ? &int_iters : NULL)) {
        fprintf(stderr, "Ошибка: не удалось вычислить площадь\n");
        return 1;
    }

    printf("Площадь S = %.10f (eps=%.3e)\n", S, g_eps);

    if (g_print_iters) {
        printf("Итераций уточнения интеграла (всего) = %d\n", int_iters);
    }

    return 0;
}
