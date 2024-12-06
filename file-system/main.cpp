#include "file-system.cpp"
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
    for(auto time : accessTimes)
    {
        cout<<"File Size: " + to_string(fileSizes[counter]) + " - " + to_string(time)<<endl;
        counter += 1;
    }
}

void main_readTime_sequential(string path, int size)
{
    double total = 0;

    for(int i = 0; i < 10; i++)
    {
        total += fs_readTime_sequential(path, size);
    }

    total /= 10;

    cout<<"Sequential Read Time: " + to_string(total)<<endl;
}

void main_readTime_random(string path, int size)
{
    double total = 0;

    for(int i = 0; i < 10; i++)
    {
        total += fs_readTime_random(path, size);
    }

    total /= 10;

    cout<<"Sequential Read Time: " + to_string(total)<<endl;
}

void main_contention(string path)
{
    fs_contention(path);
}

void main_makeFiles()
{
    counter = 0;
    for(auto name : fileNames)
    {
        fs_createFile(name, cacheFileSizes[counter]);
        counter += 1;
    }

}

int main()
{
    //string localPath = XXX; // FILL
    //string remotePath = XXX; //FILL
    //int size = XXX; // FILL BASED ON FILE CHOSEN

    //string childPath = XXX; // FILL

    // Make files
    main_makeFiles();
    
    // Cache Size
    main_cacheSize();

    // READ TIME SEQUENTIAL LOCAL
    main_readTime_sequential(localPath, size);

    // READ TIME RANDOM ACCESS LOCAL
    main_readTime_random(localPath, size);

    // READ TIME SEQUENTIAL REMOTE
    main_readTime_sequential(remotePath, size);

    // READ TIME RANDOM ACCESS REMOTE
    main_readTime_random(remotePath, size);

    // CONTENTION
    main_contention(localPath);
}