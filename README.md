# How to Run CPU Benchmark

The files in this repository will operate in groups. The sets of operations are grouped and run through the main.cpp file in each folder. To run the benchmarks compile this file using the following in your command line:
```
g++ main.cpp -o main
```
Then, run the compiled file using:
```
./main
```

To run analysis on the outputted data in the json file, run the Python file using the following:
```
python out_analysis.py
```
