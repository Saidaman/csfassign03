#include <iostream>
#include <string>

/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */

int errorCheck(int numBlocks, int numSets, int numBytes, std::string writeAllocation, std::string howToWrite);

// Got approval from office hours (Max) that this is enough for MS1 submission
int main(int argc, char *argv[]) {
    
    int numBlocks;
    int numSets;
    int numBytes;
    std::string writeAllocation;
    std::string howToWrite;
    std::string eviction;
    //array of all the counts that we need to output
    int counts[7]; // counts[0] -> total loads, counts[1] -> total stores, counts[2] -> load hits
    // counts[3] -> load misses, counts[4] -> store hits, counts[5] -> store misses, counts[6] -> total cycles

    //checking for command-line input
    if (argc == 7)
    {
        numSets = std::stoi(argv[1]); //turns type char into an int
        numBlocks = std::stoi(argv[2]);
        numBytes = std::stoi(argv[3]);
        writeAllocation = argv[4];
        howToWrite = argv[5];
        eviction = argv[6];
    }
    else { //if there are not 6 other parameters passed in from command line other than ./csim
        std::cerr << "Not enough parameters passed in through command line\n";
        return 1;
    }

    //run input error checking
    if (errorCheck(numBlocks, numSets, numBytes, writeAllocation, howToWrite)) { //errorCheck() returned with errors
        return 2;
    }

    std::string performField;
    while (std::cin >> performField) { //or endof

        int address = 0;
        std::cin >> std::hex >> address;
        int garbageVal = 0;
        std::cin >> garbageVal;

        //still need to do caching stuff
        //couunts loads and stores
        if (performField.compare("l") == 0) {
            counts[0]++;
        }
        else if (performField.compare("s") == 0) {
            counts[1]++;
        }
    }
    //printing output
    printf("Total loads: %d \n", counts[0]);
    printf("Total stores: %d \n", counts[1]);
    printf("Load hits: %d \n", counts[2]);
    printf("Load misses: %d \n", counts[3]);
    printf("Store hits: %d \n", counts[4]);
    printf("Store misses: %d \n", counts[5]);
    printf("Total cycles: %d \n", counts[6]);
}

//This is a helper method to validate the input by checking for error cases in the input parameter collection
int errorCheck(int numBlocks, int numSets, int numBytes, std::string writeAllocation, std::string howToWrite) {
    // command line input error-checking
    // when numBlocks is not a power of 2: https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
    if ((numBlocks != 0) && ((numBlocks & (numBlocks - 1)) == 0))
    {
        std::cerr << "block size is not a power of 2";
        return 2;
    }
    // when numSets is not a power of 2
    if ((numSets != 0) && ((numSets & (numSets - 1)) == 0))
    {
        std::cerr << "number of sets is not a power of 2";
        return 2;
    }
    // when numBytes is less than 4
    if (numBytes < 4)
    {
        std::cerr << "less than 4 bytes";
        return 2;
    }
    // when write-back and no-write-allocate were both specified
    if (writeAllocation.compare("write-back") == 0 && howToWrite.compare("no-write-allocate") == 0)
    { // can you use .compare() with string literals
        std::cerr << "write-back and no-write-allocate were both specified";
        return 2;
    }
    // when logbase2(numSets) + logbase2(numBlocks) > 32 (error case)
    int log2NumSets = numSets;
    int log2NumSetsRes = 0;
    int log2NumBlocks = numBlocks;
    int log2NumBlocksRes = 0;
    while (log2NumSets != 0)
    {
        log2NumSets >>= 1;
        log2NumSetsRes++;
    }
    while (log2NumBlocks != 0)
    {
        log2NumBlocks >>= 1;
        log2NumBlocksRes++;
    }

    if (log2NumSetsRes + log2NumBlocksRes > 32)
    {
        std::cerr << "address size exceeds 32 bits"; 
        return 2;
    }
    return 0;
}