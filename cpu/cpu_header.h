/* 
* @file cpu.h
* @author Shayan Boghani
*
* CPU Header
 */

#ifndef CPU_HEADER_H
#define CPU_HEADER_H

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

#pragma intrinsic(__rdtsc)

#define LOOP_COUNT 1000000 // loop count variable for simple overheads
#define CALL_COUNT 250 // call count variable for process and threads

static int fd[2]; // global file descriptor

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

// functions that take 0-7 variable inputs
static inline void zeroVars(){}

static inline void oneVars(int one){}

static inline void twoVars(int one, int two){}

static inline void threeVars(int one, int two, int three){}

static inline void fourVars(int one, int two, int three, int four){}

static inline void fiveVars(int one, int two, int three, int four, int five){}

static inline void sixVars(int one, int two, int three, int four, int five, int six){}

static inline void sevenVars(int one, int two, int three, int four, int five, int six, int seven){}

/**
 * Dummy function for thread creation benchmarking
 */
static inline void* arbFunc(void* arg)
{
    pthread_exit(NULL); // exit thread
}

/**
 * Dummy function for thread context switch benchmarking
 */
static inline void *threadCSFunc(void *)
{
    uint64_t measureInit; // init start variable

    getCPUID(); // implement serialization
    measureInit = getTime(); // read start time
    int sizeVal = sizeof(uint64_t); // init and assign value for size

    write(fd[1], (void*) &measureInit, sizeVal); // write call given file descriptor, buffer, and size

    pthread_exit(NULL); // exit thread
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

    for(int i = 0: i < data.size(); i++)
    {
        sum += data[i];
    }

    mean = sum / data.size();

    for(int i = 0; i < data.size(); i++)
    {
        stddev += pow(arr[i] - mean, 2);
    }

    return sqrt(stddev / data.size());
}

#endif