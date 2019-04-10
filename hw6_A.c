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
int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    int size = 100;
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
    int *a = malloc(sizeof(int) * size);
    int *temp = malloc(sizeof(int) * size);

    if (a == NULL || temp == NULL)
    {
        printf("Error with memory allocation %d\n", size);
        return 1;
    }

    // Random array initialization
    srand(1000);
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % size;
    }

    // Sort
    double start = get_time();
    run_omp(a, size, temp, threadCount);
    double end = get_time();

    printf("Start = %.2f\n", start);

    printf("End = %.2f\n", end);

    printf("Elapsed = %.2f\n", end - start);

    // Result check
    for (int i = 1; i < size; i++)
    {
        if (!(a[i - 1] <= a[i]))
        {
            printf("Implementation error: a[%d]=%d > a[%d]=%d\n", i - 1,
                   a[i - 1], i, a[i]);
            return 1;
        }
    }
    puts("-Success-");
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
        printf("Error: %d threadCount\n", threadCount);
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