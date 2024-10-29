#include "cpu_header.h"
/* 
* @file cpu.cpp
* @author Shayan Boghani
*
* CPU Module
 */

/*
* keep in mind warming caches and manual loop unrolling
*/

//using namespace std;

double measureReadOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit; // init in loop start time-stamp
    uint64_t measureEnd; // init in loop end time-stamp

    for (int i = 0; i < LOOP_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();
        measureEnd = getTime();
        getCPUID();

        total = total + (measureEnd - measureInit); // add current loop time to total
    }

    return ((double) total / double(LOOP_COUNT));
}

double measureLoopOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    getCPUID();
    measureInit = getTime(); // start measurement

    // loop which iterates for set number of times
    for (int i = 0; i < LOOP_COUNT; i++)
    {

    }

    measureEnd = getTime(); // end measurement
    getCPUID();

    total = total + (measureEnd - measureInit); // assign count to var

    return ((double) total / (double) LOOP_COUNT);
}

double* measureProcedureCallOverhead()
{
    static double retArr[8]; //init array to store all info

    // init in loop vals for time storage
    uint64_t measureInit;
    uint64_t measureEnd;
    uint64_t loop_total = 0;

    // zero variable procedure
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        zeroVars();

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[0] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // one variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        oneVars(0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[1] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // two variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        twoVars(0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[2] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // three variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        threeVars(0, 0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[3] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // four variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        fourVars(0, 0, 0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[4] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // five variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        fiveVars(0, 0, 0, 0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[5] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // six variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        sixVars(0, 0, 0, 0, 0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[6] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr

    // seven variable procedure
    loop_total = 0; // reset loop total
    for(int i = 0; i < LOOP_COUNT; i++){
        getCPUID();
        measureInit = getTime();

        sevenVars(0, 0, 0, 0, 0, 0, 0);

        measureEnd = getTime();
        getCPUID();

        loop_total = loop_total + (measureEnd - measureInit);
    }
    retArr[7] = ((double) loop_total / (double) LOOP_COUNT); // add time average to retArr
    
    return retArr;
}

double measureSystemCallOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    // loop which iterates for set number of times
    for (int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime(); // start measurement

        getuid();

        measureEnd = getTime(); // end measurement
        getCPUID();

        total = total + (measureEnd - measureInit); // add count to total
    }

    return ((double) total / (double) CALL_COUNT);
}

double measureProcessCreationOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    pid_t procID; // init process ID

    for(int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();

        procID = fork();

        if (procID == 0)
        {
            exit(1);
        } else 
        {
            wait(0);
            measureEnd = getTime();
            getCPUID();

            total = total + (measureEnd - measureInit);
        }
    }

    return ((double) total / (double) CALL_COUNT);
}

double kernelThreadCreationOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    pthread_t threadID; // init thread ID

    for(int i = 0; i < CALL_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();

        pthread_create(&threadID, NULL, &arbFunc, NULL);

        measureEnd = getTime();
        getCPUID();

        total = total + (measureEnd - measureInit);
    }

    return ((double) total / (double) CALL_COUNT);
}

double stddev_calc(double* array, double mean)
{
    double total_diff = 0;
    for(int i = 0; i < CALL_COUNT; i++)
    {
        total_diff = total_diff + pow((array[i] - mean), 2);
    }

    return sqrt((double) total_diff / (double) CALL_COUNT);
}

double processContextSwitch()
{
    int fd[2]; // initialize array of two file descriptors
    pipe(fd); // open pipe

    static double valArr[CALL_COUNT]; // init total overhead
    double total = 0;

    for(int i = 0; i < CALL_COUNT;)
    {
        uint64_t measureInit, measureEnd; // init timestamp vars
        pid_t procID; // init process ID
        int sizeVal = sizeof(uint64_t); // init size val for read

        procID = fork(); // fork process
        
        // conditions to run in child versus parent
        if (procID != 0) // parent process
        {
            getCPUID();
            measureInit = getTime();

            wait(NULL);
            read(fd[0], (void*) &measureEnd, sizeVal);
        } 
        else // child process
        {
            measureEnd = getTime();
            getCPUID();

            write(fd[1], (void*) &measureEnd, sizeVal); // write(file descriptor, buffer, size)
            exit(1); // exit 
        }

        double loopTotal = (measureEnd > measureInit) ? (measureEnd - measureInit) : 0; // if diff > 0 assign val otherwise assign 0

        // if val not 0 add to total and increment
        if (loopTotal != 0)
        {
            valArr[i] = loopTotal; // assign ith context switch measurement to array
            total = total + loopTotal;
            i++; // increment loop counter
        }
    }

    return ((double) total / (double) CALL_COUNT); // return array of context switch values
}

double threadContextSwitch()
{
    static double retArr[CALL_COUNT]; // init return array with overhead array
    double total = 0;
    for(int i = 0; i < CALL_COUNT;)
    {
        uint64_t measureInit, measureEnd; //init timestamp vars
        int sizeVal = sizeof(uint64_t); // init size value to pass to read()

        pthread_t threadID; // init thread

        pipe(fd); // open pipe
        pthread_create(&threadID, NULL, threadCSFunc, NULL); // create thread

        getCPUID(); // blocking
        measureInit = getTime(); // get start time
        pthread_join(threadID, NULL); // join thread
        read(fd[0], (void*) &measureEnd, sizeVal); // read(file descriptor, buffer, size)

        double loopTotal = (measureEnd > measureInit) ? (measureEnd - measureInit) : 0; // assign diff of timestamp, if < 0 assign 0

        // check if current loop output is valid and increment if it is (diff not equal to 0 meaning > 0)
        if (loopTotal != 0)
        {
            retArr[i] = loopTotal; // assign loop val to array index i
            total = total + loopTotal;
            i++; // increment loop counter
        }
    }

    return ((double) total / (double) CALL_COUNT); // return the array of context switch values
}
