/*
Angel Mejia
CSE 4610

shell.h
Contains the function stubs for the class functions
*/

#ifndef SHELL_H
#define SHELL_H

#include "filesys.h"

using namespace std;

class Shell : public Filesys
{
public:
    Shell();
    Shell(string diskName, int numberOfBlocks, int blockSize); // This will create a shell for the file system
    int dir();                                                 // This will list the files in the root directory
    int add(string file, string buffer);                       // Add a new file using buffer as data
    int del(string file);                                      // Delete file
    int type(string file);                                     // List contents of file
    int copy(string file1, string file2);                      // Copies file
    string getfile(Filesys &filesystem, string file);          // Practice Exam
};
#endif