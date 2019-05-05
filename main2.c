#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void mergesort140(int src[], int dst[], int n);

#define SZ 1000000

int compar(const void *a, const void *b)
{
    if (*(int *)a > *(int *)b)
        return 1;
    if (*(int *)a < *(int *)b)
        return -1;
    return 0;
}

int main()
{
    int *src = malloc(SZ * sizeof(int));
    int *dst = malloc(SZ * sizeof(int));
    int *dst2 = malloc(SZ * sizeof(int));
    srandom(3);
    clock_t min = 0x7fffffff;
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < SZ; i++)
            src[i] = dst[i] = random();
        clock_t t = clock();
        mergesort140(src, dst2, SZ);
        t = clock() - t;
        if (t < min)
            min = t;
    }
    qsort(dst, SZ, sizeof(int), compar);
    printf("%d\n", 0 == memcmp(dst, dst2, SZ * sizeof(int)));
    printf("%d\n", (int)min);
}