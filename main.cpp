/* 
* @file cpu.h
* @author Shayan Boghani
*
* CPU Header
 */
#include "cpu.cpp"
using namespace std;
using json = nlohmann::json;

json cpu_results; // initialize json structure

void main_readOverhead()
{
    cout<<"Measure Read Overhead"<<endl;
    double measureReadOut[10];

    for(int i = 0; i < 10; i++)
    {    
        measureReadOut[i] = measureReadOverhead();
    }

    cpu_results["Read Overhead"] = measureReadOut;
    cout<<"Completed"<<endl;
}

void main_loopOverhead()
{
    cout<<"Measure Loop Overhead"<<endl;
    double measureLoopOut[10];

    for(int i = 0; i < 10; i++)
    {
        measureLoopOut[i] = measureLoopOverhead();
    }

    cpu_results["Loop Overhead"] = measureLoopOut;
    cout<<"Completed"<<endl;
}

void main_procCallOverhead()
{
    cout<<"Measure Procedure Call Overhead"<<endl;
    for(int i = 0; i < 10; i++)
    {
        double* currArrPointer = measureProcedureCallOverhead();
        double currArr[8];
        for(int arrIter = 0; arrIter < 8; arrIter++)
        {
            currArr[arrIter] = currArrPointer[arrIter];
        }

        string label = "Run " + to_string(i) + ": Procedure Call Overhead";
        cpu_results[label] = currArr;
    }
    cout<<"Completed"<<endl;
}

void main_systemCallOverhead()
{
    cout<<"Measure System Call Overhead"<<endl;
    double measureLoopOut[10];

    for(int i = 0; i < 10; i++)
    {
        measureLoopOut[i] = measureSystemCallOverhead();
    }

    cpu_results["System Call Overhead"] = measureLoopOut;
    cout<<"Completed"<<endl;
}

void main_procCreationOverhead()
{
    cout<<"Measure Procedure Creation Overhead"<<endl;
    double measureProcCreate[10];

    for(int i = 0; i < 10; i++)
    {
        measureProcCreate[i] = measureProcessCreationOverhead();
    }

    cpu_results["Procedure Creation Overhead"] = measureProcCreate;
    cout<<"Completed"<<endl;
}

void main_threadCreationOverhead()
{
    cout<<"Measure Kernel Thread Creation Overhead"<<endl;
    double measureThreadCreate[10];

    for(int i = 0; i < 10; i++)
    {
        measureThreadCreate[i] = kernelThreadCreationOverhead();
    }

    cpu_results["Kernel Thread Creation Overhead"] = measureThreadCreate;
    cout<<"Completed"<<endl;
}

void main_procCSOverhead()
{
    cout<<"Measure Process Context Switch Overhead"<<endl;

    static double mean_arr[10];
    static double stddev_arr[10];

    for(int i = 0; i < 10; i++)
    {
        mean_arr[i] = processContextSwitch();
    }

    string mean_label = "Process Context Switch Overhead Means";

    cpu_results[mean_label] = mean_arr;

    cout<<"Completed"<<endl;
}

void main_threadCSOverhead()
{
    cout<<"Measure Kernel Thread Context Switch Overhead"<<endl;

    double mean_arr[10];

    for(int i = 0; i < 10; i++)
    {
        mean_arr[i] = threadContextSwitch();
    }

    string label = "Thread Context Switch Overhead Means";
    cpu_results[label] = mean_arr;
    cout<<"Completed"<<endl;
}

void write_json()
{
    ofstream file("cpu_results.json");
    file << cpu_results;
}

int main()
{

    // READ OVERHEAD
    main_readOverhead();

    // LOOP OVERHEAD
    main_loopOverhead();

    // PROCEDURE CALL OVERHEAD
    main_procCallOverhead();

    // SYSTEM CALL OVERHEAD
    main_systemCallOverhead();

    // PROCEDURE CREATION OVERHEAD
    main_procCreationOverhead();

    // THREAD CREATION OVERHEAD
    main_threadCreationOverhead();

    // PROCEDURE CONTEXT SWITCH OVERHEAD
    main_procCSOverhead();

    // THREAD CONTEXT SWITCH OVERHEAD
    main_threadCSOverhead();

    // Write results to json file
    write_json();
}
