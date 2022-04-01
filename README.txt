# csfassign03
Shayan Hossain (JHED: shossa11) & Saidaman Earla (JHED: searla1)

# Best Cache Experiments

For part (b), you’ll use the memory traces as well as your simulator to determine which cache configuration has the best 
overall effectiveness. You should take a variety of properties into account: hit rates, miss penalties, total cache size 
(including overhead), etc. In your README.txt, describe in detail what experiments you ran (and why!), what results you 
got (and how!), and what, in your opinion, is the best cache configuration of them all.

When we first started to test for the best cache configuration, we started off by using the total cycle count as a 
measure of how well the current configuration worked in relation to others. We decided to first use the configuration 
with 256 sets, 4 blocks per set, and 16 byte blocks and tested the write allocation and write through parameter combinations. 
The cycle count results of the following tests for the gcc.trace file are below: 

./csim 256 4 16 write-allocate write-back lru < gcc.trace
Total cycles: 9172648

./csim 256 4 16 write-allocate write-back fifo < gcc.trace
Total cycles: 9672218

./csim 256 4 16 write-allocate write-through lru < gcc.trace
Total cycles: 25305648

./csim 256 4 16 write-allocate write-through fifo < gcc.trace
Total cycles: 25636818

./csim 256 4 16 no-write-allocate write-through lru < gcc.trace
Total cycles: 22858632

./csim 256 4 16 no-write-allocate write-through fifo < gcc.trace
Total cycles: 23095322

With the configuration above, the best results came from using the write-allocate write-back, with no-write-allocate write-through 
coming second and write-allocate write-through performing the worst in terms of cycle count. One interesting thing that we also
noticed was that lru performed better than fifo in all of the three configurations. The best performing write-allocate write-back 
configuration made sense, as the writing to memory was greatly reduced in comparison to the other two configurations, since we were
just essentially keeping the cache as a close copy of the memory in the write-allocate write-back configuration.

The second piece of information that guided our testing was the variety of block sizes and set sizes that we could have used. We started
by choosing the smallest block size (4 bytes) and going up from there. We tested various block sizes in the range [4, 8192]. We immediately
noticed that there was an increase in the cycle counts as we went up in block size: 

configuration: 256 4 *block size* write-allocate write-back lru < gcc.trace
block size      cycle count
4               5093635
16              9172648
64              9544011
256             9557649
1024            14876749
8192            48438659

From the above results, we figured that it was a good idea to assume that a block size of 4 was the most ideal for a lower cycle count. In
order to confirm this thought, we repeated the process of testing different block sizes with direct mapped, set associative, and fully associative
caches and found that the trend in the results stayed the same. 

We also wanted to figure out what the optimal size for the sets and the size for the blocks in those sets were. In order to test for this we used a 
a variety of ranges for the set size and block sizes. After many trials for the cycle counts, we reached the conclusion that a set size of 1024 paired 
with a block size of 256 worked best when the bytes in each block was set to 4. This was the configuration that produced the minimal number for the 
cycle count in the tests that we conducted. We also tested the write-allocate write-back configuration again with the new values for the block and 
set sizes and noticed that write-allocate write-back continued to have the best results in terms of cycle count in comparison with the other two configs.

The final thing that we tested for was the hit/miss rates for different configurations. When testing for different block sizes, we noticed a trend for the 
relationship between increasing block size and hit/miss rates:

lower block sizes (bytes) = higher load/store miss rates
                          = lower load/store hit rates

higher block sizes (bytes) = lower load/store miss rates
                           = higher load/store hit rates

Two of the test experimental cases we tested in order to reach this conclusion are below:

./csim 1024 256 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3662596

./csim 1024 256 65536 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 318115
Load misses: 82
Store hits: 197449
Store misses: 37
Total cycles: 195485164

We also noticed that the total cache size, including the overhead, varied especially with the block sizes. As the block sizes were set to lower numbers, 
the amount of overhead seems to grow since the number of blocks also increases in relation to how many tags we have to keep track of and 
iterate through. This is the opposite relation when we increase the block size. There are less block tags to keep track of since the offset bits
are also more when the size of the blocks increases. This means that there is less overhead since we have less tag bits to keep track of and iterate
through.

In conclusion, the cache configuration that we noticed to give the best results are below:

./csim 1024 256 4 write-allocate write-back lru < gcc.trace
./csim 1024 256 4 write-allocate write-back fifo < gcc.trace

The difference between the lru and fifo eviction performances is actually the same for the configuration that we have set up above as well, meaning that
either eviction method works to produce the best cache configuration in terms of the lowest cycle count. 

# Contributions

Please note:
Most of the work was done together, with very little emphasis on individual work. But for the purpose of this README, 
below are *estimations* of who mostly worked on what functionality. Whatever funcionality/functions are not mentioned 
below were evenly split among the team members.

Saidaman Earla Contributions:
Prepared GitHub repo and worked on implementations of lru and fifo with current data structures and classes

Shayan Hossain Contributions:
Mostly worked on count checking implementation, error checking and did finishing touches (stylistic consistency, etc.)