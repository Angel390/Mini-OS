/*
Angel Mejia
CSE 4610
09/13/2020

Random Access file I/O
main.cpp
Driver code
*/

#include "sdisk.h"
#include "filesys.h"
#include "block.h"

using namespace std;

// You can use this to test your Filesys class

int main()
{
  const int numberOfblocks = 256; // 256
  const int blockSize = 128;      // 128
  Sdisk diskOne("disk_one", numberOfblocks, blockSize);
  Filesys fsys("disk_one", numberOfblocks, blockSize);
  
  fsys.newFile("file_one");
  fsys.newFile("file_two");

  string bfile1;
  string bfile2;

  for (int i = 1; i <= 1024; i++)
  {
    bfile1 += "1";
  }

  vector<string> blocks = block(bfile1, blockSize);
  int blockNumber = 0;

  for (int i = 0; i < blocks.size(); i++)
  {
    blockNumber = fsys.addBlock("file_one", blocks[i]);
    cout << endl;
  }
  return 0;
}
