#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// merge two sorted subarrays
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

// NEW FUNCTION: Merge multiple sorted arrays into one
void k_way_merge(int **chunks, int *chunkSizes, int numChunks, int result[])
{
    int *indices = (int*)calloc(numChunks, sizeof(int)); // current index in each chunk
    int resultPos = 0;
    
    while(1)
    {
        int smallestValue = 2147483647; // INT_MAX
        int smallestChunk = -1;
        
        // Find the smallest current element among all chunks
        for(int i = 0; i < numChunks; i++)
        {
            if(indices[i] < chunkSizes[i] && chunks[i][indices[i]] < smallestValue)
            {
                smallestValue = chunks[i][indices[i]];
                smallestChunk = i;
            }
        }
        
        // If no more elements, break
        if(smallestChunk == -1)
            break;
        
        // Add smallest element to result
        result[resultPos++] = smallestValue;
        indices[smallestChunk]++;
    }
    
    free(indices);
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
    
    // Ensure n is divisible by totalProcs for simplicity
    if(n % totalProcs != 0)
    {
        if(rank == 0)
            printf("Warning: n=%d not divisible by totalProcs=%d. Adjusting n to %d\n", 
                   n, totalProcs, n - (n % totalProcs));
        n = n - (n % totalProcs);
    }
    
    int partSize = n / totalProcs;
    int *localArr = (int*)malloc(partSize * sizeof(int));
    int *fullArray = NULL;
    
    // Only root creates and holds the full unsorted array
    if(rank == 0)
    {
        fullArray = (int*)malloc(n * sizeof(int));
        for(int i = 0; i < n; i++)
            fullArray[i] = rand() % 5000;
    }
    
    double startTime = MPI_Wtime();
    
    // Step 1: Distribute data to all processes
    MPI_Scatter(fullArray, partSize, MPI_INT, localArr, partSize, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Step 2: Each process sorts its own chunk locally
    mergesort(localArr, 0, partSize - 1);
    
    // Step 3: Send all sorted chunks to root for final k-way merge
    if(rank == 0)
    {
        // Root process: collect all sorted chunks
        int **sortedChunks = (int**)malloc(totalProcs * sizeof(int*));
        int *chunkSizes = (int*)malloc(totalProcs * sizeof(int));
        
        // First, store root's own sorted chunk
        sortedChunks[0] = localArr;
        chunkSizes[0] = partSize;
        
        // Receive chunks from other processes
        for(int i = 1; i < totalProcs; i++)
        {
            int *recvBuffer = (int*)malloc(partSize * sizeof(int));
            MPI_Recv(recvBuffer, partSize, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sortedChunks[i] = recvBuffer;
            chunkSizes[i] = partSize;
        }
        
        // Step 4: Perform k-way merge to get final sorted array
        int *finalSorted = (int*)malloc(n * sizeof(int));
        k_way_merge(sortedChunks, chunkSizes, totalProcs, finalSorted);
        
        double endTime = MPI_Wtime();
        
        printf("Time taken (parallel with k-way merge): %f seconds\n", endTime - startTime);
        
        // Optional: copy back to fullArray for verification
        for(int i = 0; i < n; i++)
            fullArray[i] = finalSorted[i];
        
        if(n <= 20)
        {
            printf("Sorted array:\n");
            for(int i = 0; i < n; i++)
                printf("%d ", finalSorted[i]);
            printf("\n");
        }
        
        // Clean up received chunks
        for(int i = 1; i < totalProcs; i++)
            free(sortedChunks[i]);
        free(sortedChunks);
        free(chunkSizes);
        free(finalSorted);
    }
    else
    {
        // Non-root processes: send sorted chunk to root
        MPI_Send(localArr, partSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    
    free(localArr);
    if(rank == 0)
        free(fullArray);
    
    return 0;
}