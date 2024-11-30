#include "network.cpp"
/* 
* @file main_localhost.cpp
* @author Shayan Boghani
*
* Experiments for Network Module (main)
* This file will be used to run the localhost CLIENT to do that set of benchmarks
 */

#define SERVER_IP "127.0.0.1" // define localhost IP

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
        vector<double> bandwidths = network_peakBW_client(SERVER_IP, PORT);
        cout<<"Peak Upload Bandwidth - " + to_string(bandwidths[0])<<endl;
        cout<<"Peak Download Bandwidth - " + to_string(bandwidths[1])<<endl;
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
        network_RTT_server(PORT);
    }
}

void main_connectionOverhead_teardown(int option)
{
    if (option == 1) // client
    {
        double tearDownAvg = network_connectionOverhead_teardown(SERVER_IP, PORT);
        cout<<"Connection Tear Down Overhead - " + to_string(tearDownAvg)<<endl;
    } else { // server
        network_RTT_server(PORT);
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