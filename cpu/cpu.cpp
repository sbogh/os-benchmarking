#include "cpu_header.h"
/* 
* @file cpu.cpp
* @author Shayan Boghani
*
* CPU Module
 */

/**
 * CPU Operation 1a-1: Measure overhead of reading time
 * 
 * @return double with average time per loop
 */
double measureReadOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit; // init in loop start time-stamp
    uint64_t measureEnd; // init in loop end time-stamp

    for (int i = 0; i < LOOP_COUNT; i++)
    {
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time
        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        total += cyclesToTime(measureInit, measureEnd); // add current loop time to total (ns)
    }

    return ((double) total / double(LOOP_COUNT)); // average time per loop (ns)
}

/**
 * CPU Operation 1a-2: Measure Overhead of Using a Loop
 * 
 * @return double with average time per loop
 */
double measureLoopOverhead()
{
    double total; // total overhead ticks

    uint64_t measureInit; // init start time var
    uint64_t measureEnd; // init end time var

    getCPUID(); // implement serialization
    measureInit = getTime(); // read start time

    // loop which iterates for set number of times
    for (int i = 0; i < LOOP_COUNT; i++)
    {

    }

    measureEnd = getTime(); // read end time
    getCPUID(); // implement serialization

    total = cyclesToTime(measureInit, measureEnd); // add curr (ns)

    return ((double) total / (double) LOOP_COUNT); // return the average time per loop (ns)
}

/**
 * CPU Operation 1b: Procedure Call Overhead
 * 
 * @return array of doubles with average time per loop for functions with 0-7 vars
 */
double* measureProcedureCallOverhead()
{
    static double retArr[8]; //init array to store all info

    uint64_t measureInit; // init start time var
    uint64_t measureEnd; // init end time var
    uint64_t loop_total = 0; // init total time for loop

    // zero variable procedure
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        zeroVars(); // zero variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop time to total (ns)
    }
    retArr[0] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // one variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        oneVars(0); // one variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop time to total (ns)
    }
    retArr[1] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // two variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        twoVars(0, 0); // two variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop time to total (ns)
    }
    retArr[2] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // three variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        threeVars(0, 0, 0); // three variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop time to total (ns)
    }
    retArr[3] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // four variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        fourVars(0, 0, 0, 0); // four variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop total to total (ns)
    }
    retArr[4] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // five variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        fiveVars(0, 0, 0, 0, 0); // five variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop total to total (ns)
    }
    retArr[5] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // six variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        sixVars(0, 0, 0, 0, 0, 0); // six variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop total to total (ns)
    }
    retArr[6] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)

    // seven variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        sevenVars(0, 0, 0, 0, 0, 0, 0); // seven variable function

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        loop_total += cyclesToTime(measureInit, measureEnd); // add loop total to total (ns)
    }
    retArr[7] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr (ns)
    
    return retArr; // return array of average time (ns) per loop for functions with 0-7 variables
}

/**
 * CPU Operation 1c: System Call Overhead
 * 
 * @return double with average time per loop
 */
double measureSystemCallOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit; // init start time var
    uint64_t measureEnd; // init end time var

    // loop which iterates for set number of times
    for (int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        getuid(); // sys call to get user ID associated with process

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        total += cyclesToTime(measureInit, measureEnd); // add count to total (ns)
    }

    return ((double) total / (double) CALL_COUNT); // return average time per loop (ns)
}

/**
 * CPU Operation 1d-1: Task Creation Overhead
 * 
 * @return double with average time per loop
 */
double measureProcessCreationOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    pid_t procID; // init process ID

    for(int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        procID = fork(); // call fork and create child

        // not checking if child fails to be created (fork < 0)

        if (procID == 0) // if process is child
        {
            exit(1); // exit to return to parent
        } else // if parent
        {
            wait(0); // wait call
            measureEnd = getTime(); // read end time
            getCPUID(); // implement serialization

            total += cyclesToTime(measureInit, measureEnd); // add loop time to total (ns)
        }
    }

    return ((double) total / (double) CALL_COUNT); // return average time per loop (ns)
}

/**
 * CPU Operation 1d-2: Task Creation Time
 * 
 * @return Double average time per loop
 */
double kernelThreadCreationOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit; // init start time var
    uint64_t measureEnd; // init end time var

    pthread_t threadID; // init thread ID

    for(int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID(); // implement serialization
        measureInit = getTime(); // read start time

        pthread_create(&threadID, NULL, &arbFunc, NULL); // create new thread given thread and routine (function)

        measureEnd = getTime(); // read end time
        getCPUID(); // implement serialization

        total += cyclesToTime(measureInit, measureEnd); // add loop time to total time (ns)
    }

    return ((double) total / (double) CALL_COUNT); // return average time per loop (ns)
}

/*
double stddev_calc(double* array, double mean)
{
    double total_diff = 0; // init total difference between mean and individual times
    for(int i = 0; i < CALL_COUNT; i++)
    {
        total_diff = total_diff + pow((array[i] - mean), 2); // sum over diff squared
    }

    return sqrt((double) total_diff / (double) CALL_COUNT); // return std dev
}
*/

/**
 * CPU Operation 1e-1: Process Context Switch
 * 
 * @return double average time per loop
 */
double processContextSwitch()
{
    static double valArr[CALL_COUNT]; // init total overhead
    double total = 0; // init total var

    for(int i = 0; i < CALL_COUNT;)
    {
        uint64_t measureInit, measureEnd; // init timestamp vars
        pid_t procID; // init process ID
        int sizeVal = sizeof(uint64_t); // init size val for read

        int fd[2]; // initialize array of two file descriptors
        pipe(fd); // open pipe

        procID = fork(); // fork process

        // not checking if child fails to be made
        
        // conditions to run in child versus parent
        if (procID != 0) // parent process
        {
            getCPUID(); // implement serialization
            measureInit = getTime(); // read start time

            wait(0); // wait
            read(fd[0], (void*) &measureEnd, sizeVal); // read call using file descriptor, buffer, and size
        } 
        else // child process
        {
            measureEnd = getTime(); // read end time
            getCPUID(); // implement serialization

            write(fd[1], (void*) &measureEnd, sizeVal); // write call using file descriptor, buffer, and size
            exit(1); // exit call
        }

        double loopTotal = (measureEnd > measureInit) ? cyclesToTime(measureInit, measureEnd) : 0; // if diff > 0 assign val otherwise assign 0

        // if val not 0 add to total and increment
        if (loopTotal != 0)
        {
            valArr[i] = loopTotal; // assign ith context switch measurement to array
            total = total + loopTotal; // add loop total to total
            i++; // increment loop counter
        }
    }

    return ((double) total / (double) CALL_COUNT); // return average time per loop (ns)
}

/**
 * CPU Operation 1e-2: Thread Context Switch
 * 
 * @return double average time per loop
 */
double threadContextSwitch()
{
    static double retArr[CALL_COUNT]; // init return array with overhead array
    double total = 0; // init total
    
    for(int i = 0; i < CALL_COUNT; i++)
    {
        uint64_t measureInit, measureEnd; //init timestamp vars
        int sizeVal = sizeof(uint64_t); // init size value to pass to read()

        pthread_t threadID; // init thread

        pipe(fd); // open pipe

        ready = 0; // reset condition var so child cannot be started early

        pthread_create(&threadID, NULL, threadCSFunc, NULL); // create thread given thread and routine

        pthread_mutex_lock(&mutex); // lock

        getCPUID(); // implement serialization
        measureInit = getTime(); // get start time

        ready = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex); // unlock

        pthread_join(threadID, NULL); // join thread
        read(fd[0], (void*) &measureEnd, sizeVal); // read call given file descriptor, buffer, and size

       total += cyclesToTime(measureInit, measureEnd);
    }

    return ((double) total / (double) CALL_COUNT); // return the array of context switch values (ns)
}