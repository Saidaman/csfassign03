#include <iostream>
#include <string>

/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */

int main(int argc, char *argv[]) {
    
    int numBlocks;
    int numSets;
    int numBytes;

    //checking for command-line input
    if(argc == 7) {                                     //do we even need to check for this?
        numSets = std::stoi(argv[1]); //turns type char into an int
        numBlocks = std::stoi(argv[2]);
        numBytes = std::stoi(argv[3]);
        std::string writeAllocation = argv[4];
        std::string howToWrite = argv[5];
        std::string eviction = argv[6];
    }
    else { //if there are not 6 other parameters passed in from command line other than ./csim
        std::cerr << "Not enough parameters passed in through command line\n";
        return 1;
    }

    //command line input error-checking
    if ((numBlocks % 2) != 0) {
        std::cerr << "block size is not a power of 2";
        return 2;
    }
    
}
