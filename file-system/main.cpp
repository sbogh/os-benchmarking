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
    if(remLoc == 0) //local
    {
        vector<string> path = 
    }
    for(long int size : readFileSizes)
    {
        double total = 0;

        for(int i = 0; i < 10; i++)
        {
            total += fs_readTime_sequential(path, size);
        }

        total /= 10;

        cout<<"File Size: " + to_string(size) + "Sequential Read Time: " + to_string(total)<<endl;
    }
    
}

void main_readTime_random()
{
    for(long int size : cacheFileSizes) // for set of file sizes
    {
        double total = 0;

        for(int i = 0; i < 10; i++)
        {
            total += fs_readTime_random(path, size);
        }

        total /= 10;

        cout<<"File Size: " + to_string(size) + "Random Read Time: " + to_string(total)<<endl;
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
    // Make files for cache size operation
    main_makeFiles(cacheFileNames, cacheFileSizes);
    
    // Cache Size
    main_cacheSize();

    // Make files for read operations
    main_makeFiles(readFileNames, readFileSizes);

    // READ TIME SEQUENTIAL LOCAL
    main_readTime_sequential();

    // READ TIME RANDOM ACCESS LOCAL
    main_readTime_random();

    // READ TIME SEQUENTIAL REMOTE
    main_readTime_sequential();

    // READ TIME RANDOM ACCESS REMOTE
    main_readTime_random();

    // CONTENTION
    main_contention(localPath, localPath_child);
}