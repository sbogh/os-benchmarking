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

    // initialize socket, server, client, and dummy message
    int socketFD, client_len;
    struct sockaddr_in server, client;
    char dummyMsg;

    client_len = sizeof(struct sockaddr_in); // init client length

    // init time measurement vars
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    // initialize socket FD
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // fill server address with null vals
    bzero(&server, sizeof(server));

    // initialize server info
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket to server
    int bindStatus = bind(socketFD, (struct sockaddr *) &server, sizeof(server));

    // listen on socket
    int listenStatus = listen(socketFD, 5);

    // accept requests
    int connectionFD = accept(socketFD, (struct sockaddr *) &client, (socklen_t *) &client_len);

    while(true)
    {
        recv(connectionFD, &dummyMsg, sizeof(dummyMsg), 0); // receive from connection dummy msg
        send(connectionFD, &dummyMsg, sizeof(dummyMsg), 0); // echo back dummy msg
    }

    close(socketFD); // close socket
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

    // initialize socket, server, client, and dummy message
    int socketFD;
    struct sockaddr_in server, client;
    char dummyMsg = 'a';
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    // init socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // fill server addr with null vals
    bzero(&server, sizeof(server));

    // initialize server info
    server.sin_family = AF_INET;   
    inet_pton(AF_INET, connection.c_str(), &server.sin_addr.s_addr);
    server.sin_port = htons(port);

    int connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server));

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        // get start time
        getCPUID(); // serialization
        measureInit = getTime(); // get start time

        send(socketFD, &dummyMsg, sizeof(dummyMsg), 0); // send packet
        recv(socketFD, &dummyMsg, sizeof(dummyMsg), 0); // receive echoed packet back

        // get end time
        measureEnd = getTime(); // get end time
        getCPUID(); // serialization

        totalTime += cyclesToTime(measureInit, measureEnd); // convert time from cycles to ns and add to total time
    }

    close(socketFD); // close socket

    return ((double) totalTime / (double) LOOP_COUNT); // return averaged time
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

    // initialize socket, server, client, and dummy message
    int socketFD;
    struct sockaddr_in server, client;
    char* recMsg = new char[BANDWIDTH_BENCHMARK_SIZE];
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    // fill server addr with null vals
    bzero(&server, sizeof(server));

    // init server info
    server.sin_family = AF_INET;   
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // init socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // bind socket
    int bindStatus = bind(socketFD, (struct sockaddr *) &server, sizeof(server));

    // listen for connections
    int listenStatus = listen(socketFD, 5);

    // accept connections
    int connectionFD = accept(socketFD, (struct sockaddr *) &client, (socklen_t *) sizeof(client));

    while(true)
    {
        //recv(socketFD, &recMsg, BANDWIDTH_BENCHMARK_SIZE, MSG_WAITALL);
        read(connectionFD, &recMsg, BANDWIDTH_BENCHMARK_SIZE); // receive packets
    }

    close(socketFD); // close socket
}

/**
 * Network Operation 2: Client Side
 * @return double bandwidth value averaged over runs
 */
double network_peakBW_client(string connection, int port)
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
    // initialize socket, server, client, and send msg and set val of msg
    int socketFD;
    struct sockaddr_in server, client;
    char* sendMsg = new char[BANDWIDTH_BENCHMARK_SIZE];
    memset(sendMsg, 'a', sizeof(sendMsg));

    // initialize time measurement vars
    uint64_t measureInit, measureEnd;
    uint64_t totalUploadTime = 0;
    uint64_t totalUploadBytes = 0;

    // set server addr to null vals
    bzero(&server, sizeof(server));

    // init server info
    server.sin_family = AF_INET;   
    inet_pton(AF_INET, connection.c_str(), &server.sin_addr.s_addr);
    server.sin_port = htons(port);

    // init socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // init connection
    int connectionFD = connect(socketFD, (struct sockaddr *) &server, sizeof(server));

    for(int i = 0; i < BW_LOOP_COUNT; i++)
    {
        // get start time
        getCPUID(); // serialization
        measureInit = getTime(); // get start time

        //totalUploadBytes += send(socketFD, &sendMsg, BANDWIDTH_BENCHMARK_SIZE, 0);
        totalUploadBytes += write(connectionFD, &sendMsg, BANDWIDTH_BENCHMARK_SIZE);

        // get end time
        measureEnd = getTime(); // get end time
        getCPUID(); // serialization

        totalUploadTime += cyclesToTime(measureInit, measureEnd); // convert time to ns and append to total time
    }

    close(socketFD); // close socket

    // bytes to MB for download and upload
    totalUploadBytes = totalUploadBytes / pow(1000, 2); // bytes to MB

    // ns to seconds for download and upload time
    totalUploadTime = totalUploadTime / pow(1000, 3); // ns to seconds

    // calculate bandwidths for upload and download
    double uploadBW = (double) totalUploadBytes / (double) totalUploadTime;

    return uploadBW; // return upload and download peak bandwidths
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

    // init time vars, connection FD, server
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;
    struct sockaddr_in server;

    // set server address to null vals
    bzero(&server, sizeof(server));

    // init server info
    server.sin_family = AF_INET;
    inet_pton(AF_INET, connection.c_str(), &server.sin_addr.s_addr);
    server.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        int socketFD = socket(PF_INET, SOCK_STREAM, 0); // open socket

        // start time
        getCPUID(); // serialization
        measureInit = getTime(); // get start time

        connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server)); // connect to server

        // end time
        measureEnd = getTime(); // get end time
        getCPUID(); // serialization

        totalTime += cyclesToTime(measureInit, measureEnd); // convert time to ns and add to total time
        close(socketFD); // close socket
    }

    return ((double) totalTime / (double) LOOP_COUNT); // return averaged time (ns)
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
        - close
        - repeat
    */
    // init socket, time vars, server
    int socketFD;
    uint64_t measureInit, measureEnd;
    int connectionStatus;
    uint64_t totalTime = 0;
    struct sockaddr_in server;

    char* sendMsg = new char[BANDWIDTH_BENCHMARK_SIZE]; // init dummy msg

    // fill server addr with null vals
    bzero(&server, sizeof(server));

    // init server info
    server.sin_family = AF_INET;
    inet_pton(AF_INET, connection.c_str(), &server.sin_addr.s_addr);
    server.sin_port = htons(port);

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        socketFD = socket(PF_INET, SOCK_STREAM, 0); // open socket

        connectionStatus = connect(socketFD, (struct sockaddr *) &server, sizeof(server)); // connect

        send(socketFD, &sendMsg, BANDWIDTH_BENCHMARK_SIZE, 0); // ensure that connection is established

        // start time
        getCPUID(); // serialization
        measureInit = getTime(); // get start time

        close(socketFD); // close socket

        // end time
        measureEnd = getTime(); // get end time
        getCPUID(); // serialization

        totalTime += cyclesToTime(measureInit, measureEnd); // convert time to ns and add to total time
    }

    return ((double) totalTime / (double) LOOP_COUNT); // return averaged time
}

/**
 * Network Operations 3: Server-Side
 * Use for set up and teardown on the server side
 * @param port the port to listen on
*/
void network_connectionOverhead_server(int port)
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

    // init socket, server, client, dummy msg
    int socketFD;
    struct sockaddr_in server, client;
    char dummyMsg;

    // init time vars
    uint64_t measureInit, measureEnd;
    uint64_t totalTime = 0;

    // open socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // fill server addr with null vals
    bzero(&server, sizeof(server));

    // init server info
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket
    int bindStatus = bind(socketFD, (struct sockaddr *) &server, sizeof(server));

    // listen for connections
    int listenStatus = listen(socketFD, 5);

    while(true)
    {
        // accept connections
        int connectionFD = accept(socketFD, (struct sockaddr *) &client, (socklen_t *) sizeof(client));
    }

    close(socketFD); // close socket
}