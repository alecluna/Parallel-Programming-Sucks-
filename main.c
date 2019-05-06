#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void pmerge(int t[], int p1, int r1, int p2, int r2, int a[], int p3);

#define SZ 1000000

static void fill(int *p, int n, int start)
{
    for (int i = 0; i < n; i++)
        p[i] = start + i * 2;
}

static int check(int *p, int n, int start)
{
    for (int i = 0; i < n; i++)
        if (p[i] != start + i)
            return 0;
    return 1;
}

int main()
{
    int *src = malloc(SZ * sizeof(int));
    int *dst = malloc(SZ * sizeof(int));

    clock_t min = 0x7fffffff;
    int good = 0;
    for (int i = 0; i < 3; i++)
    {
        fill(src, SZ / 2, 0);
        fill(src + SZ / 2, SZ / 2, 1);
        pmerge(src, 0, SZ / 2 - 1, SZ / 2, SZ - 1, dst, 0);
        good = check(dst, SZ, 0);
        clock_t t = clock();
        pmerge(src, 0, SZ / 2 - 1, SZ / 2, SZ - 1, dst, 0);
        pmerge(dst, 0, SZ / 2 - 1, SZ / 2, SZ - 1, src, 0);
        t = clock() - t;
        if (t < min)
            min = t;
    }
    printf("%d\n", good && check(src, SZ, 0));
    printf("%d\n", (int)min);
}
