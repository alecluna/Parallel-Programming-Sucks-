#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "get_time.c"

void ms(int a[], int b[], int p, int r, int a_to_b)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        ms(a, b, p, q, !a_to_b);
        ms(a, b, q + 1, r, !a_to_b);
        if (a_to_b)
            pmerge(a, p, q, q + 1, r, b, p);
        else
            pmerge(b, p, q, q + 1, r, a, p);
    }
}
void mergesort140(int src[], int dst[], int n)
{
    memcpy(dst, src, n * sizeof(int));
    ms(src, dst, 0, n - 1, 1);
}