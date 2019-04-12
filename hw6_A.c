#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#define SMALL 32

extern double get_time(void);
void merge(int a[], int size, int temp[]);
void insertion_sort(int a[], int size);
void mergesort_serial(int a[], int size, int temp[]);
void mergesort_parallel_omp(int a[], int size, int temp[], int threadCount);
void run_omp(int a[], int size, int temp[], int threadCount);

void pmerge(int t[], int left1, int right1, int left2, int right2, int a[], int p3);
void pMergeSort(int A[], int left, int right, int B[], int s);
int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    int size = 1000;
    //change later
    int threadCount = 2;

    // Check processorCount and threadCount

    int processorCount = omp_get_num_procs();
    printf("Size of Array = %d\n", size);
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

    // Array allocation
    // int *A = malloc(sizeof(int) * size);
    // int *B = malloc(sizeof(int) * size);
    int A[size];
    int B[size];

    if (A == NULL || B == NULL)
    {
        printf("Error with memory allocation %d\n", size);
        return 1;
    }

    // Random array initialization
    srand(100);
    for (int i = 0; i < size; i++)
    {
        A[i] = rand() % size;
    }

    int right = (int)sizeof(A) / (int)sizeof(A[0]);
    int left = 0;
    int s = 0;

    pMergeSort(A, left, right, B, s);

    //pmerge(a, 0)

    // // Sort
    // double start = get_time();
    // run_omp(a, size, temp, threadCount);
    // double end = get_time();

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

// Driver
void run_omp(int a[], int size, int temp[], int threadCount)
{
    // Parallel mergesort
    mergesort_parallel_omp(a, size, temp, threadCount);
}

// OpenMP merge sort with given number of threadCount
void mergesort_parallel_omp(int a[], int size, int temp[], int threadCount)
{
    if (threadCount == 1)
    {
        //  printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
        mergesort_serial(a, size, temp);
    }
    else if (threadCount > 1)
    {
#pragma omp parallel sections
        {
            printf("Thread %d begins recursive section\n", omp_get_thread_num());
#pragma omp section
            { //printf("Thread %d begins recursive call\n", omp_get_thread_num());
                mergesort_parallel_omp(a, size / 2, temp, threadCount / 2);
            }
#pragma omp section
            { //printf("Thread %d begins recursive call\n", omp_get_thread_num());
                mergesort_parallel_omp(a + size / 2, size - size / 2,
                                       temp + size / 2, threadCount - threadCount / 2);
            }
        }
        // Thread allocation is implementation dependent
        // Some threads can execute multiple sections while others are idle
        // Merge the two sorted sub-arrays through temp
        merge(a, size, temp);
    }
    else
    {
        printf("Error: %d threads\n", threadCount);
        return;
    }
}

void mergesort_serial(int a[], int size, int temp[])
{
    // Switch to insertion sort for small arrays
    if (size <= SMALL)
    {
        insertion_sort(a, size);
        return;
    }
    mergesort_serial(a, size / 2, temp);
    mergesort_serial(a + size / 2, size - size / 2, temp);
    // Merge the two sorted subarrays into a temp array
    merge(a, size, temp);
}

// A is our actual array and B is our final destination for sorting
void pMergeSort(int A[], int left, int right, int B[], int s)
{
    printf("Left: %d\n", left);
    printf("Right %d\n", right);

    int sizeOfPartition = right - left;
    printf("%d\n", sizeOfPartition);

    if (sizeOfPartition == 1)
    {
        B[s] = A[left];
    }
    else
    {
        int T[sizeOfPartition];
        int q = floor((left + right) / 2);
        int qPrime = q - left + 1;
        printf("q: %d\n", q);
        printf("qPrime: %d\n", qPrime);

        // spawn
        // pMergeSort(A, left, q, T, 1);
        // pMergeSort(A, q + 1, right, T, qPrime + 1);
        // //sync
        // pmerge(T, 1, qPrime, qPrime + 1, sizeOfPartition, B, s);
    }
}

//pmerge (T, p1, r1, p2, r2, A, p3)
void pmerge(int t[], int left1, int right1, int left2, int right2, int a[], int p3)
{
    //size of each partition1
    int n1 = right1 - left1 + 1;
    //size of partition2
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

    //n1 empty, sorting algo exits
    if (n1 == 0)
    {
        return;
    }
    else
    {
        /*Letting x = T[q1] be the median of 
        T[p1...r1] and q2 be the place in T[p2...r2] 
        such that x would fall between T Œq2 􏰀 1 and T Œq2,
        */
        int q1 = floor((left1 + right1) / 2);

        int q2 = binarySearch(t[q1], t, left2, right2);
        printf("Median of 1st partition: %d\n", q1);
        printf("Median of 2nd partition: %d\n", q2);

        // int q3 = p3 + (q1 - left1) + (q2 - left2);
        // a[q3] = t[q1];

        // pmerge(t, left1, q1 - 1, left2, q2 - 1, a, p3);
        // pmerge(t, q1 + 1, right1, q2, right2, a, q3 + 1);
    }
}

// needed for p merge
int binarySearch(int x, int arr[], int p, int end)
{
    int low = p;
    int high = end;

    while (low < high + 1)
    {
        int mid = (high + low) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] > x)
        {
            return binarySearch(x, arr, low, mid - 1);
        }
        else
        {
            return binarySearch(x, arr, mid + 1, high);
        }
    }
    return -1;
}

void merge(int a[], int size, int temp[])
{
    int i1 = 0;
    int i2 = size / 2;
    int tempIndex = 0;
    while (i1 < size / 2 && i2 < size)
    {
        if (a[i1] < a[i2])
        {
            temp[tempIndex] = a[i1];
            i1++;
        }
        else
        {
            temp[tempIndex] = a[i2];
            i2++;
        }
        tempIndex++;
    }
    while (i1 < size / 2)
    {
        temp[tempIndex] = a[i1];
        i1++;
        tempIndex++;
    }
    while (i2 < size)
    {
        temp[tempIndex] = a[i2];
        i2++;
        tempIndex++;
    }
    // Copy sorted temp array into main array, a
    memcpy(a, temp, size * sizeof(int));
}

void insertion_sort(int a[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        int j, v = a[i];
        for (j = i - 1; j >= 0; j--)
        {
            if (a[j] <= v)
                break;
            a[j + 1] = a[j];
        }
        a[j + 1] = v;
    }
}