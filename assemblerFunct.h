#ifndef ASSEMBLERFUNCT
#define ASSEMBLERFUNCT

void moveWhiteSpaces(char *);
				
void dotSlashEncoder(int, int, char *);	
		
int isSybmol(char *);
							
int isDirective(char *);
							
int isInstruction(char *);
						
int isRegister(char *);	
						
int sybmolShyntaxTest(char *);	
				
int addData(char *);		/*store data to data[] receive pointer to string representation of data value*/
int addString(char *);	     /*store string to data[]. receive pointer to string value*/
							
int testComma(char *);			/*test legal comma position in string, expects pointer to string*/

int addInstruction(int, char *);
				
int encodeInstruction(int, int, int);	
			
int getOperandType(char *);	
						
int encodeOperand(symPtr , char *, int , int );

int addOperand(symPtr , char *, char *); 

int newObjFile(char *);
								
int newEntFile(char *, symPtr);

int newExtFile(char *, symPtr);	
					
int isOperandLegal(int instructType,int sourceOperand,int destOperand); /*check if source and destination operands legal to type of instruction */

#endif
