/*
handling the treatment of creating files, and writing on them.
it is using the linked lists derive  the information to print 
*/

#include "fileHandle.h"

const char * data[] = {
  ".db",
  ".dh",
  ".dw",
  ".asciz"
};

bool readFile(char * str) {
  char row[MAX_LINE];
  char newRow[MAX_LINE];
  char fileName[MAX_LINE];
  FILE ** fp;
  FILE * fdp;
  char * ptr;
  char * pstr;
  nodeRow * head;
  nodeRow * tempRow;
  nodeRow * tail;
  nodeRow * printNode;
  nodeLabel * headLabel;
  int endIc = 100;
  int endDc = 0;
  int i;
  bool isData;
  int countData=0;

  setError(false);
  initIC();
  fp = (FILE ** ) malloc(sizeof(FILE ** ));
  * fp = fopen(str, "r");
  ptr = str;
  while ( * ptr != '\0') {
    ptr++;
  }
  ptr = ptr - 3;
  if ( * fp == NULL) {
    printf("The file \"%s\" is not exist!", str);
    return false;
  }
  if (strcmp(ptr, ".as") != 0) {
    printf("The file \"%s\" not in correct format!", str);
    fclose( * fp);
    return false;
  }
  setFileName(str);
  * ptr = '\0';
  /* the file exist and the name in correct format!*/
  headLabel = makeNodeLabel();
  head = makeNodeRow();
  tail = head;
  fdp = fopen(FILE_DATA, "w+");
  while ((ptr = fgets(row, MAX_LINE, * fp)) != NULL) {
    isData = false;
    pstr = ptr;
    /*cleaning \n */
    while ( * pstr != '\n' && * pstr != '\0') {
      pstr++;
    }
    * pstr = '\0';
    pstr = ptr;
    if (counterChar(pstr, ':') == 1) {
      while ( * pstr != ':') {

        pstr++;
      }
      pstr++;
    }

    strcpy(newRow, pstr);
    strcpy(newRow, skipSpace(newRow));
    findFirstWord(newRow);
    for (i = 0; i < dAmount; i++) {
      if (strcmp(data[i], newRow) == 0) {
        fprintf(fdp, "%s\n", ptr);
        isData = true;
      }
    }
    if (isData)
      countData++;
    if (isData == false) {
      if ((tempRow = decodeRowPass1(ptr, headLabel)) != NULL) {
        tail -> next = tempRow;
        tail = tempRow;
        if ((tail -> myrow.type == 'R') || (tail -> myrow.type == 'I') || (tail -> myrow.type == 'J'))
          endIc = tail -> myrow.ic;
      }
    }
    ptr++;
  }
  fclose(fdp);
  fclose( * fp);
  * fp = fopen(FILE_DATA, "r");
  if (countData >0){
  while ((ptr = fgets(row, MAX_LINE, * fp)) != NULL) {
    pstr = ptr;
    /*cleaning \n */
    while ( * pstr != '\n' && * pstr != '\0') {
      pstr++;
    }
    * pstr = '\0';
    if ((tempRow = decodeRowPass1(ptr, headLabel)) != NULL) {
      tail -> next = tempRow;
      tail = tempRow;
      if ((tail -> myrow.type == 'R') || (tail -> myrow.type == 'I') || (tail -> myrow.type == 'J'))
        endIc = tail -> myrow.ic;
      if (tail -> myrow.type == 'D')
        countData++;
    }
    else {
      freeData(head->next);
      return false;
    }
  }
  }
  if(countData>0){
    endDc = tail -> myrow.image -> dc;
    endDc = endDc + tail -> myrow.numArgumentDI;
  }
  endIc += 4;
  endIc -= 100;
  printNode = head -> next;
  tempRow = head -> next;
  while (tempRow != NULL) {
    decodeRowPass2(tempRow, headLabel);
    tempRow = tempRow -> next;
  }
  if (getError() == false) {
    strcpy(fileName, str);
    printObj(printNode, fileName, endIc, endDc);

    strcpy(fileName, str);
    printEXT(head, fileName);
    strcpy(fileName, str);
    printEnt(head, fileName, headLabel);
  }
remove(FILE_DATA);
  free(fp);
  freeData(head->next);
  return true;
}

void printObj(nodeRow * myPrintNood, char * fileName, int endIc, int endDc) {
  FILE * fp;
  nodeRow * printNood;
  int line, block1, block2, block3, block4, num, address;
  dataImage * pIamge;
  printNood = myPrintNood;
  fp = fopen(strcat(fileName, EXTEN_OBJECT), "w");
  fprintf(fp, "     %d %d  \n", endIc, endDc);
  while (printNood != NULL) {
    if (printNood -> myrow.type == 'R') {
      line = (printNood -> myrow.opcode << 26);
      line = line | (printNood -> myrow.rs << 21);
      line = line | (printNood -> myrow.rt << 16);
      line = line | (printNood -> myrow.rd << 11);
      line = line | (printNood -> myrow.funct << 6);
      block1 = line & 255;
      block2 = (line >> 8) & 255;
      block3 = (line >> 16) & 255;
      block4 = (line >> 24) & 255;
      fprintf(fp, "%04d %02X %02X %02X %02X \n", printNood -> myrow.ic, block1, block2, block3, block4);

    }
    if (printNood -> myrow.type == 'I') {
      line = (printNood -> myrow.opcode << 26);
      line = line | (printNood -> myrow.rs << 21);
      line = line | (printNood -> myrow.rt << 16);
      line = line | (printNood -> myrow.immed & 65535);
      block1 = line & 255;
      block2 = (line >> 8) & 255;
      block3 = (line >> 16) & 255;
      block4 = (line >> 24) & 255;
      fprintf(fp, "%04d %02X %02X %02X %02X \n", printNood -> myrow.ic, block1, block2, block3, block4);

    }
    if (printNood -> myrow.type == 'J') {
      line = (printNood -> myrow.opcode << 26);
      line = line | (printNood -> myrow.reg << 25);
      line = line | printNood -> myrow.addj;
      block1 = line & 255;
      block2 = (line >> 8) & 255;
      block3 = (line >> 16) & 255;
      block4 = (line >> 24) & 255;
      fprintf(fp, "%04d %02X %02X %02X %02X \n", printNood -> myrow.ic, block1, block2, block3, block4);

    }

    if (printNood -> myrow.type == 'D') {
      address = printNood -> myrow.ic;
      if (printNood -> myrow.index == 0) {
        if (printNood -> myrow.image != NULL) {
          pIamge = printNood -> myrow.image;
          pIamge = pIamge -> next;
          while (pIamge != NULL) {
            num = pIamge -> num;
            num = num & 255;
            printingByAddressFile(address, num, fp);
            address++;
            pIamge = pIamge -> next;
          }
        }
      }
      if (printNood -> myrow.index == 1) {
        /*dh*/
        if (printNood -> myrow.image != NULL) {
          pIamge = printNood -> myrow.image;
          pIamge = pIamge -> next;
          while (pIamge != NULL) {
            num = pIamge -> num;
            num = num & 65535;
            printingByAddressFile(address, num & 255, fp);
            address++;
            printingByAddressFile(address, ((num >> 8) & 255), fp);
            address++;
            pIamge = pIamge -> next;
          }
        }
      }
      if (printNood -> myrow.index == 2) {
        /*dw*/
        if (printNood -> myrow.image != NULL)
          pIamge = printNood -> myrow.image;
        pIamge = pIamge -> next;
        while (pIamge != NULL) {
          num = pIamge -> num;
          printingByAddressFile(address, num & 255, fp);
          address++;
          printingByAddressFile(address, ((num >> 8) & 255), fp);
          address++;
          printingByAddressFile(address, ((num >> 16) & 255), fp);
          address++;
          printingByAddressFile(address, ((num >> 24) & 255), fp);
          address++;
          pIamge = pIamge -> next;
        }
      }
      if (printNood -> myrow.index == 3) {
        /*asciz*/
        if (printNood -> myrow.image != NULL) {
          pIamge = printNood -> myrow.image;
          pIamge = pIamge -> next;
          while (pIamge != NULL) {
            num = pIamge -> c;
            num = num & 255;
            printingByAddressFile(address, num, fp);
            address++;
            pIamge = pIamge -> next;
          }
          printingByAddressFile(address, '\0', fp);

          address++;
        }
      }

    }

    printNood = printNood -> next;
    /*   printf("\n");*/
  }
  fclose(fp);
}

void printingByAddressFile(int address, int p, FILE * fp) {
  if (address % 4 == 0)
    fprintf(fp, "%04d %02X ", address, (int) p);
  if (address % 4 == 1 || address % 4 == 2)
    fprintf(fp, "%02X ", p);
  if (address % 4 == 3)
    fprintf(fp, "%02X\n", (int) p);

}

void printEXT(nodeRow * head, char * fileName) {
  FILE * fp;
  nodeRow * ptr = head;
  nodeRow * str = head;
  bool exter =false;
    while(ptr != NULL){
  if (ptr->myrow.type == '2')
    exter = true;
  ptr = ptr->next;
}
  ptr =head;
  if(exter){
  fp = fopen(strcat(fileName,EXTEN_EXTERN), "w");
  while (ptr != NULL) {
    if (ptr -> myrow.type == '2') {
      str = head;
      while (str != NULL) {
        if ((str -> myrow.type == 'J') || (str -> myrow.type == 'I')) {
          if (strcmp(str -> myrow.nameLabel, ptr -> myrow.nameLabel) == 0)
            fprintf(fp, "%s %04d\n", str -> myrow.nameLabel, str -> myrow.ic);
        }
        str = str -> next;
      }
      str = head;
    }
    ptr = ptr -> next;
  }
  fclose(fp);
  }
}

void printEnt(nodeRow * head, char * fileName, nodeLabel * headLabel) {
  FILE * fp;
  nodeRow * ptr = head;
  nodeLabel * pLabel = headLabel;
  bool entry = false;
  while(ptr != NULL){
  if (ptr->myrow.type == '1')
    entry = true;
  ptr = ptr->next;
}
  ptr =head;
  if(entry){
  fp = fopen(strcat(fileName, EXTEN_ENTERY), "w");
  while (ptr != NULL) {
    if (ptr -> myrow.type == '1')
      while (pLabel != NULL) {
        if (strcmp(ptr -> myrow.nameLabel, pLabel -> mylabel.name) == 0)
          fprintf(fp, "%s %04d\n", pLabel -> mylabel.name, pLabel -> mylabel.ic);
        pLabel = pLabel -> next;
      }
    pLabel = headLabel;
    ptr = ptr -> next;
  }
  fclose(fp);
  }
}
