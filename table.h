/*
Angel Mejia
CSE 4610

table.h
Contains the function stubs for the class functions
*/

#ifndef TABLE_H
#define TABLE_H

#include "filesys.h"


using namespace std;

class Table : public Filesys
{
public:
    Table();                                                                                      // Default constructor
    Table(string diskName, int numberOfBlocks, int blockSize, string flatFile, string indexFile); //:Filesys(diskName, numberOfBlocks, blockSize) // Create the table object
    int buildTable(string inputFile);                                                             // Will read records from the input file,
    int search(string value);                                                                     // Accept a key value and searches index file with a call to index search

protected:
    string flatFile;
    string indexFile;
    int indexSearch(string value); // Returns the block number key of the index record
};
#endif