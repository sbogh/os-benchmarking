/*
CPU Header File
*/
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

#pragma intrinsic(__rdtsc)

#define LOOP_COUNT 100000
#define CALL_COUNT 250

static int fd[2];


void getCPUInfo()
{
    // get CPU Info
    system("lscpu");
}

void getCPUID()
{
    __asm__ __volatile__ ("cpuid");
}

static inline uint64_t getTime()
{ 
    unsigned int ui;
    // return cycle count
    return __rdtscp(&ui);
}

static inline void zeroVars(){}

static inline void oneVars(int one){}

static inline void twoVars(int one, int two){}

static inline void threeVars(int one, int two, int three){}

static inline void fourVars(int one, int two, int three, int four){}

static inline void fiveVars(int one, int two, int three, int four, int five){}

static inline void sixVars(int one, int two, int three, int four, int five, int six){}

static inline void sevenVars(int one, int two, int three, int four, int five, int six, int seven){}

static inline void* arbFunc(void* arg)
{
    pthread_exit(NULL);
}

static inline void *threadCSFunc(void *)
{
    uint64_t measureInit;

    getCPUID();
    measureInit = getTime();
    int sizeVal = sizeof(uint64_t);

    write(fd[1], (void*) &measureInit, sizeVal);

    pthread_exit(NULL);
}
