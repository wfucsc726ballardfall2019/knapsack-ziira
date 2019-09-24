To compile sequential code:

`g++ -fopenmp -o knapsack_seqential knapsack_sequential.cpp`

(Note that -fopenmp is required to use OpenMP timers)

To run sequential code:

`./knapsack_sequential CAPACITY NUMITEMS`

YOUR ASSIGNMENT

1. Run the code as is to find ranges of CAPACITY and NUMITEMS that require around 10 seconds to run
    * record some of the running times and draw some conclusions from what you observe
2. Read the code and see if there are any cache locality optimizations that could improve performance
    * keep your optimizations separate from the original functions
    * record and explain any running time improvements you make
3. Use OpenMP to parallelize each of the 3 algorithms in a new file called knapsack_parallel.cpp
    * memory inefficient (value & solution), memory efficient (value only), memory efficient (value & solution)
    * measure your parallel performance and record strong scaling results
    * draw some conclusions from what you observe
