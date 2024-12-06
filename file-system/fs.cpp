#include "fs_header.h"
/* 
* @file file-system.cpp
* @author Shayan Boghani
*
* File System Module
 */

/*
What needs to be done

- Size of file cache
    - Note that the file cache size is determined by the OS and will be sensitive to other load on the machine
        - for an application accessing lots of file system data, an OS will use a notable fraction of main memory (GBs) for the file system cache

    Results:  
    - Report results as a graph whose x-axis is the size of the file being accessed and the y-axis 
    is the average read I/O time. Do not use a system call or utility program to determine this metric 
    except to sanity check.

- File read time
    - Report for both sequential and random access as a function of file size.
    - Ensure that you are not measuring cached data when reading.
    - The OS will cache file data by default when processes both read and write file data.
    - Fortunately, OSes will generally provide mechanisms for managing the cache.
        - programmatically, see the posix_fadvise system call
    - Report as a graph with a log/log plot with the x-axis the size of the file and y-axis the average per-block time.

- Remote file read time
    - Repeat the previous experiment for a remote file system.
    - What is the "network penalty" of accessing files over the network?
    - You can either configure your second machine to provide remote file access

- Contention
    - Report the average time to read one file system block of data as a function of the number of processes 
    simultaneously performing the same operation on different files on the same disk (and not in the file buffer cache).


Total Functions:
- Size of file cache
- File read time
    - sequential
    - random access
- Remote file read time
    - sequential
    - random access
- Contention
*/
using namespace std;

/**
 * File Systems Operation 1: File System Cache Size
 * @return a vector of doubles for the average times to access each file size
*/
vector<double> fs_cacheSize()
{
    vector<double> times;

    int counter = 0;
    for(auto size : cacheFileSizes) // for each file size
    {
        uint64_t measureInit, measureEnd;
        uint64_t totalTime = 0;

        string filename = fileNames[counter]

        fs_createFile(filename, size); // create file
        void* dummy = malloc(BLOCK); // dummy buffer

        int fd = open(filename, O_RDONLY | O_SYNC);

        lseek(fd, size - 1, SEEK_SET);
        int readBytes = 0;

        while(readBytes < size)
        {
            lseek(fd, -2 * BLOCK, SEEK_CUR);
            size_t readAmount = read(fd, dummy, BLOCK);
            readBytes += readAmount;
        }

        close(fd);

        fd = open(filename, O_RDONLY | O_SYNC);

        lseek(fd, size - 1, SEEK_SET);
        int readBytes = 0;

        while(readBytes < size)
        {
            lseek(fd, -2 * BLOCK, SEEK_CUR);
            // get start time
            getCPUID(); // serialization
            measureInit = getTime(); // start time

            size_t readAmount = read(fd, dummy, BLOCK);

            // get end time
            measureEnd = getTime(); // end time
            getCPUID(); // serialization

            readBytes += readAmount;
            totalTime += cyclesToTime(measureInit, measureEnd);
        }
        
        times.push_back(totalTime / (size / BLOCK));
        free(dummy);
        counter += 1;
    }

    return times;
}

double fs_readTime_sequential(string path, int size)
{
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;
    void * dummy = malloc(BLOCK);


    int fd = open(path, O_SYNC);
    fcntl(fd, F_NOCACHE, 1);

    while(read(fd, dummy, BLOCK) > 0)
    {
        getCPUID();
        measureInit = getTime();

        read(fd, dummy, BLOCK);

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
    }

    close(fd);

    return totalTime / (((double) size / BLOCK) * 1000); // return us
}

double fs_readTime_random(string path)
{
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;
    void * dummy = malloc(BLOCK);


    int fd = open(path, O_SYNC);
    fcntl(fd, F_NOCACHE, 1);

    int loopParam = size / BLOCK;

    for(int i = 0; i < loopParam; i++)
    {
        int randAcc = rand() % loopParam;

        getCPUID();
        measureInit = getTime();

        lseek(fd, randAcc * BLOCK, SEEK_SET);
        read(fd, dummy, BLOCK);

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
    }

    close(fd);

    return totalTime / (((double) size / BLOCK) * 1000); // return us
}

double fs_helper_create(string path, string child_path, int num)
{
    int procID = 1;
    void * dummy = malloc(BLOCK);
    uint64_t measureInit, measureEnd;

    for(int i = 0; i < num; i++)
    {
        procID = fork();

        if(procID == 0)
        {
            this_thread::sleep_for(chrono::nanoseconds(500));
            break;
        }
    }

    string fileLoc;
    if(procID == 0) // child
    {
        fileLoc = filename + "_child.txt";
    } else { // parent
        fileLoc = filename + ".txt";
    }

    int file = open(fileLoc.c_str(), O_DIRECT);

    getCPUID();
    measureInit = getTime();

    for(int j = 0; j < LOOP_COUNT; j++)
    {
        lseek(file, BLOCK, SEEK_CUR);
        read(file, dummy, BLOCK);
    }

    measureEnd = getTime();
    getCPUID();

    free(dummy);
    close(file);

    if(procID == 0)
    {
        exit(0);
    }

    return cyclesToTime(measureInit, measureEnd) / LOOP_COUNT;
}

double fs_contention(string path, string child_path)
{
    for(int i = 0; i < 40; i++) // number of processes
    {
        double total = 0;

        for(int j = 0; j < 10; j++) // number of runs
        {
            total += fs_helper_create(path, child_path, i);
        }

        total /= 10; // average over runs

        cout<<"# of Processes: " + to_string(i) + total + " sec/block"<<endl;
    }
}