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
    - Results
        - Present as graph with the x-axis as the log of the size of the memory 
        region accessed, and the y-axis as the average latency
    - Hint
        - Use imbench "back-to-back-load" latency measurement as reference
- RAM bandwidth
    - Reading
    - Writing

    - Hint
        - Use loop unrolling
        - Imbench: keep in mind the effects of cache line prefetching
        - keep in mind that RAM bandwidth is likely limited by the DRAM and not the memory bus bandwidth
- Page Fault Service Time
    - Report the time for faulting an entire page from disk (mmap is one useful mechanism)
    - Compare to latency of accessing a byte from main memory

*/