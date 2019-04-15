#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "hw6_C.h"

#define SMALL 32
#define SIZE 10

// A is our actual array and B is our final destination for sorting

//pmergeMS (T, p1, r1, p2, r2, A, p3)
void pmergeMS(int t[], int left1, int right1, int left2, int right2, int a[], int p3)
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

        int q2 = binarySearchMS(t[q1], t, left2, right2);

        int q3 = p3 + (q1 - left1) + (q2 - left2);
        a[q3] = t[q1];

        //TODO: fix this seg fault

#pragma omp parallel
        {

#pragma omp single
            {
                pmergeMS(t, left1, q1 - 1, left2, q2 - 1, a, p3);
            }
#pragma omp single
            {
                pmergeMS(t, q1 + 1, right1, q2, right2, a, q3 + 1);
            }
        }
    }
}

// needed for p merge
int binarySearchMS(int x, int arr[], int p, int r)
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

void ms(int a[], int b[], int p, int r, int a_to_b)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        ms(a, b, p, q, !a_to_b);
        ms(a, b, q + 1, r, !a_to_b);
        if (a_to_b)
            pmergeMS(a, p, q, q + 1, r, b, p);
        else
            pmergeMS(b, p, q, q + 1, r, a, p);
    }
}
void mergesort140(int src[], int dst[], int n)
{
    memcpy(dst, src, n * sizeof(int));
    ms(src, dst, 0, n - 1, 1);
}