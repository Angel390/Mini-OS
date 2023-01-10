
/*
Angel Mejia
CSE 4610

filesys.h
Contains the function stubs for the class functions
*/

#ifndef FILESYS_H
#define FILESYS_H

#include "sdisk.h"
#include "block.h"

using namespace std;

class Filesys : public Sdisk
{
public:
    Filesys();                                                   // Default Constructor
    Filesys(string diskName, int numberOfBlocks, int blockSize); // Our Constructor
    int fsClose();                                               // Closes the file system
    int newFile(string file);                                    // Creates a new file
    int rmFile(string file);                                     // Removes the file
    int getFirstBlock(string file);                              // Return the first block of a file
    int addBlock(string file, string block);                     // Add a new block to the file system
    int delBlock(string file, int blockNumber);                  // Deletes the block from the file system
    int readBlock(string file, int blockNumber, string &buffer); // Reads the block from the file system
    int writeBlock(string file, int blockNumber, string buffer); // Writes a block to the file system
    int nextBlock(string file, int blockNumber);
    vector<string> ls();

    // My Test Functions
    int getRootSize();
    int getFatSize();

protected:
    int rootSize; // Max number of entries in ROOT
    int fatSize;  // Number of blocks occupied by FAT

    vector<string> fileName; // File names in ROOT
    vector<int> firstBlock;  // First blocks in ROOT
    vector<int> fat;         // FAT

    int buildFS(); // Builds the file system
    int readFS();  // Reads the file system
    int fsSynch(); // Writes the FAT and ROOT to the sdisk
    bool fileBlockCheck(string file, int blockNumber);
};

#endif