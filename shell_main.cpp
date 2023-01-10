#include "sdisk.h"
#include "filesys.h"
#include "block.h"
#include "shell.h"
#include "table.h"

using namespace std;

int main()
{
    //
    // This main program inputs commands to the shell.
    // It inputs commands as : command op1 op2
    // You should modify it to work for your implementation.
    //
    string s;
    string command = "go";
    string op1, op2;

    int numberOfBlocks = 512; // 256
    int blockSize = 128;      // 128
    string name = "disk";
    // Sdisk disk(name, numberOfBlocks, blockSize);
    // Filesys fsys(name, numberOfBlocks, blockSize);
    Table tble(name, numberOfBlocks, blockSize, "flatFile", "indexFile");
    Shell shll(name, numberOfBlocks, blockSize);

    cout << "Type \"build\" to create the table" << endl;
    //tble.buildTable("data.txt");

    while (command != "quit")
    {
        command.clear();
        op1.clear();
        op2.clear();
        cout << "$";
        getline(cin, s);
        int firstblank = s.find(' ');
        if (firstblank < s.length())
            s[firstblank] = '#';
        int secondblank = s.find(' ');
        command = s.substr(0, firstblank);
        if (firstblank < s.length())
            op1 = s.substr(firstblank + 1, secondblank - firstblank - 1);
        if (secondblank < s.length())
            op2 = s.substr(secondblank + 1);
        if (command == "dir")
        {
            shll.dir();
        }
        if (command == "add")
        {
            // The variable op1 is the new file and op2 is the file data
            shll.add(op1, op2);
        }
        if (command == "del")
        {
            // The variable op1 is the file
            // cout << "checkpoint 0";
            shll.del(op1);
            // cout << "checkpoint 3";
        }
        if (command == "type")
        {
            // The variable op1 is the file
            shll.type(op1);
        }
        if (command == "copy")
        {
            // The variable op1 is the source file and the variable op2 is the destination file.
            shll.copy(op1, op2);
        }
        if (command == "search")
        {
            // This is the command for Project 4
            // The variable op1 is the date
            tble.search(op1);
        }
        if (command == "debug")
        {
            cout << shll.getRootSize() << endl;
            cout << shll.getBlockSize() << endl;
            cout << shll.getFatSize() << endl;
            cout << shll.getNumberOfBlocks() << endl;
        }
        if(command == "build")
        {
            tble.buildTable("data.txt");
        }
    }
    shll.fsClose();
    return 0;
}
