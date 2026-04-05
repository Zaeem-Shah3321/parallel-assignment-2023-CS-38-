#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int arr[], int left, int mid, int right);
void mergesort(int arr[], int start, int end);

// merge two parts of array
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int size1 = mid - left + 1;
    int size2 = right - mid;

    int *temp1 = (int *)malloc(size1 * sizeof(int));
    int *temp2 = (int *)malloc(size2 * sizeof(int));

    // copy data
    for (i = 0; i < size1; i++)
        temp1[i] = arr[left + i];

    for (j = 0; j < size2; j++)
        temp2[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    // merge back
    while (i < size1 && j < size2)
    {
        if (temp1[i] < temp2[j])
        {
            arr[k] = temp1[i];
            i++;
        }
        else
        {
            arr[k] = temp2[j];
            j++;
        }
        k++;
    }

    // remaining elements
    while (i < size1)
    {
        arr[k] = temp1[i];
        i++;
        k++;
    }
    while (j < size2)
    {
        arr[k] = temp2[j];
        j++;
        k++;
    }

    free(temp1);
    free(temp2);
}

// main merge sort
void mergesort(int arr[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;

        mergesort(arr, start, mid);
        mergesort(arr, mid + 1, end);

        merge(arr, start, mid, end);
    }
}

int main(int argc, char *argv[])
{
    int n = 100000;
    if (argc > 1)
        n = atoi(argv[1]);

    int *arr = (int *)malloc(n * sizeof(int));

    // fill with random values
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 5000;

    clock_t startTime = clock();
    mergesort(arr, 0, n - 1);
    clock_t endTime = clock();
    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    printf("Time taken (sequential): %f seconds\n", totalTime);

    // small check
    if (n <= 20)
    {
        printf("Sorted array:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
    }

    free(arr);
    return 0;
}