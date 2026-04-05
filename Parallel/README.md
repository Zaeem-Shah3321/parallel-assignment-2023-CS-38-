# Parallel Merge Sort (MPI)

## Overview
This program implements a parallel version of the Merge Sort algorithm using MPI (Message Passing Interface). The array is distributed across multiple processes, each sorting its local portion before gathering and performing a final merge on the root process.

## Algorithm
Parallel Merge Sort follows this approach:
1. **Root process (rank 0)** generates the full random array
2. **Scatter** – Array chunks are distributed equally among all processes
3. **Local sort** – Each process performs sequential merge sort on its chunk
4. **Gather** – Sorted chunks are collected back to the root process
5. **Final merge** – Root process merges all sorted chunks into a fully sorted array

**Time Complexity:** O((n/p) log(n/p) + n log p) – includes communication overhead  
**Space Complexity:** O(n/p) per process + O(n) on root

## Requirements
- MPI library (OpenMPI or MPICH)
- C compiler with MPI support (mpicc)
- Standard C libraries

## Compilation
```bash
mpicc -o2 -o parallel parallel.c
```
USAGE
-----
Run with default size (100,000 elements):
mpirun -np <num_processes> ./parallel

With custom array size:
mpirun -np <num_processes> ./parallel <array_size>