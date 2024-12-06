#include "fs.cpp"
/* 
* @file main.cpp
* @author Shayan Boghani
*
* Experiments for File System Module (main)
 */

using namespace std;
using json = nlohmann::json;

json filesystem_results; // initialize json structure

void main_cacheSize()
{
    vector<double> accessTimes = fs_cacheSize();

    int counter = 0;
    for(double time : accessTimes)
    {
        cout<<"File Size: " + to_string(cacheFileSizes[counter]) + " - " + to_string(time)<<endl;
        counter += 1;
    }
}

void main_readTime_sequential(int remLoc)
{
    vector<string> path;
    if(remLoc == 0) //local
    {
        path = readFileNames;
    } else{ // remote
        path = remoteFileNames;
    }
    
    int counter = 0;
    for(long int size : readFileSizes)
    {
        double total = 0;

        for(int i = 0; i < 10; i++)
        {
            total += fs_readTime_sequential(path[counter], size);
        }

        total /= 10;

        cout<<"File Size: " + to_string(size) + "Sequential Read Time: " + to_string(total)<<endl;
        counter += 1;
    }
    
}

void main_readTime_random(int remLoc)
{
    vector<string> path;
    if(remLoc == 0) //local
    {
        path = readFileNames;
    } else{ // remote
        path = remoteFileNames;
    }

    int counter = 0;
    for(long int size : cacheFileSizes) // for set of file sizes
    {
        double total = 0;

        for(int i = 0; i < 10; i++)
        {
            total += fs_readTime_random(path[counter], size);
        }

        total /= 10;

        cout<<"File Size: " + to_string(size) + "Random Read Time: " + to_string(total)<<endl;
        counter += 1;
    } 
}

void main_contention(string path, string child_path)
{
    fs_contention(path, child_path);
}

void main_makeFiles(vector<string> fileNames, vector<long int> fileSizes)
{
    int counter = 0;
    for(string name : fileNames)
    {
        fs_createFile(name, fileSizes[counter]);
        counter += 1;
    }

}

int main()
{
    int remLoc = 0; // set to 0 for local, 1 for remote

    // Make files for cache size operation
    main_makeFiles(cacheFileNames, cacheFileSizes);
    
    // Cache Size
    main_cacheSize();

    // Make files for read operations
    main_makeFiles(readFileNames, readFileSizes);

    // READ TIME SEQUENTIAL LOCAL
    main_readTime_sequential(remLoc);

    // READ TIME RANDOM ACCESS LOCAL
    main_readTime_random(remLoc);

    // READ TIME SEQUENTIAL REMOTE
    main_readTime_sequential(remLoc);

    // READ TIME RANDOM ACCESS REMOTE
    main_readTime_random(remLoc);

    // CONTENTION
    main_contention(localPath, localPath_child);
}