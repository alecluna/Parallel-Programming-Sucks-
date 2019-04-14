#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "get_time.c"
#include "hw6_A.c"
#include "hw6_B.c"
#include "hw6_C.c"

#define SMALL 32
#define SIZE 10

extern double get_time(void);

int main(int argc, char *argv[])
{

    int threadCount = 8;

    // Check processorCount and threadCount
    int processorCount = omp_get_num_procs();
    printf("Size of Array = %d\n", SIZE);
    printf("Number of theads (processes) = % d\n", threadCount);
    printf("Number of Processors = %d\n", processorCount);

    if (processorCount < threadCount)
    {
        printf("Warning: %d threads requested, will run_omp on %d Processors available\n",
               threadCount, processorCount);

        //set number of threads for omp
        omp_set_num_threads(threadCount);
    }

    int maxThreads = omp_get_max_threads(); // Max available threadCount
    if (maxThreads < threadCount)           // Requested threadCount are more than max available
    {
        printf("Error: Cannot use %d threads, only %d threads available\n", threadCount, maxThreads);
        return 1;
    }

    int A[SIZE];
    int B[SIZE];

    if (SIZE < 0)
    {
        printf("Error: %d < 0 \n", SIZE);
        return 1;
    }

    // Random array initialization
    srand(100);
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = rand() % SIZE;
    }

    int aSize = (int)sizeof(A) / (int)sizeof(A[0]);
    int bSize = (int)sizeof(B) / (int)sizeof(B[0]);

    printf("A[]: %d\n B[]: %d\n", aSize, bSize);

    int right = aSize;
    int left = 0;
    int s = 0;

    for (int i = 0; i <= SIZE; i++)
    {
        printf("%d,", B[i]);
    }
    // Sort
    double start = get_time();
    //run_omp(a, size, temp, threadCount);
    double end = get_time();

    printf("Start = %.2f\n", start);

    printf("End = %.2f\n", end);

    printf("Elapsed = %.2f\n", end - start);

    // Result check
    // for (int i = 1; i < SIZE; i++)
    // {
    //     if (!(a[i - 1] <= a[i]))
    //     {
    //         printf("Implementation error: a[%d]=%d > a[%d]=%d\n", i - 1,
    //                a[i - 1], i, a[i]);
    //         return 1;
    //     }
    // }
    // puts("-Success-");
    return 0;
}