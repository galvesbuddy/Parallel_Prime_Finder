## My Approach
  ### Structure
  I used the Sieve of Eratosthenes to find primes up to and including 10^8. I stored the information within a Bitset to save nearly 8x the amount of MB a normal int array would have used. Each word within the bitset holds 64 bits.  
  ### Workload
  The program creates 8 threads which each take ranges of 50 starting from 2, perform the sieve on each number within its range, then saving it within the bitset. Since we are using multiple threads, operations that include reading and writing to a certain location/variable must be handled differently as to make sure all threads are using the correct information. That is where Atomic Types come in. There are two atomic variables in this program, the counter to let a thread know which number to start checking from and bit setting within the bitset.  
  ### Libraries
  For libraries used there is "stdio.h" for the file writing, "time.h" to calculate execution time, "stdint.h" for the explicit bit types like "uint64_t", "stdatomic.h" for the atomic counter and atomic read/write to bitset, and "pthread.h" to manage the threads(Does not support Windows Systems natively).  

## Correctness and Efficiency
  ### Sieve of Eratosthenes
  The sieve starts at the first prime of its loop (always 2 for the first loop) and marks all of its factors as composites. Then it moves onto its next loop, skips all composites and starts at the next prime (3 for the second loop) does the same marking for composites and continues this marking until we reach 10^8 loops.
  ### Optimizations
  By nature the closer we are to the end number the less checks we do since previous loops have already crossed out quite a bit of future numbers. For example just starting with 2 cuts the dataset in half since that means all even numbers after 2 do not need to be checked. We can also stop the outer loop early by stopping on the sqrt(10^8). We can do this because after the sqrt(10^8) we are just checking numbers that we already checked just in reverse. Using the example 10^8 divided by 4 we get 25 million and if we do 10^8 divided by 25 million we get 4, the arguments are the same just the order is different and luckily for us that works in our favor for another big time cut.
  ### Execution Time and Thread Workloads
  The biggest contribution to the low execution time, if we ignore size of input, would be the algorithm and choices of optimizations within it. In terms of thread Workloads, each thread is given a range of 50 starting from 2 and ending at 10,000 (last thread has a range of 48). This allows Each thread to have about 25 runs each. If we increase the increments/range to about 100 its a few milliseconds slower and doesnt change much from lowering the range either.

## Experimental Evaluation
  
| 1 Threads | 8 Thread |
| :--- | :--- | :--- |
| Content under Header 1 | Content under Header 2 |

  



3. Experimental Evaluation Summarize the performance of your program based on
actual execution. Your evaluation should include both your 8-threaded solution and a
baseline single-threaded version of your program. Address the following points:
  • Total execution time for both versions
  • Number of primes found and their sum (should be identical in both versions)
  • Observations about thread performance, including any bottlenecks or uneven
workload distribution
  • Comparison of performance between the multi-threaded and single-threaded
  implementations:
    o How much faster (or slower) is the parallel version?
    o Are there diminishing returns or overhead introduced by threading?
    o What factors affect the scalability of your solution?
  • Discussion of scalability:
    o How might performance change with different input sizes?
    o What would happen if you increased or decreased the number of threads?
    You may include tables, charts, or graphs to support your analysis.
4. Thread Design and Load Balancing Explain how you ensured that each thread
received a fair share of the work. Consider:
  • How you partitioned the input range
  • Whether you used static or dynamic scheduling
  • Any challenges you encountered in balancing execution time across threads
5. Prime-Finding Algorithm Describe the algorithm you used to identify prime
numbers. Address:
  • Why you chose this algorithm
  • Its time complexity
  • Any optimizations you implemented (e.g., skipping even numbers, using a sieve)
6. Reflection Reflect briefly on your experience completing this assignment. Consider:
  • What you learned about parallel programming
  • What you might improve in a future version
  • Whether you used any tools (e.g., AI assistants) and how you verified their output