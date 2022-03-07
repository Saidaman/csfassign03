#include <iostream>
#include <string>

/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */

int main(int argc, char *argv[]) {
    
    //checking for command-line input
    if (argc == 7) {
        int numSets = std::stoi(argv[1]); //turns type char into an int
        int numBlocks = std::stoi(argv[2]);
        int numBytes = std::stoi(argv[3]);
        std::string writeAllocation = argv[4];
        std::string howToWrite = argv[5];
        
    }
    
}
