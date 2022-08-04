#include<stdlib.h>
#define MAX_PROGRAM_SIZE 256  
int lineNumber;
int data[MAX_PROGRAM_SIZE]; /* Data memory */
int code[MAX_PROGRAM_SIZE]; /* Code memory */
int IC, DC;
const char *directive[]={".extern",".entry",".data",".string",NULL}; 
const char *instruction[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop",NULL}; 
const char *regist[]={"r0","r1","r2","r3","r4","r5","r6","r7",NULL}; 

