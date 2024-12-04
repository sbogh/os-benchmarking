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
    vector<double> measureReadOut;

    for(int i = 0; i < 10; i++)
    {    
        measureReadOut.push_back(measureReadOverhead());
        cout<<"Read Overhead Iteration " + to_string(i) + " - " + to_string(measureReadOut.back())<<endl;
    }

    cpu_results["Read Overhead"] = measureReadOut;

    double stddev = calc_stddev(measureReadOut);
    cout<<"Read Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_loopOverhead()
{
    cout<<"Measure Loop Overhead"<<endl;
    vector<double> measureLoopOut;

    for(int i = 0; i < 10; i++)
    {
        measureLoopOut.push_back(measureLoopOverhead());
        cout<<"Loop Overhead Iteration " + to_string(i) + " - " + to_string(measureLoopOut.back())<<endl;
    }

    cpu_results["Loop Overhead"] = measureLoopOut;

    double stddev = calc_stddev(measureLoopOut);
    cout<<"Loop Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_procCallOverhead()
{
    cout<<"Measure Procedure Call Overhead"<<endl;
    for(int i = 0; i < 10; i++)
    {
        double* currArrPointer = measureProcedureCallOverhead();
        vector<double> currArr;
        for(int arrIter = 0; arrIter < 8; arrIter++)
        {
            currArr.push_back(currArrPointer[arrIter]);
            cout<<to_string(arrIter) + " # of Vars - Procedure Call Overhead: " + to_string(currArr.back())<<endl;
        }

        string label = "Run " + to_string(i) + ": Procedure Call Overhead";
        cpu_results[label] = currArr;
        cout<<label + " " + to_string(cpu_results[label])<<endl;
    }
    cout<<"Completed"<<endl;
}

void main_systemCallOverhead()
{
    cout<<"Measure System Call Overhead"<<endl;
    vector<double> measureLoopOut;

    for(int i = 0; i < 10; i++)
    {
        measureLoopOut.push_back(measureSystemCallOverhead());
        cout<<"System Call Overhead Iteration " + to_string(i) + " - " + to_string(measureLoopOut.back())<<endl;
    }

    cpu_results["System Call Overhead"] = measureLoopOut;

    double stddev = calc_stddev(measureLoopOut);
    cout<<"System Call Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_procCreationOverhead()
{
    cout<<"Measure Procedure Creation Overhead"<<endl;
    vector<double> measureProcCreate;

    for(int i = 0; i < 10; i++)
    {
        measureProcCreate.push_back(measureProcessCreationOverhead());
        cout<<"Procedure Creation Overhead Iteration " + to_string(i) + " - " + to_string(measureProcCreate.back())<<endl;
    }

    cpu_results["Procedure Creation Overhead"] = measureProcCreate;

    double stddev = calc_stddev(measureProcCreate);
    cout<<"Procedure Creation Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_threadCreationOverhead()
{
    cout<<"Measure Kernel Thread Creation Overhead"<<endl;
    vector<double> measureThreadCreate;

    for(int i = 0; i < 10; i++)
    {
        measureThreadCreate.push_back(kernelThreadCreationOverhead());
        cout<<"Kernel Thread Creation Overhead Iteration " + to_string(i) + " - " + to_string(measureThreadCreate.back())<<endl;
    }

    cpu_results["Kernel Thread Creation Overhead"] = measureThreadCreate;

    double stddev = calc_stddev(measureThreadCreate);
    cout<<"Kernel Thread Creation Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_procCSOverhead()
{
    cout<<"Measure Process Context Switch Overhead"<<endl;

    vector<double> mean_arr;

    for(int i = 0; i < 10; i++)
    {
        mean_arr.push_back(processContextSwitch());
        cout<<"Process Context Switch Overhead Iteration " + to_string(i) + " - " + to_string(mean_arr.back())<<endl;
    }

    string mean_label = "Process Context Switch Overhead Means";

    cpu_results[mean_label] = mean_arr;

    double stddev = calc_stddev(mean_arr);
    cout<<"Process Context Switch Overhead Standard Deviation: " + to_string(stddev)<<endl;

    cout<<"Completed"<<endl;
}

void main_threadCSOverhead()
{
    cout<<"Measure Kernel Thread Context Switch Overhead"<<endl;

    vector<double> mean_arr;

    for(int i = 0; i < 10; i++)
    {
        mean_arr.push_back(threadContextSwitch());
        cout<<"Kernel Thread Context Switch Overhead Iteration " + to_string(i) + " - " + to_string(mean_arr.back())<<endl;
    }

    string label = "Thread Context Switch Overhead Means";
    cpu_results[label] = mean_arr;

    double stddev = calc_stddev(mean_arr);
    cout<<"Kernel Thread Context Switch Overhead Standard Deviation: " + to_string(stddev)<<endl;

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
