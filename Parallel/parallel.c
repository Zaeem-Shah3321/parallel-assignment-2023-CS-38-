#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// merge function
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;

    int size1 = mid - left + 1;
    int size2 = right - mid;

    int *temp1 = (int*)malloc(size1 * sizeof(int));
    int *temp2 = (int*)malloc(size2 * sizeof(int));

    for(i = 0; i < size1; i++)
        temp1[i] = arr[left + i];

    for(j = 0; j < size2; j++)
        temp2[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while(i < size1 && j < size2)
    {
        if(temp1[i] < temp2[j])
            arr[k++] = temp1[i++];
        else
            arr[k++] = temp2[j++];
    }

    while(i < size1)
        arr[k++] = temp1[i++];

    while(j < size2)
        arr[k++] = temp2[j++];

    free(temp1);
    free(temp2);
}

void mergesort(int arr[], int start, int end)
{
    if(start < end)
    {
        int mid = (start + end) / 2;

        mergesort(arr, start, mid);
        mergesort(arr, mid + 1, end);

        merge(arr, start, mid, end);
    }
}

int main(int argc, char *argv[])
{
    int rank, totalProcs;
    int n = 100000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);

    if(argc > 1)
        n = atoi(argv[1]);

    int *fullArray = NULL;

    int partSize = n / totalProcs;
    int *localArr = (int*)malloc(partSize * sizeof(int));

    // only root creates full array
    if(rank == 0)
    {
        fullArray = (int*)malloc(n * sizeof(int));

        for(int i = 0; i < n; i++)
            fullArray[i] = rand() % 5000;
    }

    double startTime = MPI_Wtime();

    // distribute data
    MPI_Scatter(fullArray, partSize, MPI_INT, localArr, partSize, MPI_INT, 0, MPI_COMM_WORLD);

    // each process sorts its own part
    mergesort(localArr, 0, partSize - 1);

    // gather back
    MPI_Gather(localArr, partSize, MPI_INT, fullArray, partSize, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        // final merge
        mergesort(fullArray, 0, n - 1);

        double endTime = MPI_Wtime();

        printf("Time taken (parallel): %f seconds\n", endTime - startTime);

        if(n <= 20)
        {
            printf("Sorted array:\n");
            for(int i = 0; i < n; i++)
                printf("%d ", fullArray[i]);
        }
    }

    MPI_Finalize();

    free(localArr);
    if(rank == 0)
        free(fullArray);

    return 0;
}