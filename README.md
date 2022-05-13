# Cache Simulator

This is a cache simulator I made in C.

## Features

The cache simulator will simulate a cache according to options chosen by the user,
as well as simulating reading from and writing to the cache. The cache simulator
borrows heavily from the feature set of [UW's cache simulator](!https://courses.cs.washington.edu/courses/cse351/cachesim/). This includes:

* Different Address Widths (in bits)
    1. 4
    2. 6
    3. 8
    4. 12
    5. 16
* Cache Size (in bytes)
    1. 8
    2. 16
    3. 32
    4. 64
    5. 128
* Block Size (in bytes)
    1. 2
    2. 4
    3. 8
* Associativity (number of lines per set)
    1. 1
    2. 2
    3. 4
* Write-Hit Policy
    1. Write-Through
    2. Write-Back
* Write-Miss Policy
    1. No Write-Allocate
    2. Write-Allocate
* Replacement
    1. LRU
    2. LFU
    3. FIFO
    4. Random

The simulator supports simulating reading from valid addresses and writing one byte
to a specific address. It reports whether the operations are hits or misses, the
current number of hits and misses, and the current hit rate.

## Installation
You can use git or svn to clone the repo into a local repo, or you can use the zip manager.

## Usage
The current iteration can be built and run on Linux using Make:
```
$ make
...
```
When finished, you can run the clean target to clean up any exe files left behind:
```
$ make clean
```
The actual program will prompt the user for information. Simply follow those prompts,
and you should be able to run the simulator.

(Note: the following code requires GCC to be installed.)

## Contributing
Pull requests are welcome. Just be sure to open an issue so we can discuss changes and
potential bugfixes.

