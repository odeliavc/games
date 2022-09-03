/*
the main pass the files name to the decoder which decode it
*/

#include "main.h"



int main(int arcg, char *argv[]) {
    int i=0;
    bool file;

  for(i=1;i<arcg; i++){/*passing the command line argument (names of files)*/
     file = readFile(argv[i]);
     if (file == false){
      return 0;
   }
  }
  return 0;
}

