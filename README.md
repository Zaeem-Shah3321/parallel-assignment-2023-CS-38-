# Parallel Assignment: Merge Sort Performance Analysis

## Student Information
- Name: [Your Name]
- Roll Number: [Your Roll Number]

## Problem Description
This project implements and compares sequential vs. parallel versions of the Merge Sort algorithm. Merge Sort is a divide-and-conquer algorithm with O(n log n) time complexity. The parallel implementation distributes the array across multiple MPI processes, each sorting its local chunk before gathering and performing a final merge.

**Why chosen:** Merge Sort naturally lends itself to parallelization due to its divide-and-conquer nature, making it ideal for studying parallel performance metrics like speedup and efficiency.

## How to Run

### Sequential Version
```bash
# Compile
gcc -O2 -o sequential sequential/sequential.c

# Run with default size (100,000)
./sequential

# Run with custom size
./sequential 1000000
```

Parallel Version
```bash
# Compile
mpicc -O2 -o parallel parallel/parallel.c

# Run with 2 processes
mpirun -np 2 ./parallel 100000

# Run with 4 processes
mpirun -np 4 ./parallel 100000

# Run with 8 processes
mpirun -np 8 ./parallel 100000

# Run with custom size and processes
mpirun -np 4 ./parallel 500000
```
Results Summary
Performance Data (Problem Size: 100,000 elements)
Processes	Time (seconds)	Speedup	Efficiency
1 (Seq)	0.072155	0.700	0.700
2	0.055372	0.913	0.456
4	0.065245	0.774	0.194
8	0.090823	0.556	0.070

## Key Findings
Best speedup achieved: 0.913x with 2 processes
Efficiency at highest process count (8 processes): 7.0%
Observation: Speedup < 1 for all cases indicates that parallelization overhead dominates for this problem size

## Analysis
The parallel implementation underperforms compared to sequential due to:
Communication overhead – Scatter and gather operations add latency
Small problem size – 100,000 elements is too small to overcome parallel overhead
Sequential final merge – Root process bottleneck
Load imbalance – Perfect division only when n % p == 0

## Recommendations
For n = 100,000, sequential version is actually faster

Parallel benefits will appear for larger problem sizes (n > 10,000,000)

Use hierarchical merging to improve final merge bottleneck

## Repository Structure
```bash
Parallel-Assignment/
├── sequential/
│   └── sequential.c          # Sequential merge sort implementation
├── parallel/
│   └── parallel.c            # MPI-based parallel merge sort
├── results/
│   ├── timing_data.csv       # Execution time measurements
│   ├── speedup_plot.png      # Speedup vs processes graph
│   └── time_comparison.png   # Sequential vs parallel time plot
├── report/
│   └── final_report.pdf      # Complete analysis report
├── Makefile                  # Build automation
└── README.md                 # Project overview
```
## Speedup and Efficiency Formulas
text
Speedup     = Sequential_Time / Parallel_Time
Efficiency  = Speedup / Number_of_Processes × 100%

For 2 processes:
Speedup     = 0.050531 / 0.055372 = 0.913
Efficiency  = 0.913 / 2 × 100% = 45.6%

For 4 processes:
Speedup     = 0.050531 / 0.065245 = 0.774
Efficiency  = 0.774 / 4 × 100% = 19.4%

For 8 processes:
Speedup     = 0.050531 / 0.090823 = 0.556
Efficiency  = 0.556 / 8 × 100% = 7.0%
Required Tools
GCC – Sequential compilation

MPI (OpenMPI/MPICH) – Parallel compilation and execution


## Conclusion
This assignment demonstrates that parallelization is not always beneficial. The overhead of communication, synchronization, and sequential bottlenecks can outweigh the benefits of parallel execution for small problem sizes. Understanding these trade-offs is crucial for effective parallel algorithm design.