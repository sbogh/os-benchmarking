#include "memory_header.h"
/* 
* @file memory.cpp
* @author Shayan Boghani
*
* Memory Module
 */

/*
What needs to be done

- RAM Access Time
    - individual integer accesses
        - main memory
        - L1 cache
        - L2 cache
        - L3 cache
    - Results
        - Present as graph with the x-axis as the log of the size of the memory 
        region accessed, and the y-axis as the average latency
    - Hint
        - Use lmbench "back-to-back-load" latency measurement as reference
- RAM bandwidth
    - Reading
    - Writing

    - Hint
        - Use loop unrolling
        - lmbench: keep in mind the effects of cache line prefetching
            - stride may not be sufficient
            - Use LinkedList or serialize using CPUID
        - keep in mind that RAM bandwidth is likely limited by the DRAM and not the memory bus bandwidth

        - will know from part 1 the size of array to ensure you are reading/writing from memory instead of caches
        - ok to using prefetching for bandwidth because we won't be able to hit full RAM bandwidth without it
- Page Fault Service Time
    - Report the time for faulting an entire page from disk (mmap is one useful mechanism)
    - Compare to latency of accessing a byte from main memory

*/

/**
 * Memory Operation 1: Individual Integer Access -> L1, L2, L3 Caches and Main Memory
 * 
 * @param size the size of the desired array
 * @return double with average latency per loop
 */
double memory_accessTime(int size)
{
    // initialize array and time measurement vars
    vector<int> genArr = get_array(size / sizeof(int)); // convert size in bytes to array size (sizeof(int) == 4)
    uint64_t measureInit;
    uint64_t measureEnd;

    // cache warming (access and write to all elements in array)
    for(int i = 0; i < genArr.size(); i++)
    {
        genArr[i] = 0;
    }

    // initialize variables for measurement
    int arraySize = genArr.size(); // array size
    int dummy; // dummy variable to hold accessed values

    // perform start time read
    getCPUID(); // force serialization
    measureInit = getTime(); // get timestamp

    // for loop on length of array
    for(int i = 0; i < arraySize; i++)
    {
        dummy = genArr[(i * 16) % arraySize]; // stride of 16 added to ensure prefetching does not influence, mod added to prevent out of bounds error
    }
    // perform end time read
    measureEnd = getTime(); // get timestamp
    getCPUID(); // force serialization
    
    return ((double)(measureEnd - measureInit) / (double) arraySize); // return average time per access
}

/**
 * Memory Operation 2a: RAM Bandwidth Read
 * 
 * @return double with bandwidth in GB/s
 */
__attribute__((optimize("no-tree-vectorize"))); // force optimizer to not vectorize loop instructions for this function
double memory_bandwidthRead(int size)
{
    // initialize array and time measurement vars
    vector<int> genArr = get_array(size / sizeof(int));
    uint64_t measureInit;
    uint64_t measureEnd;

    long long int dummy; // dummy variable to ensure optimizer does not erase accesses
    int arraySize = genArr.size(); // size of array

    // read timestamp
    getCPUID(); // force serialization
    measureInit = getTime(); // read start time
    // for loop running for desired loop count
    for(int i = 0; i < LOOP_COUNT; i++)
    {
        // 5 reads from array that are added to dummy var (stride set to 16 to mitigate prefetching effects)
        dummy = dummy + genArr[(i * 16) % arraySize];
        dummy = dummy + genArr[((i * 16) + 1) % arraySize];
        dummy = dummy + genArr[((i * 16) + 2) % arraySize];
        dummy = dummy + genArr[((i * 16) + 3) % arraySize];
        dummy = dummy + genArr[((i * 16) + 4) % arraySize];
    }
    // read timestamp
    measureEnd = getTime(); // get end time
    getCPUID(); // force serialization

    // unit conversions from ns -> s and bytes -> GB
    double wallClockTime = cyclesToTime(measureInit, measureEnd) / pow(10, 9); // seconds
    double dataGB = (double)(sizeof(int) * 5 * LOOP_COUNT) / pow(1024, 3); // GB

    return ((double) dataGB / (double) wallClockTime); // return GB/s
}

/**
 * Memory Operation 2b: RAM Bandwidth Write
 * 
 * @return double with bandwidth in GB/s
 */
__attribute__((optimize("no-tree-vectorize"))); // force optimizer to not vectorize loop instructions for this function
double memory_bandwidthWrite()
{
    // initialize array and time measurement vars
    vector<int> genArr = get_array(size / sizeof(int));
    uint64_t measureInit;
    uint64_t measureEnd;
    int arraySize = genArr.size(); // size of array

    // read timestamp
    getCPUID();
    measureInit = getTime(); // get start time
    // for loop running for desired loop count
    for(int i = 0; i < LOOP_COUNT; i++)
    {
        // 5 writes to the array that are added to dummy var (stride set to 16 to mitigate prefetching effects)
        genArr[(i * 16) % arraySize] = 0;
        genArr[((i * 16) + 1) % arraySize] = 0;
        genArr[((i * 16) + 2) % arraySize] = 0;
        genArr[((i * 16) + 3) % arraySize] = 0;
        genArr[((i * 16) + 4) % arraySize] = 0;
    }
    // read timestamp
    measureEnd = getTime(); // get end time
    getCPUID();

    // unit conversions from ns -> s and bytes -> GB
    double wallClockTime = cyclesToTime(measureInit, measureEnd) / pow(10, 9); //seconds
    double dataGB = (double)(sizeof(int) * 5 * LOOP_COUNT) / pow(1024, 3); // GB

    return ((double) dataGB / (double) wallClockTime); // return GB/s
}

/**
 * Memory Operation 3: Page Fault Service Time
 * 
 * @return double with average time per page fault
 */
double memory_pageFaultServTime()
{
    // initialize time vars and average time holder
    uint64_t measureInit;
    uint64_t measureEnd;
    double totTime = 0;

    // initialize file name and get fd
    const char* file = "dummy";
    int fd = get_fd(file);

    // perform memory mapping
    char* pageMap = (char *) mmap(0, DUMMY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // map dummy file of DUMMY_SIZE to mem, can read and write mapping, no offset
    
    // check to make sure memory mapping succeeded, else continue to attempt to map until success
    while (pageMap == MAP_FAILED)
    {
        char* pageMap = (char *) mmap(0, DUMMY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // map dummy file of DUMMY_SIZE to mem, can read and write mapping, no offset
    }

    // conduct page fault timing
    for(int i = 0; i < 512; i++)
    {
        // get timestamp
        getCPUID(); // force serialization
        measureInit = getTime(); // get start time

        pageMap[i * PAGE_SIZE] = 0; // access page i (offset by size of page)

        // get timestamp
        measureEnd = getTime(); // force serialization
        getCPUID(); // get end time

        totTime += (double)(measureEnd - measureInit); // add loop time to total time
    }

    totTime = totTime / 3000; // convert cycles to ns
    double avgTime = totTime / 512; // average over pages accessed

    return (double) avgTime; // return averaged time
}