#include "memory.cpp"
/* 
* @file main.cpp
* @author Shayan Boghani
*
* Experiments for Memory Module (main)
 */

using namespace std;
using json = nlohmann::json;

json mem_results; // initialize json structure

void main_memoryAccess()
{
    cout<<"Beginning memory access latency test..."<<endl;
    for (int i : sizes)
    {
        double latency_avg = 0;
        for(int j = 0; j < MEM_ACCESS_COUNT; j++)
        {
            double latency_curr = memory_accessTime(i); // get access latency for specified array size
            latency_avg += latency_curr;
        }
        
        latency_avg /= MEM_ACCESS_COUNT;

        string header = "Memory Access Latency: " + to_string(i) + " bytes"; // json key
        mem_results[header] = latency_avg; // store latency in json
        cout<<"Completed " + header + " - " + to_string(latency_avg)<<endl;
    }
    cout<<"Completed memory access latency test"<<endl;
}

void main_memoryBW_Write()
{
    cout<<"Beginning memory write bandwidth test..."<<endl;
    for (int i : bandwidth_sizes)
    {
        double curr_bw = memory_bandwidthWrite(i); // get write bandwidth for specified array size

        string header = "Memory Write Bandwidth: " + to_string(i) + " bytes"; // json key
        mem_results[header] = curr_bw; // store latency in json
        cout<<"Completed" + header + " - " + to_string(curr_bw)<<endl;
    }
    cout<<"Completed memory write bandwidth test"<<endl;

}

void main_memoryBW_Read()
{
    cout<<"Beginning memory read bandwidth test..."<<endl;
    for (int i : bandwidth_sizes)
    {
        double curr_bw = memory_bandwidthRead(i); // get read bandwidth for specified array size

        string header = "Memory Read Bandwidth: " + to_string(i) + " bytes"; // json key
        mem_results[header] = curr_bw; // store latency in json
        cout<<"Completed" + header + " - " + to_string(curr_bw)<<endl;
    }
    cout<<"Completed memory write bandwidth test"<<endl;
}

void main_memoryPageFault()
{
    cout<<"Beginning memory page fault service test..."<<endl;

    double defTime = 0; // initialize variable to store total time taken
    vector<double> times;

    for(int i = 0; i < 100; i++)
    {
        double curr_time = memory_pageFaultServTime();
        defTime += curr_time; // add current loop time to total
        times.push_back(curr_time); // add curr loop to vector for std dev statistics
    }

    defTime /= 100; // average over 100 runs

    string header = "Memory Page Fault Service Average:"; //json key
    mem_results[header] = defTime; // add benchmark time to json file

    cout<<header + " " + to_string(defTime)<<endl;

    double stddev = calc_stddev(times);
    cout<<"Memory Page Fault Service Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed memory page fault service test"<<endl;
}

void write_json()
{
    ofstream file("mem_results.json");
    file << mem_results;
}

int main()
{
    // ACCESS L1, L2, L3 CACHES AND MAIN MEMORY
    main_memoryAccess();

    // READ MEMORY BANDWIDTH
    main_memoryBW_Read();

    // WRITE MEMORY BANDWIDTH
    main_memoryBW_Write();

    // PAGE FAULT SERVICE TIME
    main_memoryPageFault();

    // Write results to json file
    write_json();
}

