/*
Angel Mejia
CSE 4610
09/13/2020

Random Access file I/O
main.cpp
Driver code
*/

#include <iostream>
#include <string>
#include <fstream>
#include "sdisk.h"

using namespace std;

int main()
{
    Sdisk disk1("disk", 16, 32); // Initialize
    string block1, block2, block3, block4;

    for (auto i = 1; i < 32; i++)
    {
        block1 = block1 + "1"; // Create string of 32 1's for block 1
    }

    for (auto i = 1; i < 32; i++)
    {
        block2 = block2 + "2"; // Create string of 32 2's for block 2
    }

    disk1.putBlock(4, block1);
    disk1.getBlock(4, block3);

    cout << "Should be 32 1's : ";
    cout << block3 << endl;

    disk1.putBlock(8, block2);
    disk1.getBlock(8, block4);

    cout << "Should be 32 2's : ";
    cout << block4 << endl;

    // Debug code
    // cout << block1 << endl;
    // cout << block2 << endl;

    // cout << block1.size() << endl;
    // cout << block2.size() << endl;
    // cout << block3.size() << endl;
    // cout << block4.size() << endl;

    cout << disk1.getNumberOfBlocks() << endl;
    cout << disk1.getBlockSize() << endl;

    return 0;
}