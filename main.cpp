#include <iostream>
#include <string>

/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */


// Got approval from office hours (Max) that this is enough for MS1 submission
int main(int argc, char *argv[]) {
    
    int numBlocks;
    int numSets;
    int numBytes;
    std::string writeAllocation;
    std::string howToWrite;
    std::string eviction;

    //checking for command-line input
    if(argc == 7) { //do we even need to check for this?
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

    //command line input error-checking
    //when numBlocks is not a power of 2: https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
    if ((numBlocks != 0) && ((numBlocks & (numBlocks - 1)) == 0)) {
        std::cerr << "block size is not a power of 2";
        return 2;
    }
    //when numSets is not a power of 2
    if ((numSets != 0) && ((numSets & (numSets - 1)) == 0)) {
        std::cerr << "number of sets is not a power of 2";
        return 2;
    }
    //when numBlocks is less than 4
    if (numBlocks < 4) {
        std::cerr << "block size is less than 4";
        return 2;
    }
    //when write-back and no-write-allocate were both specified
    if (writeAllocation.compare("write-back") == 0 && howToWrite.compare("no-write-allocate") == 0) { // can you use .compare() with string literals
        std::cerr << "write-back and no-write-allocate were both specified";
        return 2;
    }
    //logbase2
    if () {

    }
}
