/*
Angel Mejia
CSE 4610

block.cpp
Contains the block program that is used in the file system
Sources:
https://cplusplus.com/reference/string/string/substr/
*/

#include "block.h"
#include <string>
#include <vector>

using namespace std;

vector<string> block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b

    vector<string> blocks;
    int numberOfBlocks = 0;

    if (buffer.length() % b == 0)
    {
        numberOfBlocks = buffer.length() / b;
    }
    else
    {
        numberOfBlocks = buffer.length() / b + 1;
    }

    string tempBlock;
    for (int i = 0; i < numberOfBlocks; i++)
    {
        tempBlock = buffer.substr(b * i, b);
        blocks.push_back(tempBlock);
    }

    int lastBlock = blocks.size() - 1;

    for (int i = blocks[lastBlock].size(); i < b; i++)
    {
        blocks[lastBlock] += "#";
    }

    return blocks;
}