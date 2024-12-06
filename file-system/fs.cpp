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
    vector<double> times; // list of run times

    int counter = 0;
    for(long int size : cacheFileSizes) // for each file size
    {
        // init time vars
        uint64_t measureInit, measureEnd;
        uint64_t totalTime = 0;

        string filename = fileNames[counter]; // get the filename

        //fs_createFile(filename, size); // create file
        void* dummy = malloc(BLOCK); // dummy buffer


        // Read file into cache first
        int fd = open(filename.c_str(), O_RDONLY | O_SYNC); // open file

        lseek(fd, size - 1, SEEK_SET); // pointer to the end of file
        long int readBytes = 0; // how many bytes have been read

        // while the entire file has not been read
        while(readBytes < size)
        {
            lseek(fd, -2 * BLOCK, SEEK_CUR); // step backwards from end of file
            long int readAmount = read(fd, dummy, BLOCK);
            readBytes += readAmount;
        }

        close(fd);

        
        // Measured run where the file is guaranteed to be in cache
        fd = open(filename.c_str(), O_RDONLY | O_SYNC); // open file

        lseek(fd, size - 1, SEEK_SET); // pointer to the end of file
        readBytes = 0; // how many bytes have been read

        // while the entire file has not been read
        while(readBytes < size)
        {
            lseek(fd, -2 * BLOCK, SEEK_CUR); // step backwards from end of file to start

            // get start time
            getCPUID(); // serialization
            measureInit = getTime(); // start time

            size_t readAmount = read(fd, dummy, BLOCK); // number of bytes read in

            // get end time
            measureEnd = getTime(); // end time
            getCPUID(); // serialization

            readBytes += readAmount; // add read bytes
            totalTime += cyclesToTime(measureInit, measureEnd); // add cycle run
        }
        
        times.push_back(totalTime / (size / BLOCK)); // add averaged time to vector
        free(dummy); // free buffer
        counter += 1; // increment counter for filename
    }

    return times; // return time vectors
}

/**
 * File Systems Operation 2-a: File System Sequential Read
 * @param path the path to file
 * @param size the size of file
 * @return a double for the average sequential access
*/
double fs_readTime_sequential(string path, long int size)
{
    // init time vars
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    void * dummy = malloc(BLOCK); // allocate buffer of one block size


    int fd = open(path.c_str(), O_SYNC); // open file
    posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED); // enable no caching

    // while the read is occurring
    while(read(fd, dummy, BLOCK) > 0)
    {
        // get start time
        getCPUID(); // serialization
        measureInit = getTime(); // get start time

        read(fd, dummy, BLOCK); // read data

        measureEnd = getTime(); // get end time
        getCPUID(); // serialization

        totalTime += cyclesToTime(measureInit, measureEnd); // add current loop to total
    }

    close(fd); // close file

    return totalTime / (((double) size / BLOCK) * 1000); // return us
}

/**
 * File Systems Operation 2-b: File System Random Read
 * @param path the path to file
 * @param size the size of file
 * @return a double for average random access
*/
double fs_readTime_random(string path, long int size)
{
    // init time vars
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    void * dummy = malloc(BLOCK); // allocate buffer

    int fd = open(path, O_SYNC); // open file
    posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED); // set no caching

    int loopParam = size / BLOCK; // loop param for the number of blocks to access

    for(int i = 0; i < loopParam; i++)
    {
        int randAcc = rand() % loopParam; // get random location

        // get start time
        getCPUID(); // serialization
        measureInit = getTime(); // start time

        lseek(fd, randAcc * BLOCK, SEEK_SET); // find location
        read(fd, dummy, BLOCK); // read

        // get end time
        measureEnd = getTime(); // end time
        getCPUID(); // serialization

        totalTime += cyclesToTime(measureInit, measureEnd); // add current loop time to total time
    }

    close(fd); // close file
    free(dummy); // free buffer

    return totalTime / (((double) size / BLOCK) * 1000); // return us
}

/**
 * File Systems Operation 2-c: File System Contention - HELPER
 * @param path the path to file
 * @param child_path the path to child process file
 * @param num the number of processes
 * @return a double for the average times for number of processes
*/
double fs_helper_create(string path, string child_path, int num)
{
    int procID = 1; // set pid
    void * dummy = malloc(BLOCK); // allocate buffer
    uint64_t measureInit, measureEnd; // init time vars

    // for the number of processes we want to make
    for(int i = 0; i < num; i++)
    {
        procID = fork(); // fork the process

        // if we are in the child
        if(procID == 0)
        {
            this_thread::sleep_for(chrono::nanoseconds(500));
            break;
        }
    }

    // get file location
    string fileLoc;
    if(procID == 0) // child
    {
        fileLoc = child_path;
    } else { // parent
        fileLoc = path;
    }

    int file = open(fileLoc.c_str(), O_DIRECT); // open file

    // get start time
    getCPUID(); // serialization
    measureInit = getTime(); // start time

    // read many times
    for(int j = 0; j < LOOP_COUNT; j++)
    {
        lseek(file, BLOCK, SEEK_CUR); // seek location
        read(file, dummy, BLOCK); // read from location
    }

    // get end time
    measureEnd = getTime(); // end time
    getCPUID(); // serialization

    free(dummy); // free buffer
    close(file); // close file

    // if child exit
    if(procID == 0)
    {
        exit(0);
    }

    return cyclesToTime(measureInit, measureEnd) / LOOP_COUNT; // return the loop count
}

/**
 * File Systems Operation 2-c: File System Contention
 * @param path the path to file
 * @param child_path the path to child process file
*/
void fs_contention(string path, string child_path)
{
    for(int i = 0; i < 40; i++) // number of processes
    {
        double total = 0; // total of time

        for(int j = 0; j < 10; j++) // number of runs
        {
            total += fs_helper_create(path, child_path, i); // add run to total time
        }

        total /= 10; // average over runs

        cout<<"# of Processes: " + to_string(i) + " " + to_string(total) + " sec/block"<<endl;
    }
}