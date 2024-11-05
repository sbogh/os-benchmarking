import json
import statistics
import numpy as np

def main():
    data = read() # read all data

    # assign data to structures
    readOverhead = data["Read Overhead"]
    loopOverhead = data["Loop Overhead"]
    procCreationOverhead = data["Procedure Creation Overhead"]
    threadCreationOverhead = data["Kernel Thread Creation Overhead"]
    procCallOverhead = procCallAnalysis(data)
    procCSOverhead = data["Process Context Switch Overhead Means"]
    threadCSOverhead = data["Thread Context Switch Overhead Means"]
    systemCallOverhead = data["System Call Overhead"]

    # READ OVERHEAD
    print("Read Overhead = " + str(calc_mean(readOverhead)))

    # LOOP OVERHEAD
    print("Loop Overhead = " + str(calc_mean(loopOverhead)))

    # PROCESS CALL OVERHEAD MEANS AND INCREMENTS
    means = []
    for i in range(8):
        for j in range(10):
            mean_curr_vals = []
            mean_curr_vals.append(procCallOverhead[j][i])

        means.append(calc_mean(mean_curr_vals))
    print("Process Call Overhead Means = " + str(means))

    increment = []
    for i in range(7):
        increment.append(means[i + 1] - means[i])
    
    print("Process Call Overhead Increments = " + str(increment))
    
    # SYSTEM CALL OVERHEAD
    systemCallOverhead = np.subtract(systemCallOverhead, [38, 38, 38, 38, 38, 38, 38, 38, 38, 38])
    print("System Call Overhead = " + str(calc_mean(systemCallOverhead)))

    # CREATION OVERHEADS
    print("Process Creation Overhead = " + str(calc_mean(procCreationOverhead)))
    print("Thread Creation Overhead = " + str(calc_mean(threadCreationOverhead)))

    # CONTEXT SWITCH OVERHEADS
    print("Process Context Switch Overhead = " + str(calc_mean(procCSOverhead)))
    print("Thread Context Switch Overhead = " + str(calc_mean(threadCSOverhead)))

    return

def calc_mean(arr):
    """
    Calculate mean of an array

    Input(s):
    arr: Array

    Output(s):
    Value containing the mean of the array
    """
    return statistics.mean(arr)

def calc_stddev(arr):
    """
    Calculate standard deviation of an array

    Input(s):
    arr: Array

    Output(s):
    Value containing the standard deviation of the array
    """
    return statistics.stdev(arr)

def read():
    """
    Read in json file

    Input(s):

    Output(s):
    json file struct
    """
    f = open("cpu_results.json")
    return json.load(f)

def procCallAnalysis(data):
    """
    Parse the procedure call overhead data

    Input(s):
    data: Procedure Call Overhead Data for 0-7 variables

    Output(s):
    procCallOverhead: Organized structure with each of the different functions runs grouped
    """
    procCallOverhead = []
    for i in range(10):
        procCallOverhead.append(data["Run " + str(i) + ": Procedure Call Overhead"])

    return procCallOverhead

main()