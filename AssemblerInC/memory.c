/*
create and handling linked list for stroing the information we gather from the rows
*/
#include "memory.h"

void printBit1(int x) {
  int w = sizeof(x);
  int i;
  int unsigned mask = 1;
  mask = (mask << (31));
  for (i = 0; i < 8 * w; i++) {
    printf("%d", ((x & mask) >> (8 * w - i - 1)));
    mask = mask >> 1;
  }
  printf("\n");
}

nodeLabel * makeNodeLabel() {
  nodeLabel * pNode = (nodeLabel * ) malloc(sizeof(nodeLabel));
  pNode -> next = NULL;
  return pNode;
}

dataImage * makeDataImage() {
  dataImage * pNode = (dataImage * ) malloc(sizeof(nodeLabel));
  pNode -> next = NULL;
  return pNode;
}

nodeRow * makeNodeRow() {
  nodeRow * pNode = (nodeRow * ) malloc(sizeof(nodeRow));
  pNode -> next = NULL;
  pNode -> myrow.ic = 0;
  pNode -> myrow.type = '\0';
  pNode -> myrow.rs = 0;
  pNode -> myrow.rt = 0;
  pNode -> myrow.rd = 0;
  pNode -> myrow.opcode = 0;
  pNode -> myrow.funct = 0;
  pNode -> myrow.immed = 0;
  pNode -> myrow.reg = 0;
  pNode -> myrow.image = NULL;
  pNode -> myrow.isLabel = false;
  pNode -> myrow.branch = false;
  strcpy(pNode -> myrow.nameLabel, "/0");
  return pNode;
}


void printMyNodes(nodeRow * printNood) {
  int line, block1, block2, block3, block4, num, address;
  dataImage * pIamge;
  while (printNood != NULL) {
    printf("the ic is: %d\n", printNood -> myrow.ic);
    printf("the type is %c\n", printNood -> myrow.type);
    printf("***the addersss is: %d\n", printNood -> myrow.addj);
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
      printf("%04d %02X %02X %02X %02X0 \n", printNood -> myrow.ic, block1, block2, block3, block4);
      printf("THE ANSWER:%d\n", line);
      printf("the opcode is: %d\n", printNood -> myrow.opcode);
      printf("the rs is: %d\n", printNood -> myrow.rs);
      printf("the rd is: %d\n", printNood -> myrow.rd);
      printf("the rt is: %d\n", printNood -> myrow.rt);
      printf("the funct is %d\n", printNood -> myrow.funct);
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
      printf("%04d %02X %02X %02X %02X \n", printNood -> myrow.ic, block1, block2, block3, block4);
      printf("the opcode is: %d\n", printNood -> myrow.opcode);
      printf("the rs is: %d\n", printNood -> myrow.rs);
      printf("the rt is: %d\n", printNood -> myrow.rt);
      printf("the immed is %d\n", printNood -> myrow.immed);
    }
    if (printNood -> myrow.type == 'J') {
      line = (printNood -> myrow.opcode << 26);
      line = line | (printNood -> myrow.reg << 25);
      line = line |  printNood -> myrow.addj;
      block1 = line & 255;
      block2 = (line >> 8) & 255;
      block3 = (line >> 16) & 255;
      block4 = (line >> 24) & 255;
      printf("%04d %02X %02X %02X %02X \n", printNood -> myrow.ic, block1, block2, block3, block4);
      printf("the opcode is: %d\n", printNood -> myrow.opcode);
      printf("the reg is: %d\n", printNood -> myrow.reg);
      printf("the addres is: %d\n", printNood -> myrow.addj);
    }
    if (printNood -> myrow.type == 'D') {
      address = printNood -> myrow.ic;
      printf("the dc is:%d\n", printNood -> myrow.image -> dc);
      if (printNood -> myrow.index == 0) {
        /*db*/
        if (printNood -> myrow.image != NULL) {
          pIamge = printNood -> myrow.image;
          pIamge = pIamge -> next;
          while (pIamge != NULL) {
            num = pIamge -> num;
            num = num & 255;
            printingByAddress(address, num);
            address++;
            pIamge = pIamge -> next;
          }
          printf("\n");
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

            printingByAddress(address, num & 255);
            address++;

            printingByAddress(address, ((num >> 8) & 255));
            address++;

            pIamge = pIamge -> next;
          }
          printf("\n");
        }
      }
      if (printNood -> myrow.index == 2) {
        /*dw*/
        if (printNood -> myrow.image != NULL)
          pIamge = printNood -> myrow.image;
        pIamge = pIamge -> next;
        while (pIamge != NULL) {
          num = pIamge -> num;
          printingByAddress(address, num & 255);
          address++;
          printingByAddress(address, ((num >> 8) & 255));
          address++;
          printingByAddress(address, ((num >> 16) & 255));
          address++;
          printingByAddress(address, ((num >> 24) & 255));
          address++;
          pIamge = pIamge -> next;
        }
        printf("\n");
      }
            if (printNood -> myrow.index == 3) {
        /*asciz*/
        if (printNood -> myrow.image != NULL) {
          pIamge = printNood -> myrow.image;
          pIamge = pIamge -> next;
          while (pIamge != NULL) {
            num = pIamge -> c;
            num = num & 255;
            printingByAddress(address, num);
            address++;
            pIamge = pIamge -> next;
          }
                printingByAddress(address, '\0');

          address++;
          printf("\n");
        }
      }

    }
    printNood = printNood -> next;
    printf("\n");
  }

}


void freeDataImage(dataImage * head) {

  dataImage * temp = head;
  temp = temp -> next;
  while (temp != NULL) {
    free(head);
    head = temp;
    temp = temp -> next;
  }
  free(head);
}

int countImage(dataImage * head) {
  int counter = 0;
  dataImage * temp = head;
  temp = temp -> next;
  while (temp != NULL) {
    counter++;
    temp = temp -> next;
  }
  return counter;
}

void printingByAddress(int address, int p) {
  if (address % 4 == 0)
    printf("%04d  %02X ", address, (int) p);
  if (address % 4 == 1 || address % 4 == 2)
    printf("%02X ", p);
  if (address % 4 == 3)
    printf("%02X\n", (int) p);

}



