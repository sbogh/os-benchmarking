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
    // look into volatile
    for(int i = 0; i < arraySize; i++)
    {
        dummy = genArr[(i * 16) % arraySize]; // stride of 16 added to ensure prefetching does not influence, mod added to prevent out of bounds error
    }
    // perform end time read
    measureEnd = getTime(); // get timestamp
    getCPUID(); // force serialization

    double clockTime = cyclesToTime(measureInit, measureEnd); // convert time from cycles to ns
    
    return ((double) time / (double) arraySize); // return average time per access
}

/**
 * Memory Operation 1-ALTERNATE: Individual Integer Access -> L1, L2, L3 Caches and Main Memory using LL implementation
 * 
 * @param size the size of the desired array
 * @return double with average latency per loop
 */
double memory_accessTime_LL(int size)
{
    // initialize time stamp variables and linked list vector
    uint64_t measureInit;
    uint64_t measureEnd;

    Vector<Node> linkedList(size); // initialize vector of size with structure Node
    Vector<Node*> pointers(size); // initialize vector to store pointers to Linked List Nodes

    // for loop to assign the address of Linked List node to a pointer in the pointers list
    for(int i = 0; i < size; i++)
    {
        pointers[i] = &linkedList[i];
    }

    // shuffle pointers to ensure randomized order for Linked List
    random_device randDev;
    mt19937 g(randDev());
    shuffle(begin(pointers), end(pointers), g);

    // create Linked List by assigning next pointer to a random node (based on shuffling done above)
    for(int i = 0; i < size - 1; i++)
    {
        pointers[i]->next = pointers[i + 1];
    }

    Node* head = pointers[0]; // define the head of the Linked List
    Node* temp = head; // create a pointer that points to head

    // get timestamp
    CPUID(); // force serialization
    measureInit = getTime(); // get start time

    // access every Node in the entire Linked List
    for(int i = 0; i < size; i++)
    {
        temp = temp->next;
    }

    // get timestamp
    measureEnd = getTime(); // get end time
    CPUID(); // force serialization

    double clock_time = cyclesToTime(measureInit, measureEnd); // convert time from cycles to ns

    return (double) clock_time / (double) size; // return averaged time in ns per access
}


/**
 * Memory Operation 2a: RAM Bandwidth Read
 * 
 * @return double with bandwidth in GB/s
 */
// force optimizer to not vectorize loop instructions for this function
extern "C" __attribute__((optimize("no-tree-vectorize"))) double memory_bandwidthRead(int size)
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
    for(int i = 0; i < LOOP_COUNT; i += 64)
    {
        // 4 reads from array that are added to dummy var (1 instruction per cache line)
        dummy = dummy + genArr[(i + (16)) % arraySize];
        dummy = dummy + genArr[(i + (16 * 2)) % arraySize];
        dummy = dummy + genArr[(i + (16 * 3)) % arraySize];
        dummy = dummy + genArr[(i + (16 * 4)) % arraySize];
    }
    // read timestamp
    measureEnd = getTime(); // get end time
    getCPUID(); // force serialization

    // unit conversions from ns -> s and bytes -> GB
    double wallClockTime = cyclesToTime(measureInit, measureEnd) / pow(10, 9); // seconds
    double dataGB = (double)(CACHE_LINE_SIZE * 4 * (LOOP_COUNT / 64)) / pow(1000, 3); // GB

    return ((double) dataGB / (double) wallClockTime); // return GB/s
}

/**
 * Memory Operation 2b: RAM Bandwidth Write
 * 
 * @return double with bandwidth in GB/s
 */
// force optimizer to not vectorize loop instructions for this function
extern "C" __attribute__((optimize("no-tree-vectorize"))) double memory_bandwidthWrite(int size)
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
    for(int i = 0; i < LOOP_COUNT; i += 64)
    {
        // 4 writes to the array that are added to dummy var (1 instruction per cache line)
        genArr[(i + (16)) % arraySize] = 0;
        genArr[(i + (16 * 2)) % arraySize] = 0;
        genArr[(i + (16 * 3)) % arraySize] = 0;
        genArr[(i + (16 * 4)) % arraySize] = 0;
    }

    // read timestamp
    measureEnd = getTime(); // get end time
    getCPUID();

    // unit conversions from ns -> s and bytes -> GB
    double wallClockTime = cyclesToTime(measureInit, measureEnd) / pow(10, 9); //seconds
    double dataGB = (double)(CACHE_LINE_SIZE * 4 * (LOOP_COUNT / 64)) / pow(1000, 3); // GB

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

    // initialize file name, get fd, and adjust size of file
    const char* file = "dummy";
    int fd = get_fd(file);
    ftruncate(fd, DUMMY_SIZE);

    // perform memory mapping
    char* pageMap = (char *) mmap(nullptr, DUMMY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // map dummy file of DUMMY_SIZE to mem, can read and write mapping, no offset

    // conduct page fault timing
    for(int i = 0; i < PAGE_FAULT_LOOP_COUNT; i++)
    {
        // get timestamp
        getCPUID(); // force serialization
        measureInit = getTime(); // get start time

        pageMap[(i * PAGE_SIZE) % DUMMY_SIZE] = 0; // access page i (offset by size of page)

        // get timestamp
        measureEnd = getTime(); // force serialization
        getCPUID(); // get end time

        totTime += (double)(measureEnd - measureInit - 6); // add loop time to total time while adjusting for read overhead (from CPU ops)
    }

    totTime = totTime / 3; // convert cycles to ns
    double avgTime = totTime / PAGE_FAULT_LOOP_COUNT; // average over pages accessed

    // Clean up (unmap memory and close file descriptor)
    munmap(pageMap, DUMMY_SIZE);
    close(fd);

    return (double) avgTime; // return averaged time
}