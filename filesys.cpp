/*
Angel Mejia
CSE 4610

filesys.cpp
Contains the implentation for file system
*/

#include "sdisk.h"
#include "filesys.h"
#include "block.h"

using namespace std;

Filesys::Filesys() : Sdisk(diskName, numberOfBlocks, blockSize)
{
    // Default Constructor
}

Filesys::Filesys(string diskName, int numberOfBlocks, int blockSize) : Sdisk(diskName, numberOfBlocks, blockSize) // Needs work
{
    this->diskName = diskName;
    this->numberOfBlocks = numberOfBlocks;
    this->blockSize = blockSize;

    rootSize = getBlockSize() / 12;                           // Characters for root record (was 12)
    fatSize = (getNumberOfBlocks() * 5) / getBlockSize() + 1; // Increase fatsize?
    string buffer;

    getBlock(1, buffer);  // put contents of Sdisk in buffer
    if (buffer[0] == '#') // if empty
    {
        buildFS();
        fsSynch();
    }
    else
    {
        readFS(); // Fixed 11/04/2022
    }
}

int Filesys::fsClose() // Done
{

    fsSynch(); // Easiest function, just call fsSynch()
    cout << "System session saved" << endl;
    return 1;
}

int Filesys::newFile(string file) // Done
{
    /*
     This function adds an entry for the string file in ROOT with an initial first block of 0 (empty). It returns
     error codes of 1 if successful and 0 otherwise (no room or file already exists).
    */

    for (int i = 0; i < fileName.size(); i++) // First fail condition
    {
        if (fileName[i] == file) // If the file already exists
        {
            cout << "File already exists";
            return -1; // Return error code -1
        }
    }

    for (int i = 0; i < fileName.size(); i++)
    {
        if (fileName[i] == "XXXXXX") // If the file does not exist
        {
            fileName[i] = file; // set new file name to be file
            firstBlock[i] = 0;  // set the first block to start search at 0
            fsSynch();          // Synchronize
            return 1;           // Return success code 1
        }
    }
    return 0; // Return error code 0 if there is no space left
}

int Filesys::rmFile(string file) // Done
{
    for (int i = 0; i < fileName.size(); i++)
    {
        if (fileName[i] == file) // If the file is found/exists
        {
            if (firstBlock[i] == 0)
            {
                cout << "File deleted";
                fileName[i] = "XXXXXX"; // Overwrite the file with "empty" content
                fsSynch();              // Synchronize
                return 1;               // Return sucess code 1
            }
            else
            {
                return 0; // If the file exists but it did not start from 0, return error code 0
            }
        }
    }
    cout << "File does not exist";
    return -1; // File was not found, return error code -1
}

int Filesys::getFirstBlock(string file) // Done
{
    for (int i = 0; i < fileName.size(); i++)
    {
        if (fileName[i] == file) // If the file exists
        {
            return firstBlock[i]; // Return the first blocks in root (it is a vector)
        }
    }
    return -1; // If not found, return -1 error code
}

int Filesys::addBlock(string file, string block) // Done 10/11/22
{
    /*
     This function adds a block of data stored in the string buffer to the end of file F and returns the
     blocknumber.
     It returns error code 0 if the file does not exist, and returns -1 if there are no available blocks (filesystem is full!).
    */

    int allocate = fat[0]; // This represents the block that will be allocated (A location on the fat table)

    if (allocate == 0) // If there is no space left on the disk (no spaces on the fat table)
    {
        cout << "No space left on disk "; // Print error message
        return -1;
    }

    int blockID = getFirstBlock(file); // Initialize the first block of the file to block ID
    // cout << blockID << "Block ID" << endl;
    if (blockID == -1) // If it's -1, then it doesn't exist
    {
        cout << "File does not exist "; // Print error message
        return 0;
    }

    if (blockID == 0) // Update the root to the new location on the fat table if the block ID is found
    {
        for (int i = 0; i < rootSize; i++)
        {
            if (fileName[i] == file) // If the file is found in the file name table
            {
                firstBlock[i] = allocate; // allocate the first available block ERROR! was set to ==
                break;                    // You need to exit as soon as this happens
            }
        }
    }
    else
    {
        while (fat[blockID] != 0) // Keep searching for an available block
        {
            blockID = fat[blockID];
        }
        fat[blockID] = allocate;
    }

    fat[0] = fat[fat[0]];
    fat[allocate] = 0;
    fsSynch();
    putBlock(allocate, block);
    cout << "Block added!: " << allocate;
    // cout << getFirstBlock(file) << "GFB" << endl;

    return allocate;
}

int Filesys::delBlock(string file, int blockNumber) // Done 10/20/22
{
    /*
     The function removes block numbered blocknumber from file and returns an error code of 1 if successful
     and 0 otherwise

     Deallocates the block
     Update the root, get first block
     next block fat[block number]

     You want to over write the block and put it on the free list (fat[0])
    */

    int block = getFirstBlock(file);
    bool flag = fileBlockCheck(file, blockNumber);

    if (!flag)
    {
        return 0;
    }

    if (block == blockNumber)
    {
        // Deleting first block in file
        for (int i = 0; i < fileName.size(); i++)
        {
            if (fileName[i] == file)
            {
                firstBlock[i] = fat[blockNumber]; // Was firstBlock[i]
                break;
            }
        }
    }
    else
    {
        // Not first block
        while (fat[block] != blockNumber)
        {
            block = fat[block];
        }
        fat[block] = fat[blockNumber];
    }
    fat[0] = blockNumber; // Update the free list
    cout << "Block deleted!" << endl;
    return 1;
}

int Filesys::readBlock(string file, int blockNumber, string &buffer) // Done 10/18/2022
{
    /*
     gets block numbered blocknumber from file and stores the data in the string buffer. It returns an error code
     of 1 if successful and 0 otherwise.
    */

    if (fileBlockCheck(file, blockNumber))
    {
        getBlock(blockNumber, buffer);
        return 1;
    }
    return 0;
}

int Filesys::writeBlock(string file, int blockNumber, string buffer) // Done 10/18/2022
{
    /*
     writes the buffer to the block numbered blocknumber in file.
     It returns an appropriate error code.
    */

    if (fileBlockCheck(file, blockNumber))
    {
        putBlock(blockNumber, buffer);
        return 1;
    }
    return 0;
}

int Filesys::nextBlock(string file, int blockNumber) // Done 10/18/2022
{
    /*
     returns the number of the block that follows blocknumber in file. It will return 0 if blocknumber is the last
     block and -1 if some other error has occurred (such as file is not in the root directory, or blocknumber is
     not a block in file.)
    */

    if (fileBlockCheck(file, blockNumber)) // Find out if the block number belongs to the file
    {
        return fat[blockNumber]; // If it does, return the block number
    }

    return -1; // Error
}

int Filesys::buildFS() // Done
{
    for (int i = 0; i < rootSize; i++) // While iterator is less than root size
    {
        fileName.push_back("XXXXXX"); // Fill the file vector with "empty" content X
        firstBlock.push_back(0);      // the first block will always start at 0
    }

    fat.push_back(2 + fatSize); // The first data block
    fat.push_back(-1);          // The second data block

    for (int i = 0; i < fatSize; i++)
    {
        fat.push_back(-1); // -1 are areas not accessible by fat
    }

    for (int i = 2 + fatSize; i < getNumberOfBlocks(); i++) // Data blocks
    {
        fat.push_back(i + 1);
    }
    fat[fat.size() - 1] = 0; // End of fat is 0
    // fsSynch();               // added
    cout << "Filesystem created!" << endl;
    return 1;
}

int Filesys::readFS() // Done
{
    string buffer;
    getBlock(1, buffer);
    istringstream instream1;
    instream1.str(buffer);

    cout << "Reading root" << endl;
    fileName.clear();
    firstBlock.clear();
    for (int i = 0; i < rootSize; i++)
    {
        string f;
        int b;
        instream1 >> f >> b;
        fileName.push_back(f);
        firstBlock.push_back(b);
    }

    cout << "Reading fat" << endl;
    // cout << buffer << endl;
    buffer.clear();
    istringstream instream2;
    for (int i = 0; i < fatSize; i++)
    {
        string b;
        getBlock(i + 2, b);
        buffer += b;
    }

    instream2.str(buffer);
    fat.clear(); // added
    // cout << buffer;
    for (int i = 0; i < getNumberOfBlocks(); i++)
    {
        int k;
        instream2 >> k;
        fat.push_back(k);
    }

    return 1;
}

int Filesys::fsSynch() // Done
{
    // Synchronize the filesystem
    ostringstream outStreamOne; // output string stream

    for (int i = 0; i < fileName.size(); i++) // Fixed? was firstblock size
    {
        // cout << firstBlock[i] << endl;
        outStreamOne << fileName[i] << " " << firstBlock[i] << " "; // Output file name and the first block
    }

    ostringstream outStreamTwo;

    for (int i = 0; i < fat.size(); i++)
    {
        outStreamTwo << fat[i] << " "; // Output the contents of fat
    }

    // Makes sure that the size is correct
    string bufferOne = outStreamOne.str(); // Convert to string and store it in the buffers
    string bufferTwo = outStreamTwo.str();

    // cout << bufferOne << " " << bufferTwo << endl;

    vector<string> blockOne = block(bufferOne, getBlockSize());
    vector<string> blockTwo = block(bufferTwo, getBlockSize());

    putBlock(1, bufferOne); // Write the block into the file (was blockone[0])

    for (int i = 0; i < blockTwo.size(); i++)
    {
        putBlock(2 + i, blockTwo[i]); // had fatsize
    }

    return 1;
}

int Filesys::getRootSize() // Done
{
    return rootSize;
}

int Filesys::getFatSize() // Done
{
    return fatSize;
}

vector<string> Filesys::ls()
{
    vector<string> flist;
    for (int i = 0; i < fileName.size(); i++)
    {
        if (fileName[i] != "XXXXXX")
        {
            flist.push_back(fileName[i]);
        }
    }
    return flist;
}

bool Filesys::fileBlockCheck(string file, int blockNumber) // Done 10/18/2022
{
    int block = getFirstBlock(file);

    if (block == -1) // If the block is reserved
    {
        return false;
    }

    while (block != 0) // While not at the end of file
    {
        if (block == blockNumber)
        {
            return true;
        }
        else
        {
            block = fat[block];
        }
    }
    return false;
}