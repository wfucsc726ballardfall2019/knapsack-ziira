#!/bin/bash -l

export OMP_NUM_THREADS=1
echo $OMP_NUM_THREADS
echo "Sequential"
./knapsack_sequential 100000 1500 1
./knapsack_sequential 100000 3000 1
./knapsack_sequential 1000000 300 1
./knapsack_sequential 2000000 30 1
echo "Cache"
./knapsack_cache 100000 1500 1
./knapsack_cache 100000 3000 1
./knapsack_cache 1000000 300 1
./knapsack_cache 2000000 30 1
echo "Parallel"
./knapsack_parallel 100000 1500 1
./knapsack_parallel 100000 3000 1
./knapsack_parallel 1000000 300 1
./knapsack_parallel 2000000 30 1
export OMP_NUM_THREADS=2
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 2
./knapsack_parallel 100000 3000 2
./knapsack_parallel 1000000 300 2
./knapsack_parallel 2000000 30 2
export OMP_NUM_THREADS=4
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 4
./knapsack_parallel 100000 3000 4
./knapsack_parallel 1000000 300 4
./knapsack_parallel 2000000 30 4
export OMP_NUM_THREADS=6
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 6
./knapsack_parallel 100000 3000 6
./knapsack_parallel 1000000 300 6
./knapsack_parallel 2000000 30 6
export OMP_NUM_THREADS=8
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 8
./knapsack_parallel 100000 3000 8
./knapsack_parallel 1000000 300 8
./knapsack_parallel 2000000 30 8
export OMP_NUM_THREADS=11
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 11
./knapsack_parallel 100000 3000 11
./knapsack_parallel 1000000 300 11
./knapsack_parallel 2000000 30 11
export OMP_NUM_THREADS=16
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 16
./knapsack_parallel 100000 3000 16
./knapsack_parallel 1000000 300 16
./knapsack_parallel 2000000 30 16
export OMP_NUM_THREADS=22
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 22
./knapsack_parallel 100000 3000 22
./knapsack_parallel 1000000 300 22
./knapsack_parallel 2000000 30 22
export OMP_NUM_THREADS=32
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 32
./knapsack_parallel 100000 3000 32
./knapsack_parallel 1000000 300 32
./knapsack_parallel 2000000 30 32
export OMP_NUM_THREADS=33
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 33
./knapsack_parallel 100000 3000 33
./knapsack_parallel 1000000 300 33
./knapsack_parallel 2000000 30 33
export OMP_NUM_THREADS=44
echo $OMP_NUM_THREADS
echo "Parallel"
./knapsack_parallel 100000 1500 44
./knapsack_parallel 100000 3000 44
./knapsack_parallel 1000000 300 44
./knapsack_parallel 2000000 30 44