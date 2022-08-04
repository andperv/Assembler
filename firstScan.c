#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolLinkedList.h"
#include "assemblerData.h"
#include "assemblerFunct.h"



int firstScan(FILE *fp, symPtr *head){
	char line[MAX_LINE_LENGTH+1];
	char *token, *token2 ;
	char tempSybmol[MAX_LABEL_SIZE+1];
	int sybmolSize;
	int errorFlag = 0;
        int sybmolFlag;
	int typeFlag = 0;
	lineNumber = 0;

	while(fgets(line,MAX_LINE_LENGTH,fp)){
		lineNumber++;
                sybmolFlag = 0;
		if(line[0] == ';')/*comment */
		   continue;
		moveWhiteSpaces(line);
		token = strtok(line," ");
        
		if(!token)/*empty line */
		   continue;
		  
  
              /*Line begin with label */
		sybmolFlag = isSybmol(token);
		if(sybmolFlag)
                   {
			sybmolSize = sybmolShyntaxTest(token);
			if(!sybmolSize)
                          {
				errorFlag = 1;
				continue;
			   }
			strncpy(tempSybmol, token,sybmolSize);
			tempSybmol[sybmolSize] = '\0';
			token = strtok(NULL," ");
                        if(token==NULL)
                           {
                             printf("Error in line %d:Illegal declaration of sybmol(alone in the line)\n", lineNumber);
                             errorFlag = 1;
			     continue;
			   }
		  }
          
/*------------------------------------------------------------------------------------------------------------------------------*/
		typeFlag = isDirective(token);
		if(typeFlag >= 0){
			token = strtok(NULL," ");
			if(!token)
                          {
				printf("Error in line %d: Missing parameters\n", lineNumber);
				errorFlag = 1;
				continue;
			  }
            
                 if(typeFlag==1) /*entry instruction declaration ignored on a first scan */    
			continue;
                
                     
		 else if(typeFlag==0)/*external instruction */
                            {
                               
                                if(addNewSybmol(head, token, EXTERNAL, 0))
                                  {
                                     printf("Error in line %d: Failed to add label %s to label list\n", lineNumber, token);
		                     errorFlag = 1;
			             continue;
	                           }
                     
                              }    
                            /*data or string */
				if(sybmolFlag)
                                    { 
					if(addNewSybmol(head, tempSybmol, DATA, DC))
                                         {
						printf("Error in line %d: Failed to add label %s to label list\n", lineNumber, tempSybmol);
						errorFlag = 1;
						continue;
					  }
				   }
             
				if(typeFlag==2) /*data    */
				   {		
                                       token2 = strtok(NULL," ");
						if(token2)
                                                  {
							errorFlag = 1;
							printf("Error in line %d: Redundant parameters\n", lineNumber);
						  }
                                                 else
                                                    errorFlag = addData(token); 
				  }		
                                 
				 else if(typeFlag==3)/*string */
                                        {
					   if(addString(token))
                                               errorFlag = 1;
                       
			                 }
		}/*end of guidance */
 /*-----------------------------------------------------------------------------------------------------------------*/       
         /* Instruction */
        else{
            
			if(sybmolFlag)/* Appear symbol before instruction */
                          {
				if(addNewSybmol(head, tempSybmol, CODE, (IC)+OFFSET))
                                    {
					printf("Error in line %d: Failed to add Sybmol %s to Sybmol List\n", lineNumber,tempSybmol);
                                        errorFlag=1;

				     }
			   }
               
			typeFlag = isInstruction(token);
			if(typeFlag < 0)
                          {
				 printf("Error in line %d: '%s' is synthax error\n", lineNumber, token);
                                 errorFlag=1;
			   }

            else
                {
				token = strtok(NULL," ");
				if(strtok(NULL," "))
                                    {
					printf("Error in line %d: Redundant parameters\n", lineNumber);
					errorFlag = 1;
					continue;
				    }
				if(addInstruction(typeFlag, token))
                                        errorFlag=1;
			
			}
		}/* end of instruction */
		
	}
	updateDataSybmols(*head, (IC)+OFFSET);
	return errorFlag;
}
