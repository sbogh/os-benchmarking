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

double measureReadOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit; // init in loop start time-stamp
    uint64_t measureEnd; // init in loop end time-stamp

    for (int i = 0; i < LOOP_COUNT; i++)
    {
        getCPUID(); // implement barrier before read clock call
        measureInit = getTime();
        measureEnd = getTime();
        getCPUID(); // barrier after read clock call

        total = total + (measureEnd - measureInit); // add current loop time to total
    }

    return ((double) total / double(LOOP_COUNT))
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

    total = total + (measureEnd - measureStart); // assign count to var

    return ((double) total / (double) LOOP_COUNT)
}

double measureProcedureCallOverhead()
{
    double retArr[8]; //init array to store all info

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

    return ((double) total / (double) CALL_COUNT)
}

double measureProcessCreationOverhead()
{
    double total = 0; // total overhead ticks

    uint64_t measureInit;
    uint64_t measureEnd;

    pid_t procID; // init process ID

    for(i = 0; i < CALL_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();

        procID = fork();

        if (procID == 0)
        {
            exit();
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

        total = total + (measureEnd - measureStart);
    }

    return ((double) total / (double) CALL_COUNT);
}

/*
double processContextSwitch()
{

}

double threadContextSwitch()
{

}
*/
