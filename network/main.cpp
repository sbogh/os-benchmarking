#include "network.cpp"
/* 
* @file main.cpp
* @author Shayan Boghani
*
* Experiments for Network Module (main)
* This file can act as the client and server side run file for remote operations.
* For local operations this will act only as the SERVER side run file. See "main_localhost.cpp" for the
* CLIENT side file to run in a separate terminal.
 */

#define SERVER_IP "192.168.1.219" // server VM IPv4

using namespace std;
using json = nlohmann::json;

json network_results; // initialize json structure

void main_RTT(int option)
{
    if (option == 1) // client
    {
        double roundTripTimeAvg = network_RTT_client(SERVER_IP, PORT);
        cout<<"Round Trip Time - " + to_string(roundTripTimeAvg)<<endl;
    } else { // server
        network_RTT_server(PORT);
    }
}

void main_peakBW(int option)
{
    if(option == 1) // client
    {
        double bandwidth = network_peakBW_client(SERVER_IP, PORT);
        cout<<"Peak Bandwidth - " + to_string(bandwidth)<<endl;
    } else { // server
        network_peakBW_server(PORT);
    }
}

void main_connectionOverhead_setup(int option)
{
    if (option == 1) // client
    {
        double setUpAvg = network_connectionOverhead_setup(SERVER_IP, PORT);
        cout<<"Connection Set Up Overhead - " + to_string(setUpAvg)<<endl;
    } else { // server
        network_connectionOverhead_server(PORT);
    }
}

void main_connectionOverhead_teardown(int option)
{
    if (option == 1) // client
    {
        double tearDownAvg = network_connectionOverhead_teardown(SERVER_IP, PORT);
        cout<<"Connection Tear Down Overhead - " + to_string(tearDownAvg)<<endl;
    } else { // server
        network_connectionOverhead_server(PORT);
    }
}

int main()
{
    int option = 1; // change to 1 if client, 0 if server

    main_RTT(option);

    main_peakBW(option);

    main_connectionOverhead_setup(option);

    main_connectionOverhead_teardown(option); 
    
}