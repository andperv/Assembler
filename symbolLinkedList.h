#ifndef LINKEDLIST
#define LINKEDLIST
#include "assemblerData.h"

int addNewSybmol(symPtr *, char *, symbolType, int);
									
int addAdresNode(adrPtr *, int);
		
void updateDataSybmols(symPtr, int);	

void freeSybmolList(symPtr);

int changeSybmolType(symPtr, char *);
			
symPtr findSybmol(symPtr, char *,int);

int addNewSybmol(symPtr *, char *, symbolType, int);

void freeAdresNodeList(adrPtr);
#endif
