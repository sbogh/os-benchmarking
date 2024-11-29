#include "file-system_header.h"
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

double fs_cacheSize()
{

}

double fs_localReadTime_sequential()
{

}

double fs_localReadTime_random()
{

}

double fs_remoteReadTime_sequential()
{

}

double fs_remoteReadTime_random()
{

}

double fs_contention()
{

}