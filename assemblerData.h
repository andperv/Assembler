#ifndef ASSEMBLERDATA
#define ASSEMBLERDATA
#define MAX_LINE_LENGTH 80
#define OFFSET 100
#define MAX_FILE_NAME 40														
#define MAX_LABEL_SIZE 31 /*max size of symbol name*/
/*#include "symbolLinkedList.h"*/
extern const char *directive[];
extern const char *instruction[];
extern const char *regist[];
extern int lineNumber;
extern int data[]; /*data memory */
extern int code[]; /* code memory */
extern int IC, DC;

typedef enum addressType {IMMEDIATE,DIRECT,JUMP,REG,NOTSET} addressType; /*enum for operand addressing types*/
typedef enum symbolType {DATA,CODE,EXTERNAL,ENTRY} symbolType;		/*enum for symbol types*/

typedef struct addressNode * adrPtr; 

 struct addressNode{		   /*addressdNode of linked list used to store  addresses for external sybmols*/
 int adres;
 adrPtr next;
}typedef addressNode;



typedef struct symbolNode * symPtr;

struct symbolNode{		 
 char name[MAX_LABEL_SIZE+1];
 symbolType type;
 int address;
 symPtr next;
 adrPtr head;
}typedef  symbolNode;



#endif
