#include "network_header.h"
/* 
* @file network.cpp
* @author Shayan Boghani
*
* Network Module
 */

/*
What needs to be done

- Round trip time
    - Compare application-level round trip time with the time to perform a ping using the ping command (ICMP requests are handled at kernel level)

    - Hint
        - You should implement the application-level benchmark yourself but you may use an existing ping utility for comparison.

- Peak bandwidth

- Connection overhead: Report setup and tear-down.

- Additional Details
    - Evaluate the above for the TCP protocol
    - For each quantity, compare both remote and loopback (localhost) interfaces

- Questions to keep in mind
    - Comparing the remote and loopback results, what can you deduce about baseline network performance and the overhead of OS software?
    - For both round trip time and bandwidth, how close to ideal hardware performance do you achieve?
    - What are reasons why the TCP performance does not match ideal hardware performance (e.g., what are the pertinent overheads)?
    - In describing your methodology for the remote case, either provide a machine description for the second machine (as below), or use two identical machines.
*/


void network_roundTripTime(int option, string remoteConnect, int port)
{

}

void helperRTT_server(string connection, int port)
{


}

void helperRTT_client(string connection, int port)
{

}

/**
 * Measure peak network bandwidth
 * @return double bandwidth value averaged over runs
 */
void network_peakBW()
{

}

/**
 * Measure socket setup overhead
 * @param connection the IP address to connect to
 * @param port the server port
 * @return double time value averaged over runs
 */
double network_connectionOverhead_setup(string connection, int port)
{
    int socketFD;
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;

    struct sockaddr_in connectServer;
    connectServer.sin_family = AF_INET;
    connectServer.sin_addr.s_addr = inet_addr(connection);
    connectServer.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        socketFD = socket(PF_INET, SOCK_STREAM, 0);

        getCPUID();
        measureInit = getTime();

        connectionStatus = connect(socketFD, (struct sockaddr *) &connect, sizeof(connect));

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);

        connectionStatus = close(socketFD);
    }

    return ((double) totalTime / (double) LOOP_COUNT);
}


/**
 * Measure socket teardown overhead
 * @param connection the IP address to connect to
 * @param port the server port
 * @return double time value averaged over runs
 */
double network_connectionOverhead_teardown(string connection, int port)
{
    int socketFD;
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;

    struct sockaddr_in connectServer;
    connectServer.sin_family = AF_INET;
    connectServer.sin_addr.s_addr = inet_addr(connection);
    connectServer.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        socketFD = socket(PF_INET, SOCK_STREAM, 0);
        connectionStatus = connect(socketFD, (struct sockaddr *) &connect, sizeof(connect));

        getCPUID();
        measureInit = getTime();

        connectionStatus = close(socketFD);

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
    }

    return ((double) totalTime / (double) LOOP_COUNT);
}