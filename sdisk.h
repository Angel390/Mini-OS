/*
Angel Mejia
CSE 4610
09/13/2020

Random Access file I/O
sdisk.h
Contains the function stubs for the class functions
*/

#ifndef SDISK_H
#define SDISK_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Sdisk
{
public:
    Sdisk();                                                   // Default Constructor
    Sdisk(string diskName, int numberOfBlocks, int blockSize); // Constructor
    int getBlock(int blockNumber, string &buffer);             // We have to pass by reference; Opens the file and gets a block
    int putBlock(int blockNumber, string buffer);              // Opens the file and writes a block
    int getNumberOfBlocks();                                   // Returns number of blocks
    int getBlockSize();                                        // Returns size

protected:
    string diskName;    // Name of the virtual disk in string format
    int numberOfBlocks; // Set the number of blocks
    int blockSize;      // Set the block size
};

#endif