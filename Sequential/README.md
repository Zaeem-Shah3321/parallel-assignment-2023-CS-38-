# Sequential Merge Sort

## Overview
This program implements a sequential version of the Merge Sort algorithm in C. It generates an array of random integers and sorts them using the divide-and-conquer approach of merge sort.

## Algorithm
Merge Sort is a divide-and-conquer algorithm that:
1. **Divides** the array into two halves
2. **Recursively sorts** each half
3. **Merges** the two sorted halves back together

**Time Complexity:** O(n log n)  
**Space Complexity:** O(n) for temporary arrays

## Requirements
- C compiler (GCC recommended)
- Standard C libraries (stdio.h, stdlib.h, time.h)

## Compilation
```bash
gcc -o mergesort mergesort.c
```
       
Optimized:    gcc -O2 -o mergesort mergesort.c

USAGE
-----
./mergesort              (uses default size: 100,000 elements)
./mergesort 50000        (sorts 50,000 elements)
./mergesort 1000000      (sorts 1,000,000 elements)

OUTPUT
------
- For any array size: Displays execution time in seconds
- For array size ≤ 20: Also displays the sorted array

Sample output: Time taken (sequential): 0.050531 seconds

CODE STRUCTURE
--------------
Function        Description
--------        -----------
main()          Entry point - creates array, measures time, calls mergesort
mergesort()     Recursively divides the array into halves
merge()         Merges two sorted subarrays using temporary storage

PERFORMANCE NOTES
-----------------
- Array filled with random integers between 0-4999
- Uses clock() for timing (CPU time, not wall-clock time)
- Memory dynamically allocated and freed
- Fixed random seed (same input produces same random numbers)

LIMITATIONS
-----------
- Sequential implementation only (no parallelization)
- Maximum array size limited by available RAM
- Fixed random number range (0-4999)

EXTENDING FOR PARALLEL EXPERIMENTS
----------------------------------
1. Add MPI directives: #include <mpi.h>
2. Distribute array portions across processes
3. Implement parallel merge steps
4. Measure parallel execution time