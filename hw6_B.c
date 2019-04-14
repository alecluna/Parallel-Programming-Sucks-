#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "get_time.c"

#define SMALL 32
#define SIZE 10

extern double get_time(void);
void merge(int a[], int size, int temp[]);
int binarySearch(int x, int arr[], int p, int end);
void insertion_sort(int a[], int size);
void mergesort_parallel_omp(int a[], int size, int temp[], int threadCount);
void run_omp(int a[], int size, int temp[], int threadCount);

void pmerge(int t[], int left1, int right1, int left2, int right2, int a[], int p3);
void pMergeSort(int A[], int left, int right, int B[], int s);

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    int threadCount = 2;

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
    srand(20);
    for (int i = SIZE - 1; i >= 0; i--)
    {
        A[i] = i;
        printf("A[%d] = %d\n", i, A[i]);
    }
    printf("\n");

    int right = SIZE;
    int left = 0;
    int s = 0;

    // Sort
    //double start = get_time();
    pMergeSort(A, left, right, B, s);
    //double end = get_time();

    // printf("Start = %.2f\n", start);

    // printf("End = %.2f\n", end);

    // printf("Elapsed = %.2f\n", end - start);

    // // Result check
    // for (int i = 1; i < size; i++)
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

// A is our actual array and B is our final destination for sorting
void pMergeSort(int A[], int p, int r, int B[], int s)
{
    printf("Left: %d\n", p);
    printf("Right %d\n", r);

    int sizeOfPartition = r - p + 1;
    printf("Size of partition: %d\n", sizeOfPartition);

    if (sizeOfPartition == 1)
    {
        B[s] = A[p];
    }
    else
    {

        int T[sizeOfPartition];

        int sizeOfT = (int)sizeof(T) / (int)sizeof(T[0]);
        printf("Size of T: %d\n", sizeOfT);

        int q = floor((p + r) / 2);
        int qPrime = q - p + 1;

        printf("q = %d\n", q);
        printf("qPrime: %d\n", qPrime);

        // q is right of this partition
        // #pragma omp parallel sections
        //         {

        // #pragma omp section
        //             {
        //                 printf("Thread %d begins recursive call\n", omp_get_thread_num());
        pMergeSort(A, p, q, T, 0);
        //             }
        // #pragma omp section
        //             {
        //printf("Thread %d begins recursive call\n", omp_get_thread_num());
        pMergeSort(A, q + 1, r, T, qPrime + 1);
        //     }
        //     // //sync
        // }
        printf("\n");
        pmerge(T, 0, qPrime, qPrime + 1, sizeOfPartition, B, s);

        for (int i = 0; i < SIZE; i++)
        {
            printf("\n B[%d] = %d", i, B[i]);
        }
    }
}

//pmerge (T, p1, r1, p2, r2, A, p3)
void pmerge(int t[], int left1, int right1, int left2, int right2, int a[], int p3)
{
    int n1 = right1 - left1 + 1;
    int n2 = right2 - left2 + 1;

    if (n1 < n2)
    {
        //exchange left1 and left2
        int temp;
        temp = left1;
        left1 = left2;
        left2 = temp;

        //exchange right1 and right2
        temp = right1;
        right1 = right2;
        right2 = temp;

        //exchange n1 and n2
        temp = n1;
        n1 = n2;
        n2 = temp;
    }

    //     //n1 empty, sorting algo exits
    if (n1 == 0)
    {
        return;
    }
    else
    {
        /*Letting x = T[q1] be the median of
    T[p1...r1] and q2 be the place in T[p2...r2]
    such that x would fall between T Œq2 􏰀 1 and T Œq2,
    //*/
        int q1 = (left1 + right1) / 2;

        int q2 = binarySearch(t[q1], t, left2, right2);

        int q3 = p3 + (q1 - left1) + (q2 - left2);
        a[q3] = t[q1];

        //TODO: fix this seg fault
        pmerge(t, left1, q1 - 1, left2, q2 - 1, a, p3);
        pmerge(t, q1 + 1, right1, q2, right2, a, q3 + 1);
    }
}

// needed for p merge
int binarySearch(int x, int arr[], int p, int r)
{
    int low = p;
    int high;
    if (p > r + 1)
    {
        high = p;
    }
    else
    {
        high = r + 1;
    }

    while (low < high)
    {
        int mid = (high + low) / 2;
        if (x <= arr[mid])
        {
            high = mid;
        }
        else
        {
            low = mid + 1;
        }
    }
    return high;
}