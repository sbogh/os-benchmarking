/* 
* @file file-system_header.h
* @author Shayan Boghani
*
* File System Header
 */

#ifndef FILESYSTEM_HEADER_H
#define FILESYSTEM_HEADER_H

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
#include <algorithm>
#include <fcntl.h>
#include <thread>
#include <chrono>

#pragma intrinsic(__rdtsc)
using namespace std;

#define LOOP_COUNT 1000000 // num loops
#define CACHE_LINE 64 // cache line size
#define BUFFER_SIZE 1024*1024
vector<long int> cacheFileSizes = {128000000, 256000000, 512000000, 1024000000, 1500000000, 2000000000, 3000000000}; // create files of size 128 MB to 3 GB (ideally at 2048 MB (RAM size) should see jump)
vector<string> cacheFileNames = {"file128.txt", "file256.txt", "file512.txt", "file1000.txt", "file1500.txt", "file2000.txt", "file3000.txt"};
vector<string> remoteCacheFileNames = {"../../../../../../mnt/remoteAccess/file128.txt", "../../../../../../mnt/remoteAccess/file256.txt", "../../../../../../mnt/remoteAccess/file512.txt", "../../../../../../mnt/remoteAccess/file1000.txt", "../../../../../../mnt/remoteAccess/file1500.txt", "../../../../../../mnt/remoteAccess/file2000.txt", "../../../../../../mnt/remoteAccess/file3000.txt"};
#define BLOCK 4096

vector<long int> readFileSizes = {2000000, 4000000, 8000000, 16000000, 32000000, 64000000};
vector<string> readFileNames = {"file2.txt", "file4.txt", "file8.txt", "file16.txt", "file32.txt", "file64.txt"};
vector<string> remoteFileNames = {"../../../../../../mnt/remoteAccess/file2.txt", "../../../../../../mnt/remoteAccess/file4.txt", "../../../../../../mnt/remoteAccess/file8.txt", "../../../../../../mnt/remoteAccess/file16.txt", "../../../../../../mnt/remoteAccess/file32.txt", "../../../../../../mnt/remoteAccess/file64.txt"};

string localPath = "file1000.txt"; // file for contention
string localPath_child = "file1000_child.txt"; // file for contention children


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
 * Create a file of a size
 * 
 * @return Unsigned 64-bit int of cycles
 */
void fs_createFile(string filename, size_t size)
{
    ofstream file(filename, std::ios::binary);

    vector<char> buffer(BUFFER_SIZE, 'a');

    size_t totalSize = size;

    while(totalSize > 0)
    {
        size_t batch = std::min((size_t) BUFFER_SIZE, totalSize);
        file.write(buffer.data(), batch);
        totalSize -= batch;
    }

    file.close();
}

#endif