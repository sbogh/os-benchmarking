/* 
* @file memory.h
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

#pragma intrinsic(__rdtsc)
#pragma GCC novector
using namespace std;

#define LOOP_COUNT = 1000000

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
    return ((double) (measureEnd - measureInit) / 3000);
}

#endif