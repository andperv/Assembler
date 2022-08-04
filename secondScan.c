#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolLinkedList.h"
#include "assemblerData.h"
#include "assemblerFunct.h"


int secondScan(FILE *fp, symPtr head){
	char *token, *oper1, *oper2;
	char line[MAX_LINE_LENGTH+1];
	int typeFlag = 0;
	int errorFlag = 0;
	IC = 0;
	lineNumber = 0;

	while(fgets(line,MAX_LINE_LENGTH,fp)){
		lineNumber++;
         
        /* comment */
		if(line[0] == ';')
			continue;
		
		moveWhiteSpaces(line);
		token = strtok(line," ");
        /* empty line */
		if(!token)
			continue;
        /* Line begin with symbol */
		if(isSybmol(token))
			token = strtok(NULL," ");
		typeFlag =isDirective(token);
                if(typeFlag == 0 || typeFlag == 2 || typeFlag == 3)
                     continue;
		else if(typeFlag == 1)
                     {
			   token = strtok(NULL," ");
			   if(!changeSybmolType(head, token))
                             {
				   printf("Error in line %d: Entry label %s was not found in table or label is external\n", lineNumber, token);
                                   errorFlag = 1;
			     }
		     }
        /* Instructions */      
        else
            {
			token = strtok(NULL," ");
                        /* No operands */
			if(!token){
				IC++;
				continue;
			}
                        oper1 = strtok(token," ,");
			oper2 = strtok(NULL," )"); 
            /* Two operands */
			if(oper2 && oper1)
                            {   
				if(addOperand(head, oper1, oper2))
                                  {
			             printf("Error while processing line %d: Instruction source and destination operands encoding failure\n", lineNumber);
					errorFlag = 1;
				  }
                            }
            /* One operand */ 
            else
                {
				  if(addOperand(head,NULL, oper1))
                    {
					  printf("Error while processing line %d: Instruction destination operand encoding failure\n", lineNumber);
					  errorFlag = 1;
				    }
			    }
		}/* End of operation case */
		
	}/* end of while */
	return errorFlag;
}
