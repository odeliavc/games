/*the decoder knows to split all the parties of the rows and interpret it - and determine if there is a error or not. also it knows what error occurred.
*/

#include "decoder.h"


const char entry[] = ".entry";
const char exter[] = ".extern";
const char * guidance[] = {
  ".db",
  ".dh",
  ".dw",
  ".asciz"
};

const char * commandsR[] = {
  "add",
  "sub",
  "and",
  "or",
  "nor",
  "move",
  "mvhi",
  "mvlo"
};

const char * commandsI[] = {
  "addi",
  "subi",
  "andi",
  "ori",
  "nori",
  "bne",
  "beq",
  "blt",
  "bgt",
  "lb",
  "sb",
  "lw",
  "sw",
  "lh",
  "sh"
};

const char * commandsJ[] = {
  "jmp",
  "la",
  "call",
  "stop"
};

const int numParamR[] = {
  3,
  3,
  3,
  3,
  3,
  2,
  2,
  2
};

const int numParamI = 2;

const int numParamJ = 1;

const int functR[] = {
  1,
  2,
  3,
  4,
  5,
  1,
  2,
  3
};

const int opcodeR[] = {
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1
};

const int opcodeI[] = {
  10,
  11,
  12,
  13,
  14,
  15,
  16,
  17,
  18,
  19,
  20,
  21,
  22,
  23,
  24
};

const int opcodeJ[] = {
  30,
  31,
  32,
  63
};

const int iType[] = {
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3
};

int numRowPass1 = 0;
int numRowPass2 = 0;
int IC = 100; /*DEFINE*/
int DC = 0;

dataImage * checkImageData(char * str, int index) {
  char row[MAX_LINE];
  char number[MAX_LINE];
  int comma, i;
  char * ptr;
  char * headNum;
  bool wrong = false;
  dataImage * pImage;
   char * firstChar;
  dataImage * head = makeDataImage();
  dataImage * tail = head;
  strcpy(row, str);
  ptr = row;
  headNum = ptr;
  if (index != 3) {
    comma = counterChar(row, ',');
    for (i = 0; i < comma; i++) {
      headNum = ptr;
      firstChar = skipSpace(headNum);
      if (( * firstChar != '+') && ( * firstChar != '-') && (!(( * firstChar >= '0') && ( * firstChar <= '9')))) {
        printError(numRowPass1, DATA_WRONG_CHAR);
        return NULL;
      }
      if (( * firstChar == '+') || ( * firstChar == '-')) {
        firstChar++;
        if (!(( * firstChar >= '0') && ( * firstChar <= '9'))) {
          printError(numRowPass1, DATA_WRONG_CHAR_ONLY_NUM);
          return NULL;
        }
      }
      while ( * ptr != ',') {
        ptr++;
      }
      * ptr = '\0';
      strcpy(number, headNum);
      if ((pImage = checkNumberImageData(numRowPass1, number, index, DC)) != NULL) {
        tail -> next = pImage;
        tail = pImage;
      } else {
        wrong = true;
      }
      ptr++;
    }
    strcpy(number, ptr);
    if ((pImage = checkNumberImageData(numRowPass1, number, index, DC)) != NULL) {
      tail -> next = pImage;
      tail = pImage;
    } else {
      wrong = true;
    }
    if (wrong == true) {
      freeDataImage(pImage);
      return NULL;
    }
  }
  if (index == 3) {
    checkAsciz(numRowPass1, headNum);
    headNum++;
    strcpy(number, headNum);
    if ((pImage = checkNumberImageData(numRowPass1, number, index, DC)) != NULL) {
      headNum++;
      tail -> next = pImage;
      tail = pImage;
      while ( * headNum != '\"') {
        strcpy(number, headNum);
        if ((pImage = checkNumberImageData(numRowPass1, number, index, DC)) != NULL) {
          tail -> next = pImage;
          tail = pImage;
          headNum++;
          str++;
        } else {
          wrong = true;
        }
      }
    }
    if (wrong == true) {
      freeDataImage(pImage);
      printf("jjjjj\n");
      return NULL;
    }
  }
  return head;

}

bool checkAsciz(int rowNum, char * str) {
  char * ptr;
  ptr = str;
  strcpy(ptr, skipSpace(ptr));
  if ( * ptr != '\"') {
    printError(rowNum, ASC_START);
    return false;
  }
  while ( * ptr != '\0') {
    if ( * ptr == '\"')
      return true;
    ptr++;
  }
  printError(rowNum, ASC_END);
  return false;
}

dataImage * checkNumberImageData(int rowNum, char * str, int index, int dc) {
  int num;
  dataImage * ptr;
  strcpy(str, skipSpace(str));
  strcpy(str, skipSpaceRight(str));
  if (index != 3) {
    if (counterChar(str, ' ') > 0) {
      printError(rowNum, DATA_SPACE);
      return NULL;
    }

    if (checkNumber(str) == false) {
      printError(rowNum, DATA_WRONG_CHAR);
    }

    if (( * str == '+') || ( * str == '-')) {
      if (!((counterChar(str, '+') + counterChar(str, '-')) == 1)) {
        printError(rowNum, DATA_PLUS_MINUS);
        return NULL;
      }
    } else {
      if (!((counterChar(str, '+') + counterChar(str, '-')) == 0)) {
        printError(rowNum, DATA_PLUS_MINUS);
        return NULL;
      }
    }
    num = atoi(str);
    /*must to be number */
    if (index == 0) {
      if (!((maxDb >= num) && (minDb <= num))) {
        printError(rowNum, DATA_RANGE);
        return NULL;
      }
    }
    if (index == 1) {
      if (!((maxDh >= num) && (minDh <= num))) {
        printError(rowNum, DATA_RANGE);
        return NULL;
      }
    }
  }
  ptr = makeDataImage();
  if (index == 3)
  {
    ptr -> c = * str;
  }
  else
    ptr -> num = num;
  ptr -> dc = dc;
  return ptr;

}

int returnOpcode(char type, int index) {
  if (type == 'R')
    return opcodeR[index];
  if (type == 'I')
    return opcodeI[index];
  if (type == 'J')
    return opcodeJ[index];
  return 0;
}

int returnReg(char * reg) {
  reg++;
  return atoi(reg);
}

char * skipSpace(char str[]) {
  char * ptr = str;
  while ( * ptr == ' ') {
    ptr++;
  }
  return ptr;
}

char * skipSpaceRight(char str[]) {
  char * ptr = str;
  while ( * ptr != '\0') {
    ptr++;
  }
  ptr--;
  while ( * ptr == ' ') {
    ptr--;
  }
  ptr++;
  * ptr = '\0';
  return str;
}

char * skipFirstWord(char str[]) {
  char * ptr = str;
  while ( * ptr != ' ') {
    ptr++;
  }
  return ptr;
}

char * skipParameter(char str[]) {
  char * ptr = str;
  while ( * ptr != ',') {
    ptr++;
  }
  ptr++;
  return ptr;
}

void findFirstWord(char str[]) {
  strtok(str, " ");
}

void findParameter(char str[]) {
  strtok(str, ",");
}

int counterChar(char row[], char ch) {
  char * ptr = row;
  int counter = 0;
  while ( * ptr != '\0') {
    if ( * ptr == ch)
      counter++;
    ptr++;
  }
  return counter;
}

bool checkNumber(char * str) {
  char number[MAX_LINE];
  char * ptr ;
  strcpy(number, str);
  ptr = number;
  while ( * ptr != '\0') {
    if (!(( * ptr == '+') || ( * ptr == '-') || (( * ptr >= '0') && ( * ptr <= '9'))))
    {

      return false;
    }

    ptr++;
  }
  return true;

}

bool checkComma(char row[], char type, int index) {
  int counter = counterChar(row, ',');
  if (type == 'R') {
    if (numParamR[index] - 1 == counter)
      return true;
  }
  if (type == 'I') {
    if (numParamI == counter)
      return true;
  }
  if (type == 'J') {
    counter++;
    if (numParamJ == counter)
      return true;
  }
  if (type == 'D')
    return true;
  return false;
}

int findIndex(char firstWord[], char type) {
  if (type == 'R') {
    int i;
    for (i = 0; i < rAmount; i++) {
      if (strcmp(commandsR[i], firstWord) == 0)
        return i;
    }
  }
  if (type == 'I') {
    int i;
    for (i = 0; i < iAmount; i++) {
      if (strcmp(commandsI[i], firstWord) == 0)
        return i;
    }
  }
  if (type == 'J') {
    int i;
    for (i = 0; i < jAmount; i++) {
      if (strcmp(commandsJ[i], firstWord) == 0)
        return i;
    }
  }
  if (type == 'D') {
    int i;
    for (i = 0; i < dAmount; i++) {
      if (strcmp(guidance[i], firstWord) == 0)
        return i;
    }
  }
  return 0;
}

char findType(char firstWord[]) {
  int i = 0;
  for (i = 0; i < dAmount; i++) {
    if (strcmp(guidance[i], firstWord) == 0)
      return 'D';
  }

  for (i = 0; i < rAmount; i++) {
    if (strcmp(commandsR[i], firstWord) == 0)
      return 'R';
  }
  for (i = 0; i < iAmount; i++) {
    if (strcmp(commandsI[i], firstWord) == 0)
      return 'I';
  }
  for (i = 0; i < jAmount; i++) {
    if (strcmp(commandsJ[i], firstWord) == 0)
      return 'J';
  }
  if ((strcmp(firstWord, entry)) == 0)
    return '1';
  if ((strcmp(firstWord, exter)) == 0)
    return '2';
  return '0';
}

int findItype(int index) {
  return iType[index];
}

bool checkLabel(int rowNum, char row[]) {
  char * str;
  strtok(row, ":");
  if (strlen(row) > MAXLABEL) {
    printError(rowNum, MAX_LABEL);
    return false;
  }
  str = row;
  if (counterChar(row, ' ') > 0) {
    printError(rowNum, SPACE);
    return false;
  }
  if (!(( * str >= 'a' && * str <= 'z') || ( * str >= 'A' && * str <= 'Z'))) {
    printError(rowNum, LETTER);
    return false;
  }
  str++;
  while ( * str != '\0') {
    if (!(( * str >= 'a' && * str <= 'z') || ( * str >= 'A' && * str <= 'Z') || ( * str >= '0' && * str <= '9'))) {
      printError(rowNum, LET_AND_NUM);
      return false;
    }
    str++;
  }
  if (findType(row) != '0') {
    printError(rowNum, SAVE_WORD);
    return false;
  }
  str++;
  return true;
}



int checkRegister(int rowNum, char argument[]) {
  char * ptr = argument;
  int registerNumber;
  if ( * ptr != '$') {
    printError(rowNum, REG_DOLLAR);
    return -1;
  }
  if (strlen(argument) == 1) {
    printError(rowNum, MISS_NUM);
    return -1;
  }
  ptr++;
  while ( * ptr != '\0') {
    if (!( * ptr >= '0' && * ptr <= '9')) {
      printError(rowNum, REG_NUM);
      return -1;
    }
    ptr++;
  }
  ptr = argument;
  ptr++;
  registerNumber = atoi(ptr);
  if ((registerNumber >= minRegister) && (registerNumber <= maxRegister))
    return registerNumber;
  else {
    printError(rowNum, RAN_REG);
    return -1;
  }
}

nodeRow * decodeRowPass1(char str[], nodeLabel * headLabel) {
  char label[MAX_LINE];
  char row[MAX_LINE];
  char firstWord[MAX_LINE];
  char parameters[MAX_LINE];
  char rs[MAX_LINE];
  char rt[MAX_LINE];
  char rd[MAX_LINE];
  char immedI[MAX_LINE];
  char addressJ[MAX_LINE];
  char addressI[MAX_LINE];
  int numParameters;
  char * ptr;
  char type;
  int index;
    nodeRow * pNodeRow;
  dataImage * pImage;
  nodeLabel * pLNode;
  bool isLabel = false;
  numRowPass1++;
  pNodeRow = makeNodeRow();
  strcpy(row, skipSpace(str));
  if (counterChar(row, ':') > 1)
    printError(numRowPass1, COLON);
  if (counterChar(row, ':') == 1) {
    strcpy(label, row);
    if (checkLabel(numRowPass1, label) == false) {
      printError(numRowPass1, ERROR_LABEL);
      return NULL;
    } else {
      isLabel = true;
      pLNode = makeNodeLabel();
      strcpy(pLNode -> mylabel.name, label);
      pLNode -> mylabel.ic = IC;
      if (headLabel -> next == NULL)
        headLabel -> next = pLNode;
      else {
        pLNode -> next = headLabel -> next;
        headLabel -> next = pLNode;
      }
      /*removing the label*/
      ptr = & row[0];
      while ( * ptr != ':') {
        ptr++;
      }
      ptr++;
    }
    strcpy(row, skipSpace(ptr));
  }

  strcpy(firstWord, row);
  findFirstWord(firstWord);
  if ( * skipSpace(row) == ';') {
    return NULL;
  }
  type = findType(firstWord);
  if (type == '0') {
    printError(numRowPass1, UNIDENTIFIED);
    return NULL;
  }
  if (type == '1') {
    pNodeRow -> myrow.type = type;
    strcpy(parameters, skipFirstWord(row));
    strcpy(parameters, skipSpace(parameters));
    strcpy(pNodeRow -> myrow.nameLabel, skipSpaceRight(parameters));
    return pNodeRow;
  }
  if (type == '2') {
    pNodeRow -> myrow.type = type;
    strcpy(parameters, skipFirstWord(row));
    strcpy(parameters, skipSpace(parameters));
    strcpy(pNodeRow -> myrow.nameLabel, skipSpaceRight(parameters));
    return pNodeRow;
  }
  index = findIndex(firstWord, type);
  if (checkComma(row, type, index) == false){
    printError(numRowPass1, COMMA);
    return NULL;
  }
  if (isLabel == true)
    pNodeRow -> myrow.isLabel = true;
  if (type == 'R')
    numParameters = numParamR[index];
  if (type == 'I')
    numParameters = numParamI;
  if (type == 'J')
    numParameters = numParamJ;
  pNodeRow -> myrow.type = type;
  pNodeRow -> myrow.opcode = returnOpcode(type, index);
  strcpy(parameters, skipFirstWord(row));
  strcpy(parameters, skipSpace(parameters));
  if (type == 'D') {
    pImage = checkImageData(parameters, index);
    if(pImage == NULL)
      return NULL;
  }
  if (type == 'R') {
    pNodeRow -> myrow.funct = functR[index];
    if (numParameters >= 1) {
      strcpy(rs, parameters);
      findParameter(rs);
      strcpy(rs, skipSpaceRight(rs));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
    }
    if (numParameters >= 2) {
      strcpy(rt, row);
      findParameter(rt);
      strcpy(rt, skipSpaceRight(rt));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
    }

    if (numParameters == 3) {
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      strcpy(rd, row);
      strcpy(rd, skipSpaceRight(rd));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
    } else {
      strcpy(rd, rs);
      strcpy(rs, rt);
      strcpy(rt, "$0");
    }
    pNodeRow -> myrow.rs = returnReg(rs);
    pNodeRow -> myrow.rt = returnReg(rt);
    pNodeRow -> myrow.rd = returnReg(rd);
    pNodeRow -> myrow.ic = IC;
  }
  if (type == 'I') {
    int iType = findItype(index);
    if (iType == 1 || iType == 3) {
      strcpy(rs, parameters);
      findParameter(rs);
      strcpy(rs, skipSpaceRight(rs));
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      strcpy(immedI, row);
      findParameter(immedI);
      strcpy(rt, skipSpaceRight(immedI));
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      strcpy(rt, row);
      strcpy(rt, skipSpaceRight(rt));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      pNodeRow -> myrow.immed = atoi(immedI);
      pNodeRow -> myrow.branch = false;
    }
    if (iType == 2) {
      strcpy(rs, parameters);
      findParameter(rs);
      strcpy(rs, skipSpaceRight(rs));
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      strcpy(rt, row);
      findParameter(rt);
      strcpy(rt, skipSpaceRight(rt));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      pNodeRow -> myrow.immed = -1;
      strcpy(addressI, row);
      strcpy(addressI, skipSpaceRight(addressI));
      strcpy(pNodeRow -> myrow.nameLabel, addressI);
      pNodeRow -> myrow.branch = true;
    }
    pNodeRow -> myrow.rt = returnReg(rt);
    pNodeRow -> myrow.rs = returnReg(rs);
    pNodeRow -> myrow.ic = IC;
  }
  if (type == 'J') {
    if (index == 3) {
      pNodeRow -> myrow.addj = 0;
      pNodeRow -> myrow.reg = 0;
      pNodeRow -> myrow.ic = IC;
      IC = IC + 4;
      return pNodeRow;
    }
    if ((parameters[0] == '$') && (index == 0)) {
      pNodeRow -> myrow.reg = 1;
      strcpy(addressJ, parameters);
      findParameter(addressJ);
      strcpy(addressJ, skipSpaceRight(addressJ));
      if(checkRegister(numRowPass1, rs)== -1)
        return NULL;
      pNodeRow -> myrow.addj = returnReg(addressJ);
    } else {
      strcpy(addressJ, parameters);
      findParameter(addressJ);
      strcpy(addressJ, skipSpaceRight(addressJ));
      strcpy(row, skipParameter(row));
      strcpy(row, skipSpace(row));
      pNodeRow -> myrow.reg = 0;
      strcpy(pNodeRow -> myrow.nameLabel, addressJ);
    }
    pNodeRow -> myrow.ic = IC;
  }
  if ((type == 'D') && (pImage != NULL)) {
    int count;
    pImage -> dc = DC;
    count = countImage(pImage);
    pNodeRow -> myrow.ic = IC;
    pNodeRow -> myrow.image = pImage;
    
     count= count;
      if (index == 1)
       count= count * 2;
      if (index == 2) 
         count = count * 4;
      if (index == 3)
       count++;
      
    pNodeRow->myrow.numArgumentDI = count;
    IC = IC + count;
    DC = DC + count;
    if (DC> MAX_DC)
     printError(numRowPass1,MEMORY_SLID);
    pNodeRow -> myrow.index = index;
    return pNodeRow;
  }else
  pNodeRow -> myrow.ic = IC;
  IC = IC + 4;

  pNodeRow -> myrow.index = index;
  return pNodeRow;
}

void decodeRowPass2(nodeRow * nodeRow, nodeLabel * headLabel) {
  nodeLabel * pNodeLabel;
  pNodeLabel = headLabel;
    numRowPass2++;

  if ((nodeRow -> myrow.type == 'I') && (nodeRow -> myrow.branch == true)) {
    while (pNodeLabel != NULL) {
      if (strcmp(nodeRow -> myrow.nameLabel, pNodeLabel -> mylabel.name) == 0) {
        nodeRow -> myrow.immed = pNodeLabel -> mylabel.ic - nodeRow -> myrow.ic;
        return;
      }
      pNodeLabel = pNodeLabel -> next;
    }
    printError(numRowPass2, NO_LEBAL);
  }
  if ((nodeRow -> myrow.type == 'J')&&(nodeRow->myrow.reg == 0)) {
    while (pNodeLabel != NULL) {
      if (strcmp(nodeRow -> myrow.nameLabel, pNodeLabel -> mylabel.name) == 0) {
        nodeRow -> myrow.addj = pNodeLabel -> mylabel.ic;
        return;
      }
      pNodeLabel = pNodeLabel -> next;
    }
    nodeRow -> myrow.addj = 0;
  }
  if (nodeRow -> myrow.type == '1') {
    while (pNodeLabel != NULL) {
      if (strcmp(nodeRow -> myrow.nameLabel, pNodeLabel -> mylabel.name) == 0)
        return;
      pNodeLabel = pNodeLabel -> next;
    }
    printf("%s",nodeRow -> myrow.nameLabel);
    printError(numRowPass2, LABEL_DONT_EXIST);
  }
  if (nodeRow -> myrow.type == '2') {
    while (pNodeLabel != NULL) {
      if (strcmp(nodeRow -> myrow.nameLabel, pNodeLabel -> mylabel.name) == 0)
        printError(numRowPass2,LABEL_EXIST );
      pNodeLabel = pNodeLabel -> next;
    }
  }
}

void printBit(int * bits) {
  while (bits != NULL) {
    printf(" %d ", * bits);
    bits++;
  }
}

void initIC(){
  IC = STARTINGIC;
}

void freeData(nodeRow *headrow){
  nodeRow * ptr = headrow;
  nodeRow *tempRow;
  dataImage *pData;
  dataImage *tempData;
  
  while(ptr != NULL){
    if (ptr->myrow.type == 'D'){
      pData = ptr->myrow.image;
        while(pData !=NULL){
          tempData = pData->next;
          free(pData);
          pData =tempData;
        }
    }
    tempRow = ptr->next;
    free(ptr);
    ptr = tempRow;
  }
}

