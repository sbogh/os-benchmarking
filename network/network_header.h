/* 
* @file network_header.h
* @author Shayan Boghani
*
* Network Header
 */

#ifndef NETWORK_HEADER_H
#define NETWORK_HEADER_H

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
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#pragma intrinsic(__rdtsc)
using namespace std;

#define LOOP_COUNT 1000000 // num loops
#define BW_LOOP_COUNT 1000 // num loops for bandwidth calc
#define BANDWIDTH_BENCHMARK_SIZE 1000000 // 1000000 bytes message
#define SERVER_IP "192.168.1.219" // server VM IPv4
#define PORT 8080 // chosen port


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
 * Convert clock cycle measurements to nanoseconds (aka wall clock measurements)
 * 
 * @return Unsigned 64-bit int of cycles
 */
static double cyclesToTime(uint64_t measureInit, uint64_t measureEnd)
{
    return ((double) (measureEnd - measureInit) / 3); // divide by processor frequency
}

/**
 * Create socket
 * 
 * @return Unsigned 64-bit int of cycles
 */
static inline int createSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

/**
 * Assign socket fields
 * @return socketAddr struct
 */
static inline sockaddr_in assignSocketFields(sockaddr_in socket, string connection, string port)
{
    socket.sin_family = AF_INET;
    //socket.sin_addr.s_addr = inet_addr(connection);
    //socket.sin_port = htons(atoi(port));

    return socket;
}


#endif