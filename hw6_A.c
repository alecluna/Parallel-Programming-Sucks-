#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SIZE 10

extern double get_time(void);
int binarySearch(int x, int arr[], int p, int end);
void pmerge(int t[], int left1, int right1, int left2, int right2, int a[], int p3);
void pMergeSort(int A[], int left, int right, int B[], int s);

int main(int argc, char *argv[]);

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

        pMergeSort(A, p, q, T, 0);
        pMergeSort(A, q + 1, r, T, qPrime + 1);
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

    //n1 empty, sorting algo exits
    if (n1 == 0)
    {
        return;
    }
    else
    {
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