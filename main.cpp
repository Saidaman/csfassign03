#include <iostream>
#include <string>
#include <map>
#include <vector>

/*
 * Implementation of a cache simulator program in C++.
 * CSF Assignment 3
 * Shayan Hossain, Sai Earla
 * shossa11@jhu.edu, searla1@jhu.edu
 */

//TODO: add javadoc comments for these?
int errorCheck(int numSets, int numBlocks, int numBytes, std::string writeAllocation, std::string howToWrite);
void printOutput(int counts[]);
int logBase2(int num);
int isPowerOfTwo(int num);

class Block {
	public: //get/set
		unsigned offset;
		unsigned index;
		unsigned tag;
		bool valid;
		bool dirty;
		unsigned load_ts; //timestamp for FIFO
		unsigned access_ts; //timestamp for LRU

	public:
		Block(unsigned offset, unsigned index, unsigned tag, bool valid, bool dirty, unsigned load_ts, unsigned access_ts) {
			this->offset = offset;
			this->index = index;
			this->tag = tag;
			this->valid = valid;
			this->dirty = dirty;
			this->load_ts = load_ts;
			this->access_ts = access_ts;
		}
};

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

	// initialize the cache: key = index, vector of blocks = sets
	std::map<unsigned, std::vector<Block>> cache;
	// initialize the iterator for the map cache
	std::map<unsigned, std::vector<Block>>::iterator it;

		// checking for command-line input
		if (argc == 7)
	{
		numSets = std::stoi(argv[1]); //turns type char into an int
		numBlocks = std::stoi(argv[2]);
		numBytes = std::stoi(argv[3]);
		writeAllocation = argv[4];
		howToWrite = argv[5];
		eviction = argv[6];
	}
	else
	{ // if there are not 6 other parameters passed in from command line other than ./csim
		std::cerr << "Not enough parameters passed in through command line\n";
		return 1;
	}

	//run input error checking
	if (errorCheck(numSets, numBlocks, numBytes, writeAllocation, howToWrite)) { //errorCheck() returned with errors
		return 2;
	}

	std::string performField;
	while (std::cin >> performField) { //or endof

		long address;
		std::cin >> std::hex >> address;
		int garbageVal;
		std::cin >> garbageVal;

		int numOffsetBits = logBase2(numBytes);
		int numIndexBits = logBase2(numSets);

		//logBase2(numBytes) gives you the number of offset bits. Then,
		//we left shift 1 so it can have logBase2(numBytes) number of trailing zeros.
		//Then, we subtract 1 so that we can have a binary number with
		//logBase2(numBytes) number of bits of just 1s. We later use this result
		//with the & operator to isolate those bits in the address.
		unsigned offsetBits = (1 << numOffsetBits) - 1;
		//similar approach as above   
		unsigned indexBits = (1 << numIndexBits) - 1;
		//similar approach as above
		unsigned tagBits = (1 << (32 - numOffsetBits - numIndexBits)) - 1;
		
		unsigned offset = address & offsetBits;
		address >>= numOffsetBits;
		unsigned index = address & indexBits;
		unsigned tag = address >> numIndexBits;

		Block currBlock(offset, index, tag, false, false, 0, 0);
		it = cache.find(currBlock.index);
		//if the index doesn't currently exist
		if (it != cache.end())

		//couunts loads and stores
		if (performField.compare("l") == 0) {
			counts[0]++;
		} else if (performField.compare("s") == 0) {
			counts[1]++;
		}
	}
	//printing output
	printOutput(counts);
}

int logBase2(int num) {
	int res = 0;
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
