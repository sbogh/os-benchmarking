/* 
* @file memory_header.h
* @author Shayan Boghani
*
* Memory Header
 */

#ifndef MEMORY_HEADER_H
#define MEMORY_HEADER_H

#include <cstdlib>
#include <cstdint>
#include <x86intrin.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <iostream>
#include <wait.h>
#include "json.hpp"
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include <algorithm>
#include <random>

#pragma intrinsic(__rdtsc)
using namespace std;

#define LOOP_COUNT 1000000 // num loops
#define MEM_ACCESS_COUNT 1000 // num loops for memory access
#define PAGE_FAULT_LOOP_COUNT 10000 // num loops for page fault
#define CACHE_LINE_SIZE (size_t) sysconf(_SC_LEVEL1_DCACHE_LINESIZE) // size of cache line
#define PAGE_SIZE (size_t) sysconf (_SC_PAGESIZE) // get page size
const int DUMMY_SIZE = PAGE_SIZE * 512 * 1000; // file size


vector<int> sizes = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864}; // L1: 80 KiB = 81920 bytes, L2: 1.3 MiB = 1363148.8 bytes, L3: 12 MiB = 12582912 bytes
vector<int> bandwidth_sizes = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864}; // 2^10 to 2^26 bytes

// Node structure for Linked List for memory integer access latency benchmark
struct Node {
	int data; // data value for node
	Node* next = nullptr; // next pointer for node
};

/**
 * Get CPU ID to implement serialization
 */
void getCPUID()
{
    __asm__ __volatile__ ("cpuid"); // read CPUID to serialize
}

/**
 * Get cycle time stamp
 * 
 * @return Unsigned 64-bit int of cycles
 */
static inline uint64_t getTime()
{ 
    unsigned int ui; // dummy var to pass to rdtscp
    return __rdtscp(&ui); // return cycle count
}

/**
 * Return array of a size
 * 
 * @param size size of array
 * @return Unsigned 64-bit int of cycles
 */
static std::vector<int>& get_array(int size) 
{
    static std::vector<int> genArr(size);
    return genArr;
}

/**
 * Convert clock cycle measurements to nanoseconds (aka wall clock measurements)
 * 
 * @return Unsigned 64-bit int of cycles
 */
static double cyclesToTime(uint64_t measureInit, uint64_t measureEnd)
{
    return ((double) (measureEnd - measureInit) / 3); // divide by processor frequency
}

/**
 * Create file for mmap operation
 * 
 * @param file name of file
 * @return file descriptor int
 */
static int get_fd(const char* file)
{
    int fd = open(file, O_RDWR); // open file using filename
    if (fd < 0) {
        return -1;
    }

    return fd; // return file descriptor
}

/**
 * Calc std dev
 * 
 * @param data input vector<double>
 * @return double std dev value of vector
 */
static inline double calc_stddev(vector<double> data)
{
    double sum = 0.0;
    double mean;
    double stddev = 0.0;

    for(int i = 0; i < data.size(); i++)
    {
        sum += data[i];
    }

    mean = sum / data.size();

    for(int i = 0; i < data.size(); i++)
    {
        stddev += pow(data[i] - mean, 2);
    }

    return sqrt(stddev / data.size());
}

#endif