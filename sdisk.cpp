/*
Angel Mejia
CSE 4610
09/13/2020

Random Access file I/O
sdisk.cpp
Contains the implentation

Sources:
https://www.learncpp.com/cpp-tutorial/random-file-io/
https://cplusplus.com/reference/istream/istream/seekg/
https://www.geeksforgeeks.org/set-position-with-seekg-in-cpp-language-file-handling/
https://www.geeksforgeeks.org/ostreamseekppos-method-in-c-with-exmaples/
https://cplusplus.com/reference/ostream/ostream/put/
https://cplusplus.com/reference/istream/istream/get/

Work on the Filesys constructors where you create ROOT and FAT
Work on fssynch that writes root and fat to the disk
*/

#include "sdisk.h"

Sdisk::Sdisk()
{
    // Default constructor
}

Sdisk::Sdisk(string diskName, int numberOfBlocks, int blockSize) // Had to be fixed 11/05/2022
{
    // "Format" the drive
    this->diskName = diskName;
    this->numberOfBlocks = numberOfBlocks;
    this->blockSize = blockSize;

    ifstream indisk; // Initialize the input file
    indisk.open(diskName.c_str(), ios::in);
    if (indisk.fail()) // If file does not open
    {
        indisk.close();                           // Close input file, as it is no longer needed
        ofstream outdisk;                         // initialize the output file
        outdisk.open(diskName.c_str(), ios::out); // Open the output file for writting
        for (int i = 0; i < numberOfBlocks * blockSize; i++)
        {
            outdisk.put('#'); // Fill it with # Symbols
        }
        cout << "Sdisk created!" << endl;
        outdisk.close();
    }
    else // If file opens, check if it is the correct size
    {
        ifstream infile;                        // Initialize the input file
        infile.open(diskName.c_str(), ios::in); // open for reading
        int counter = 0;
        char c;
        infile.get(c); // Get one character at a time
        while (infile.good())
        {
            counter++;     // Count number of # characters
            infile.get(c); // Retrive the characters from the string (unformatted)
            // cout << c;
        }
        if (counter = numberOfBlocks * blockSize) // If the disk size = number of blocks * block size
        {
            printf("Disk size is correct: %i\n", counter);
        }
        infile.close();
    }
}

int Sdisk::getBlock(int blockNumber, string &buffer) // Done and works
{
    // Retrieves a block blocknumber from the disk and stores the data in the string buffer.
    fstream iofile; // Initialize input/output file for reading
    iofile.open(diskName.c_str(), ios::in);

    if (blockNumber < 0 || blockNumber >= numberOfBlocks) // Throw error if file is invalid
    {
        // If block is less than 0 or block number is less than or equal to number blocks, it does not exist
        cout << "Block doesn't exist";
        return 0;
    }

    // Start at block number b * blocksize
    iofile.seekg(blockNumber * blockSize); // Seekg allows you to seek an arbitrary position in a file (with 512 positions)
    buffer.clear();                        // Clear the buffer vector
    char c;

    for (int i = 0; i < blockSize; i++) // Iterate the block to initialize the buffer
    {
        iofile.get(c);       // Retrieve character from the file
        buffer.push_back(c); // And add it to the buffer vector
    }
    iofile.close();
    return 1;
}

int Sdisk::putBlock(int blockNumber, string buffer) // D=Broken fixed 11/5/22
{
    // Write the string buffer to block blocknumber.
    fstream iofile;
    iofile.open(diskName.c_str(), ios::out | ios::in);

    if (blockNumber < 0 || blockNumber >= numberOfBlocks) // Throw error if file is invalid
    {
        cout << "Block doesn't exist";
        return 0;
    }

    iofile.seekp(blockSize * blockNumber); // Seekp is used to set the position of the pointer
    for (int i = 0; i < blockSize && i < buffer.size(); i++)
    {
        iofile.put(buffer[i]); // Insert the character into the stream
    }
    iofile.close();
    return 1;
}

int Sdisk::getNumberOfBlocks() // Done
{
    return numberOfBlocks;
}

int Sdisk::getBlockSize() // Done
{
    return blockSize;
}