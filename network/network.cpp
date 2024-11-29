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


/**
 * Network Operations 1: Server-Side
 * @param port the port to listen on
*/
void network_RTT_server(int port)
{
    /*
    - Init socket
    - Apply server internet address settings
    - Bind
    - Listen
    - Accept
    - in loop
        - Send/recv
    */

    int socketFD;
    struct sockaddr_in server, client;
    char dummyMsg;

    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // handle error socketFD == -1

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int bindStatus = bind(socketFD, (struct sockaddr *) &server, sizeof(server));
    // handle bindStatus != 0

    int listenStatus = listen(socketFD, 5);
    // handle listenStatus != 0

    int connectionFD = accept(socketFD, (struct sockaddr *) &client, sizeof(client));
    // handle connectionFD == -1

    while(true)
    {
        recv(connectionFD, dummyMsg, sizeof(dummyMsg), 0);
        // handle receive error?
        send(connectionFD, dummyMsg, len(dummyMsg), 0);
        // handle send error?
    }

    close(socketFD);
}

/**
 * Network Operations 1: Client-Side
 * @param connection the IP address to connect to
 * @param port the port to listen on
*/
double network_RTT_client(string connection, int port)
{
    /*
    - Init socket
    - Set server address information
    - Connect
    - in loop
        - Send/recv
    */

    int socketFD;
    struct sockaddr_in server, client;
    char dummyMsg = 'a';
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;


    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // handle error socketFD == -1
    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;   
    inetpton(AF_INET, (PCWSTR) connection, &server.sin_addr.s_addr);
    // handle error?
    server.sin_port = htons(port);

    int connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server));
    // handle connectionStatus error

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();

        send(socketFD, &dummyMsg, len(dummyMsg), 0);
        // handle send error?
        recv(socketFD, &dummyMsg, len(dummyMsg), 0);
        // handle receive error?

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
    }

    close(socketFD);

    return ((double) totalTime / (double) LOOP_COUNT);
}

/**
 * Network Operation 2: Server Side
 * @return double bandwidth value averaged over runs
 */
void network_peakBW_server(int port)
{
    /*
    - init socket
    - set server address info
    - bind
    - listen
    - accept
    - in loop
        - recv
    */

    int socketFD;
    struct sockaddr_in server, client;
    char* recMsg = new char[BANDWIDTH_BENCHMARK_SIZE];
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    struct sockaddr_in server, client;

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;   
    sever.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // handle error socketFD == -1

    int bindStatus = bind(socketFD, (struct sockaddr *) &server, sizeof(server));
    // handle bindStatus != 0

    int listenStatus = listen(socketFD, 5);
    // handle listenStatus != 0

    int connectionFD = accept(socketFD, (struct sockaddr *) &client, sizeof(client));
    // if using multithreading need to accept multiple
    // handle connectionFD == -1

    while(true)
    {
        recv(connectionFD, recMsg, BANDWIDTH_BENCHMARK_SIZE, MSG_WAITALL);
        // handle receive error?
    }

    close(socketFD);
}

/**
 * Network Operation 2: Client Side
 * @return double bandwidth value averaged over runs
 */
// multithread this if need to hit higher bandwidth
vector<int> network_peakBW_client(string connection, int port)
{
    /*
    - Init socket
    - Set server address info
    - connect
    - in loop
        - send
        - count bytes sent
        - measure time
    - bytes / time BW
    */
    int socketFD;
    struct sockaddr_in server, client;
    char* sendMsg = new char[BANDWIDTH_BENCHMARK_SIZE];
    memset(sendMsg, 'a', sizeof(sendMsg));

    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;
    uint64_t totalUploadBytes = 0;
    uint64_t totalDownloadBytes = 0;

    struct sockaddr_in server, client;

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;   
    inetpton(AF_INET, (PCWSTR) connection, &server.sin_addr.s_addr)
    // handle error?
    server.sin_port = htons(port);

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // handle error socketFD == -1

    int connectionFD = connect(socketFD, (struct sockaddr *) &server, sizeof(server))
    // handle connectionFD == -1

    for(int i = 0; i < BW_LOOP_COUNT; i++)
    {
        getCPUID();
        measureInit = getTime();

        totalUploadBytes += send(connectionFD, sendMsg, BANDWIDTH_BENCHMARK_SIZE, 0);
        // handle receive error?

        measureEnd = getTime();
        getCPUID();

        totalUploadTime += cyclesToTime(measureInit, measureEnd);

        // Fix to measure receive separately
        getCPUID();
        measureInit = getTime();

        totalDownloadBytes += recv(connectionFD, sendMsg, BANDWIDTH_BENCHMARK_SIZE, 0);
        // handle receive error?

        measureEnd = getTime();
        getCPUID();

        totalDownloadTime += cyclesToTime(measureInit, measureEnd);
    }

    close(socketFD);

    // bytes to MB for download and upload
    totalDownloadBytes = totalDownloadBytes / pow(1000, 2); // bytes to MB
    totalUploadBytes = totalUploadBytes / pow(1000, 2); // bytes to MB

    // ns to seconds for download and upload time
    totalDownloadTime = totalDownloadTime / pow(1000, 3); // ns to seconds
    totalUploadTime = totalUploadTime / pow(1000, 3); // ns to seconds

    // calculate bandwidths for upload and download
    double uploadBW = (double) totalUploadBytes / (double) totalUploadTime;
    double downloadBW = (double) totalDownloadBytes / (double) totalDownloadTime;

    // place values in vector
    vector<double> bandwidths = {uploadBW, downloadBW};

    return bandwidths; // return upload and download peak bandwidths
}


/**
 * Measure socket setup overhead
 * @param connection the IP address to connect to
 * @param port the server port
 * @return double time value averaged over runs
 */
double network_connectionOverhead_setup(string connection, int port)
{
    /*
    - init socket
    - set server address info
    - in loop
        - init socket
        - connect
        - close
        - repeat
    */
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    inetpton(AF_INET, (PCWSTR) connection, &server.sin_addr.s_addr)
    // handle error?
    server.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        int socketFD = socket(PF_INET, SOCK_STREAM, 0);

        getCPUID();
        measureInit = getTime();

        connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server));

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
        close(socketFD);
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
    /*
    - Init server address info
    - In for loop
        - init socket
        - connect
        - set socket behavior (linger)
        - close
        - repeat
    */
    int socketFD;
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;

    struct sockaddr_in server;

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    inetpton(AF_INET, (PCWSTR) connection, &server.sin_addr.s_addr)
    // handle error?
    server.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        socketFD = socket(PF_INET, SOCK_STREAM, 0);

        connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server));

        struct linger settings = {1, 10};
        setsockopt(socketFD, SOL_SOCKET, SO_LINGER, &settings, sizeof(settings));

        getCPUID();
        measureInit = getTime();

        close(socketFD);

        measureEnd = getTime();
        getCPUID();

        totalTime += cyclesToTime(measureInit, measureEnd);
    }

    return ((double) totalTime / (double) LOOP_COUNT);
}