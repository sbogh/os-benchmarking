/*
CPU Header File
*/
#include <cstdlib>
#include <cstdint>
#include <intrin.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#pragma intrinsic(__rdtsc)

#define LOOP_COUNT = 100000
#define CALL_COUNT = 250


void getCPUInfo()
{
    // get CPU Info
    system("lscpu");
}

static inline void getCPUID()
{
    // inline assembly to barrier for calls
    __asm__ __volatile__ ("CPUID\n\t")
}

static inline uint64_t getTime()
{ 
    // define high and low variable
    uint32_t cycles_high, cycles_low;

    // inline assembly to pull cycle vals
    __asm__ __volatile__ ("rdtsc" : "=a" (cycles_low), "=d" (cycles_high));

    // return cycle count
    return ((uint64_t)cycles_high << 32) | cycles_low;
}

static inline void zeroVars(){}

static inline void oneVars(int one){}

static inline void twoVars(int one, int two){}

static inline void threeVars(int one, int two, int three){}

static inline void fourVars(int one, int two, int three, int four){}

static inline void fiveVars(int one, int two, int three, int four, int five){}

static inline void sixVars(int one, int two, int three, int four, int five, int six){}

static inline void sevenVars(int one, int two, int three, int four, int five, int six, int seven){}

void* arbFunc(void* arg)
{
    pthread_exit(NULL);
}