#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "hw6_A.h"
#include "hw6_B.h"
#include "hw6_C.h"
#include "get_time.c"
#include <stdint.h>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

uint64_t readTSCp()
{
    unsigned dummy;
    return __rdtscp(&dummy);
}

#define SMALL 32
#define SIZE 1000

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
    int right = SIZE;
    int left = 0;
    int s = 0;
    uint64_t start, end;

    if (SIZE < 0)
    {
        printf("Error: %d < 0 \n", SIZE);
        return 1;
    }

    // Random array initialization
    srand(20);
    for (int i = SIZE - 1; i >= 0; i--)
    {
        A[i] = i;
        printf("A[%d] = %d\n", i, A[i]);
    }

    printf("\n Serial pmerge algorithm: \n");

    start = readTSCp();
    printf("Start = %llu\n", start);
    pMergeSortSerial(A, left, right, B, s);
    end = readTSCp();
    printf("End = %llu\n", end);
    printf("Elapsed = %llu\n", readTSCp() - start);

    // for (int i = 0; i < SIZE; i++)
    // {
    //     printf("\n B[%d] = %d", i, B[i]);
    // }

    // for (int i = SIZE - 1; i >= 0; i--)
    // {
    //     A[i] = i;
    //     printf("A[%d] = %d\n", i, A[i]);
    // }

    printf("\n pmerge algorithm: \n");

    start = readTSCp();
    printf("Start = %llu\n", start);
    pMergeSort(A, left, right, B, s);
    end = readTSCp();
    printf("End = %llu\n", end);
    printf("Elapsed = %llu\n", readTSCp() - start);

    // for (int i = 0; i < SIZE; i++)
    // {
    //     printf("\n B[%d] = %d", i, B[i]);
    // }

    printf("\n Merge Sort Algorithm: \n");

    start = readTSCp();
    printf("Start = %llu\n", start);
    mergesort140(A, B, SIZE);
    end = readTSCp();
    printf("End = %llu\n", end);
    printf("Elapsed = %llu\n", readTSCp() - start);

    return 0;
}