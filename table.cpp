#include "table.h"

using namespace std;

Table::Table() : Filesys(diskName, numberOfBlocks, blockSize)
{
    // Default Constructor
}

Table::Table(string diskName, int numberOfBlocks, int blockSize, string flatFile, string indexFile) : Filesys(diskName, numberOfBlocks, blockSize)
{
    this->diskName = diskName;
    this->numberOfBlocks = numberOfBlocks;
    this->blockSize = blockSize;

    // Create empty files
    newFile(flatFile);
    this->flatFile = flatFile;
    newFile(indexFile);
    this->indexFile = indexFile;

    cout << "Table created!" << endl;
}

int Table::buildTable(string inputFile)
{
    ostringstream outstream;
    // newFile(indexFile);
    // newFile(flatFile);
    ifstream infile;
    infile.open(inputFile.c_str());
    string rec;           // record to process each line of the input file
    getline(infile, rec); // Read the first record outside the loop
    while (infile.good())
    {
        string date = rec.substr(0, 5); // Starts at position 0 and ends in our sentinel, replace 88888 with 5
        vector<string> blocks = block(rec, getBlockSize());
        int b = addBlock(flatFile, blocks[0]); // write that block to addblock
        outstream << date << " " << b << " ";
        getline(infile, rec); // no more lines to read

    }                                 // Flatfile should be created now
    outstream << "88888" << 0 << " "; // End, this will be the last record, added!
    string buffer = outstream.str();
    vector<string> blocks = block(buffer, getBlockSize());
    // Now we write it out to the index file
    for (int i = 0; i < blocks.size(); i++)
    {
        addBlock(indexFile, blocks[i]);
    }
    return 1;
}

int Table::search(string value)
{
    int block = indexSearch(value);
    if (block == 0) // Was 0
    {
        cout << "record not found";
        return 0;
    }
    else
    {
        string buffer;
        int error = readBlock(flatFile, block, buffer); // was flatfile
        // cout << "This is the block: " << block;
        cout << buffer;
        return 1;
    }
    return 0;
}

int Table::indexSearch(string value)
{
    /*
    read in index file
    (istringstream)
    reads value block #
    */

    string buffer;
    int block = getFirstBlock(indexFile);

    // cout << block;
    while (block != 0)
    {
        string b; // temporary buffer
        int error = readBlock(indexFile, block, b);
        buffer += b;
        block = nextBlock(indexFile, block);
    }
    // cout << buffer << endl;
    // cout << block;

    istringstream instream; // to read all the index records
    string s;
    int b;
    instream.str(buffer);
    instream >> s >> b;

    while (s != "88888") // While there is still an index record
    {
        if (s == value)
            return b;
        instream >> s >> b;
    }
    return 0;
}
