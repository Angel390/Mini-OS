#include "shell.h"
#include "filesys.h"

using namespace std;

Shell::Shell() : Filesys(diskName, numberOfBlocks, blockSize)
{
    // Default Constructor
}

Shell::Shell(string diskName, int numberOfBlocks, int blockSize) : Filesys(diskName, numberOfBlocks, blockSize) // This fixed it! Will Study later
{
    this->diskName = diskName;
    this->numberOfBlocks = numberOfBlocks;
    this->blockSize = blockSize;
    cout << "Shell created!" << endl;
}

int Shell::dir()
{
    vector<string> flist = ls();
    for (int i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
    return 0;
}

int Shell::add(string file, string buffer)
{
    int code = newFile(file);
    if (code == 1)
    {
        // block the buffer
        vector<string> blocks = block(buffer, getBlockSize());
        for (int i = 0; i < blocks.size(); i++)
        {
            code = addBlock(file, blocks[i]);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Shell::del(string file)
{
    // cout << "checkpoint 1";
    int block = getFirstBlock(file);
    while (block > 0)
    {
        delBlock(file, block);
        block = getFirstBlock(file);
    }

    rmFile(file);
    return 1;
}

int Shell::type(string file)
{
    string buffer;
    int block = getFirstBlock(file);
    // cout << block;

    while (block > 0) // Can we delete blocks with this?
    {
        string b;
        readBlock(file, block, b);
        buffer += b;
        block = nextBlock(file, block);
    }
    cout << buffer << endl;
    // cout << buffer.length() << endl;
    return 0;
}

int Shell::copy(string fileOne, string fileTwo)
{
    // Everything you do a read block
    // New file file2
    // loop and addblock to file2

    string buffer;
    int block = getFirstBlock(fileOne);
    while (block > 0)
    {
        string b;
        readBlock(fileOne, block, b);
        buffer += b;
        block = nextBlock(fileOne, block);
    }

    add(fileTwo, buffer);
    return 0;
}

string Shell::getfile(Filesys& filesystem, string file)
{
    string buffer;
    int block = getFirstBlock(file);
    if(block == -1)
    {
        cout << "File does not exist!" << endl;
    }
    
    while(block > 0)
    {
        string b;
        readBlock(file, block,b);
        buffer += b;
        block = nextBlock(file, block);
    }

    return buffer;
}