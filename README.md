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
|  | 1 Threads | 8 Thread |
| :--- | :--- | :--- |
| Average Execution Time (Seconds) | .45 | .29 |
| Number of Primes Found | 5761455 | 5761455 |
| Sum of all Primes | 279209790387276 | 279209790387276 |  
  
  We notice a stark difference from 1 thread to 8 threads with a .15 second difference and would see a greater difference if the numbers checked, n, was higher. The interesting thing to notice is that if the threads were cut down from 8 to 2, it would give pretty much the same average time. That tracks all the way up to 9-10 threads which right after begans to fall off again and take a little bit longer to execute. A clear example that more does not equal better in this case as well as not equaling the same performance gains as the previous number of threads gives compared to ITS previous. Perhaps if we had more numbers to check and needed more processing power more threads would offer more gains, but at this amount of data, it is unneeded.

## Thread Design and Load Balancing
  As stated previously above, the threads were being scheduled dynamically. When they were in function they would keep looping, going back to the top, grabbing the counter, and repeat the process until the counter limit was reached. To decide how much I wanted to increment by i simply grabbed the max number I was going to check (sqrt(10^8)) divided it by 8 (# of Threads) and found an even number so each thread has the same amount of function loops.
  
## Why Sieve of Eratosthenes?
  Honestly, this was one of the most basic sieves to implement as well as super efficient and well known with the CS community. As it was my first time working with multi-threading I did not wish to struggle with two things at once, which is not to say I did not struggle with the implementation of the sieve (because i did). The sieve has a O(N * Log * Log(N)).

  ## Reflection
  ### Learned About Parallel Programming
  I have not learned as much as solidify my knowledge of parallel programming with this project as the book and slides helped greatly to introduce me to this. Though there is one thing that stands out as I completed this project which was my method truly parallel programming or was it just concurrency? My confusion was "how is it parallel if the threads are not grabbing the counter at the literal exact time (which would also not be possible with an atomic counter)?" After a bit of digging, the algorithm working with each thread was the parallel processing not the grabbing of the value of the counter!
  ### Improvements?
  With this current amount of input data, I do not think many improvements could be made but I have not searched that deep. Now for a big number, maybe there can be a way of saving the early "static" numbers that we know, such as the start of the crossing out like "p + p", and apply that to the sieve and save some redundant processing at the start?
  ### LLM Assistance?
  I did use an LLM to assist me and it was insanely useful. Matter fact, the LLM gave me the idea to use a bitset (something I have not heard too much of) to cut down massively on the size and speed of the data structure used rather than using a full byte for each number! Since it was also not common to find much knowledge on how/what/why for a bitset and it walked me through the whole thing from a beginner level of setting bits to understanding that setting bits must be atomic as well. In terms of how i verified my output, well I tested it throughout the assignment inching closer to the correct outputs that were online.