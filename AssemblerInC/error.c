/*
handling all the error messages - prining it to user
*/

 #include "error.h"

bool mistake;

char myFileName[NAMEFILELENGTH];


void printError(int rowNum,char * str ){
  mistake=true;
  printf("In file %s there is a problem in row number %d : %s",myFileName,rowNum,str);
}

void setError(bool err)
{
mistake=err;

}

bool getError()
{
return mistake;
}


void setFileName(char  namfOfFile[])
{

  strcpy(myFileName,namfOfFile);
}
