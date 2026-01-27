// Gabriel Alves
// Day-Month-Year = 27/01/2026
// COP 4520 - Assignment 1

#include <stdio.h>
#include <time.h> // For execution time
#include <stdint.h> // For the explicit bit types
#include <stdatomic.h> // For the atomic counter and atomic read/write to bitset
#include <pthread.h> // Does not support Windows Systems natively

// Uses bit operations to access the bitset since it it quicker than using mathmatical operations (index byte == 64 bits)
#define GET_BYTE(i) (i >> 6) 
#define GET_BIT(i) (i & 63) // Have to wrap this in paranthesis or else << takes precedence before the & operation and messes up everything.
#define SET_BIT(i) __sync_fetch_and_or(&bitSet[GET_BYTE(i)], (1ULL << GET_BIT(i)))
#define IS_BIT_SET(i) (bitSet[GET_BYTE(i)] & (1ULL << GET_BIT(i)))
//
#define THREADS 8

const char outFileName[] = "primes.txt";
atomic_int n = 2; // Numbers to check if prime (No need to check for 0 or 1 and stops at 10000 since that is the sqrt of 10^8)
uint64_t bitSet[1562500]; // Holds 10^8 bits. Each one represents a number. 0 == Prime, 1 == Composite.

// Sieve of Eratosthenes
// Fetches and increments atomic counter, then multiplies with itself to create the first number to be checked.
void *Sieve(void *data);

int main(){
    pthread_t thread[THREADS];
    struct timespec start, end;
    int totalPrimes = 0;
    long long primesSum = 0;
    int largestPrimes[10];
    int topTenFlag = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int t = 0; t < THREADS; t++){ //TODO Try to complete it with one thread because 8 threads is issues rn 
        pthread_create(&thread[t], NULL, Sieve, NULL);
    }
    for(int t = 0; t < THREADS; t++){
        pthread_join(thread[t], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1e9);
    printf("%f seconds\n", time);

    for(int i = 0; i < 20; i++){
        int is_composite = ((bitSet[0] & (1ULL << i)) != 0);
        printf("%d ", is_composite);
    }
    printf("\n");

    for(int i = 100000000; i > 1; i--){
        if(IS_BIT_SET(i) == 0){
            if(topTenFlag < 10){
                largestPrimes[9 - topTenFlag] = i; // For ascending order
                topTenFlag++;
            }

            primesSum += i;
            totalPrimes++;
        }
    }

    printf("\n%d primes\n", totalPrimes ); // Subtract 2 for the 0 and 1
    printf("%lld\n", primesSum);
    for(int i = 0; i < 10; i++){
        printf("%d ", largestPrimes[i]);
    }
    printf("\n");

    FILE *out = fopen(outFileName, "w");
    // TODO Print to file (execution time, total number of primes found, sum of all the primes found, top ten largest primes in ascending order (ex. 1,2,3,4,5))
    fclose(out);
}

void *Sieve(void *argv){
    // Increments by 50 should give each thread 25 runs
    int increment = 50;

    while(1){
        int x = atomic_fetch_add(&n, increment);
        if(x > 10000) // Breaks thread and joins back with main
            return NULL;

        for(int i = 0; i < increment; i++){ // This increments the internal counter within the range this thread has been given
            int tempCount = x + i;
            if(tempCount > 10000) // Same check as before but count starts at 2 therefore one thread would be checking 2 more loops than needed when only 48 loops is necessary (10002)
                break;

            if(IS_BIT_SET(tempCount) == 0){
                tempCount *= tempCount;
                while(tempCount <= 100000000){ // This checks for factors
                    SET_BIT(tempCount);
                    tempCount += (x + i);
                } 
            }
        }  
    }
}