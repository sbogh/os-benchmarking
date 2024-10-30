import json
import statistics

def main():
    data = read()

    readOverhead = data["Read Overhead"]
    loopOverhead = data["Loop Overhead"]
    procCreationOverhead = data["Procedure Creation Overhead"]
    threadCreationOverhead = data["Kernel Thread Creation Overhead"]
    procCallOverhead = procCallAnalysis(data)
    procCSOverhead = data["Process Context Switch Overhead Means"]
    threadCSOverhead = data["Thread Context Switch Overhead Means"]

    #print(calc_mean(readOverhead))
    #print(calc_mean(loopOverhead))

    """
    means = []
    for i in range(8):
        for j in range(10):
            mean_curr_vals = []
            mean_curr_vals.append(procCallOverhead[j][i])

        means.append(calc_mean(mean_curr_vals))
    print(means)

    increment = []
    for i in range(7):
        increment.append(means[i + 1] - means[i])
    
    print(increment)
    """
    print(calc_mean(procCreationOverhead))


    return

def calc_mean(arr):
    return statistics.mean(arr)

def calc_stddev(arr):
    return statistics.stdev(arr)

def read():
    f = open("cpu_results.json")
    return json.load(f)

def procCallAnalysis(data):
    procCallOverhead = []
    for i in range(10):
        procCallOverhead.append(data["Run " + str(i) + ": Procedure Call Overhead"])

    return procCallOverhead

main()