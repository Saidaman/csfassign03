/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <climits>

/*
 * A class to represent a block in the cache.
 */
class Block {
public:
	unsigned index;
	unsigned tag;
	bool valid;
	bool dirty;
	unsigned load_ts;	//timestamp for FIFO
	unsigned access_ts; //timestamp for LRU

public:
	//default constructor
	Block() {
		index = 0;
		tag = 0;
		valid = false;
		dirty = false;
		load_ts = 0;
		access_ts = INT_MAX; //for implementation of lru
	}

public:
	//parameterized constructor
	Block(unsigned index, unsigned tag, bool valid, bool dirty, unsigned load_ts, unsigned access_ts) {
		this->index = index;
		this->tag = tag;
		this->valid = valid;
		this->dirty = dirty;
		this->load_ts = load_ts;
		this->access_ts = access_ts;
	}
};

/*
 * Error checking function for the command-line input.
 * 
 * Parameters:
 * 	numSets - number of sets present in the cache
 * 	numBlocks - number of blocks in each set
 * 	numBytes - number of bytes for each block
 * 	writeAllocation - string to relay whether to write allocate or not
 * 	howToWrite - string to relay whether to write through or write back
 * 
 * Returns:
 * 	2 if there is an error
 *  0 if there is no error
 * 
 */
int errorCheck(int numSets, int numBlocks, int numBytes, std::string writeAllocation, std::string howToWrite);

/*
 * Function to print the output of the program (total loads, stores, hits, etc.).
 *
 * Parameters:
 * 	counts - array containing each of the counts to be printed
 */
void printOutput(int counts[]);

/*
 * Function to get the log-base-2 of a number.
 *
 * Parameters:
 * 	num - the number to get log-base-2 of
 *
 * Returns:
 * 	the result of log-base-2 of num
 */
int logBase2(int num);

/*
 * Function to confirm whether a given numer is a power of two or not.
 *
 * Parameters:
 * 	num - the number to check
 *
 * Returns:
 *	whether the given number is a power of two or not (0 false, non-zero otherwise)
 */
int isPowerOfTwo(int num);

/*
 * Main method for a cache simulator program in C++.
 *
 * Parameters:
 * 	main method/command line arguments
 */
int main(int argc, char *argv[]) {

	int numSets;
	int numBlocks;
	int numBytes;
	std::string writeAllocation;
	std::string howToWrite;
	std::string eviction;
	//array of all the counts that we need to output
	int counts[7] = {0}; // counts[0] -> total loads, counts[1] -> total stores, counts[2] -> load hits
	// counts[3] -> load misses, counts[4] -> store hits, counts[5] -> store misses, counts[6] -> total cycles
	int lruCounter = 0; //"global" counter for access timestamps for the blocks

	// checking for command-line input
	if (argc == 7) {
		numSets = std::stoi(argv[1]); //turns type char into a hex and converts to int
		numBlocks = std::stoi(argv[2]);
		numBytes = std::stoi(argv[3]);
		writeAllocation = argv[4];
		howToWrite = argv[5];
		eviction = argv[6];
	} else { // if there are not 6 other parameters passed in from command line other than ./csim
		std::cerr << "Not enough parameters passed in through command line\n";
		return 1;
	}

	//run input error checking
	if (errorCheck(numSets, numBlocks, numBytes, writeAllocation, howToWrite)) { //errorCheck() returned with errors
		return 2;
	}

	//initializing the cache
	std::vector<std::vector<Block> > cache(numSets, std::vector<Block>(numBlocks));
	for (int i = 0; i < numSets; i++) {
		for (int j = 0; j < numBlocks; j++) {
			cache[i][j] = Block(); //initialized with default constructor
		}
	}

	std::string performField;
	while (std::cin >> performField) { 

		long address;
		std::cin >> std::hex >> address;
		int garbageVal;
		std::cin >> garbageVal;

		lruCounter++; //increments "global" access timestamp for lru

		int numOffsetBits = logBase2(numBytes);
		int numIndexBits = logBase2(numSets);

		//logBase2(numSets) gives you the number of offset bits. Then,
		//we left shift 1 so it can have logBase2(numSets) number of trailing zeros.
		//Then, we subtract 1 so that we can have a binary number with
		//logBase2(numSets) number of bits of just 1s. We later use this result
		//with the & operator to isolate the respective bits in the address.
		unsigned indexBits = (1 << numIndexBits) - 1;

		address >>= numOffsetBits;
		unsigned index = address & indexBits;
		unsigned tag = address >> numIndexBits; //tag value is whatever's left

		Block currBlock(index, tag, true, false, 0, lruCounter);

		bool alreadyExists = false;
		if (performField.compare("l") == 0) {
			counts[0]++; //increment total loads
			for (int i = 0; i < numBlocks; i++) {
				if (cache[index][i].tag == currBlock.tag && cache[index][i].valid) {
					alreadyExists = true;
					counts[2]++; //increment load hits
					cache[index][i].access_ts = lruCounter;
					counts[6]++; //increment cycle count
				}
			}
			if (!alreadyExists) {
				counts[3]++; // increment load misses
				counts[6] += numBytes / 4 * 100; //increment cycle count
				int idx; 
				for (idx = 0; idx < numBlocks; idx++) {
					if (!cache[index][idx].valid) {
						cache[index][idx].tag = currBlock.tag;
						cache[index][idx].access_ts = lruCounter;//currBlock.access_ts;
						//TODO: Add load_ts for MS3
						cache[index][idx].dirty = false;
						cache[index][idx].valid = true;
						break;
					}
				}
				if (idx == numBlocks) { //meaning we've reached the end of the set without finding invalid blocks
					if(eviction.compare("lru") == 0) {
						unsigned idxToFind;
						unsigned minVal = INT_MAX;
						//calculates the index of the block with the smallest timestamp
						for (int i = 0; i < numBlocks; i++) {
							if (cache[index][i].access_ts < minVal) {
								minVal = cache[index][i].access_ts;
								idxToFind = i;
							}
						}
						if (howToWrite.compare("write-through") != 0 && cache[index][idxToFind].dirty) {
							counts[6] += numBytes / 4 * 100;
						}
						//load at min index
						cache[index][idxToFind].tag = currBlock.tag;
						cache[index][idxToFind].access_ts = currBlock.access_ts;
						//TODO: Add load_ts for MS3
						cache[index][idxToFind].dirty = false;
						cache[index][idxToFind].valid = true;
					} else {
						//TODO: fifo for MS3 
					}
				}
			}
		} else if (performField.compare("s") == 0) {
			counts[1]++; //increment total stores count
			for (int i = 0; i < numBlocks; i++) {
				if (cache[index][i].tag == tag && cache[index][i].valid) {
					alreadyExists = true;
					counts[4]++; //increment store hits
					cache[index][i].access_ts = lruCounter;
					if (howToWrite.compare("write-through") != 0) {
						cache[index][i].dirty = true;
						if (writeAllocation.compare("write-allocate") == 0) {
							counts[6]++; // increment cycle count
						} else {
							counts[6] += 100;
						}
					} else {
						counts[6] += 101;
					}
				}
			}
			if (!alreadyExists) {
				counts[5]++; //increment store misses
				if (writeAllocation.compare("write-allocate") == 0) {
					if (howToWrite.compare("write-through") == 0) {
						counts[6] += ((numBytes / 4) * 100) + 100;
					} else {
						counts[6] += (numBytes / 4) * 100;
					}
					int idx;
					for (idx = 0; idx < numBlocks; idx++) {
						if (!cache[index][idx].valid) {
							cache[index][idx].tag = currBlock.tag;
							cache[index][idx].access_ts = currBlock.access_ts;
							//TODO: Add load_ts for MS3
							cache[index][idx].dirty = false;
							cache[index][idx].valid = true;
							break;
						}
					}
					if (idx == numBlocks) { //meaning we've reached the end of the set
						if(eviction.compare("lru") == 0) {
						unsigned idxToFind;
						unsigned minVal = INT_MAX;
						//calculates the index of the block with the smallest timestamp
						for (int i = 0; i < numBlocks; i++) {
							if (cache[index][i].access_ts < minVal) {
								minVal = cache[index][i].access_ts;
								idxToFind = i;
							}
						}
						if (howToWrite.compare("write-through") != 0) {
							if (cache[index][idxToFind].dirty) {
								counts[6] += numBytes / 4 * 100;
							}
						}
						//store at min index
						cache[index][idxToFind].tag = currBlock.tag;
						cache[index][idxToFind].access_ts = currBlock.access_ts;
						//TODO: Add load_ts for MS3
						cache[index][idxToFind].dirty = false;
						cache[index][idxToFind].valid = true;
						} else {
						//TODO: fifo for MS3 
						}
					}
				} else {
					if (howToWrite.compare("write-through") == 0) {
						counts[6] += 100;
					}
				}
			}
		}
	}
	//print output counts
	printOutput(counts);
}

int logBase2(int num) {
	int res = -1;
	while (num != 0) {
		num >>= 1;
		res++;
	}
	return res;
}

int isPowerOfTwo(int num) { //https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
	return ((num != 0) && ((num & (num - 1)) == 0));
}

//this is the helper method to print all the output at the end of main
void printOutput(int counts[]) {
	printf("Total loads: %d\n", counts[0]);
	printf("Total stores: %d\n", counts[1]);
	printf("Load hits: %d\n", counts[2]);
	printf("Load misses: %d\n", counts[3]);
	printf("Store hits: %d\n", counts[4]);
	printf("Store misses: %d\n", counts[5]);
	printf("Total cycles: %d\n", counts[6]);
}

//This is a helper method to validate the input by checking for error cases in the input parameter collection
int errorCheck(int numSets, int numBlocks, int numBytes, std::string writeAllocation, std::string howToWrite) {
	// command line input error-checking
	// when numSets is not a power of 2
	if (!isPowerOfTwo(numSets)) {
		std::cerr << "number of sets is not a power of 2\n";
		return 2;
	}
	// when numBlocks is not a power of 2
	if (!isPowerOfTwo(numBlocks)) {
		std::cerr << "block size is not a power of 2\n";
		return 2;
	}
	//when numBytes is not a power of 2
	if (!isPowerOfTwo(numBytes)) {
		std::cerr << "number of bytes is not a power of 2\n";
		return 2;
	}
	// when numBytes is less than 4
	if (numBytes < 4) {
		std::cerr << "number of bytes indicated is less than 4 bytes\n";
		return 2;
	}
	// when write-back and no-write-allocate were both specified
	if (writeAllocation.compare("write-back") == 0 && howToWrite.compare("no-write-allocate") == 0) {
		std::cerr << "write-back and no-write-allocate were both specified\n";
		return 2;
	}
	// when logbase2(numSets) + logbase2(numBlocks) > 32 (error case)
	if (logBase2(numSets) + logBase2(numBlocks) > 32) {
		std::cerr << "address size exceeds 32 bits\n"; 
		return 2;
	}
	return 0;
}
