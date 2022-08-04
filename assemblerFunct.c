
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbolLinkedList.h"
#include "assemblerData.h"
/*#include "assemblerFunct.h"*/


int isSybmol(char *str){
	while(*str)
           {
		if(*str == ':')
	            return 1;
		str++;
	   }
	return 0;
}

/*----------------------------------------------------------------------- */

int isDirective(char * str ){
	int i=0;
        
	while(directive[i] != NULL)
            {
		if(!strcmp(directive[i], str))
		   return i;
		i++;
	     }
	return -1;
}

/*----------------------------------------------------------------------- */

int isInstruction(char *str){
	int i=0;
	while(instruction[i] != NULL)
           {
		if(!strcmp(instruction[i], str))
                  {
			return i;
		  }
		i++;
	   }
	return -1;
}

int isRegister(char * str ){
	int i=0;
        
	while(regist[i] != NULL)
            {
		if(!strcmp(regist[i], str))
		   return i;
		i++;
	     }
	return -1;
}

/* -------------------------------------------------------------------------------------------- */

int testComma(char *line){
    /* line start with comma */
	if(*line == ','){
		printf("Error in line %d: Comma error\n", lineNumber);
		return 0;
	}
	line++;
	while(*line){
		if(*line == ','){
           /* A line ends with a comma */
			if(*(line+1) == '\0')
              {
				printf("Error in line %d: Line ends with a comma\n", lineNumber);
				return 0;
			  }
            /* Sequence of commas */
			if(*(line+1) == ',')
               {
				printf("Error in line %d: Sequence of a commas\n", lineNumber);
				return 0;
			  }
		}
		line++;
	}
	return 1;
}

/*----------------------------------------------------------------------- */

int sybmolShyntaxTest(char *str){
	char temp[MAX_LABEL_SIZE];
	int i=0;
        if(str==NULL)
           {
		printf("Error in line %d: Symbol is empty\n", lineNumber);
		return 0;
	   }
	if(!isalpha(*str))
           {
		printf("Error in line %d: Symbol first character is not alphabetic \n", lineNumber);
		return 0;
	   }
	while(*str != ':' && *str != '\0')
        {
		  if(i>30)
                    {
			  printf("Error in line %d: Symbol is too long\n", lineNumber);
			  return 0;
		    }
		  if(!isalpha(*str) && !isdigit(*str)) 
                     {
			  printf("Error in line %d: Symbol contains a not legal character\n", lineNumber);
			  return 0;
		    }
		 temp[i] = *str;
		 str++;i++;
	   }
	if(isRegister(temp)>=0 || isInstruction(temp)>=0)
           {
		printf("Error in line %d: Symbol can't get register/instruction name\n", lineNumber);
		return 0;
	    }
	return i;
}

/* ---------------------------------------------------------------------------------------------------------- */

/*function add data to the data[] */

int addData(char *p){
	char *token;
	char *endptr = NULL;
	int tempDC = DC;
	int tempNum;
	if(!testComma(p)){
		printf("Error in line %d: Data  parameter is illegal\n", lineNumber);
        /* remove next instruction */
		DC = tempDC;
		return 1;
	}
	token = strtok(p,",");
    /* nextChar  */
	while(token)
            {
		tempNum = (int) strtol(token, &endptr, 10);
		if(*endptr!= '\0')
                  {
			printf("Error in line %d: Format of natural number is illegal %s\n", lineNumber, token);
			DC = tempDC;
			return 1;
		  }
          
		if(tempNum > 8191 || tempNum< -8192)
                   {
			printf("Error in line %d:Number  %s is bigger than a memory word\n", lineNumber, token);
			DC = tempDC;
			return 1;
		   }
		data[DC] = tempNum;
		DC++;
		token = strtok(NULL,",");
	      }
	return 0;

}

/*----------------------------------------------------------------------------------------------------- */

/*function add string to the data[] */

int addString(char *p){
    int i=0;
    char  *string;
    
    if(p[0]!='\"')
     {
        printf("Error in line %d: Illegal characters before string\n", lineNumber);
        return 1;
     }
    string = strtok(p, "\"");
    while(string[i] != '\0')
       {
		data[DC] = string[i];
		i++;DC++;
       }

        
	data[DC] = '\0';
	DC++;
	return 0;
}

/* ------------------------------------------------------------------------------------ */

int getOperandType(char *operand){
	if(operand[0] == '#')
		return IMMEDIATE;
	if(isRegister(operand) >= 0)
		return REG;
	
	return DIRECT;
}


/* ------------------------------------------------------------------------------------------------ */

/*Function check if source and destination operands legal to type of instruction she receive */

int isOperandLegal(int insType,int sourceOperand,int destOperand){
    int result=0;
    if(destOperand!=-1)
       {
          if(destOperand == IMMEDIATE && (insType != 1 && insType != 12))
            {
             printf("Error in line %d: Destination operand is illegal\n",lineNumber);
             result-=1;
            }
        
        }
      
       if(sourceOperand!=-1)
         {
          if(insType == 6 && (sourceOperand == IMMEDIATE || sourceOperand == REG))
            {
             printf("Error in line %d: Source operand  is illegal\n",lineNumber);
             result-=1;
            }
        
         }
      return result+=1;
}

/* -------------------------------------------------------------------------------- */

int encodeInstruction(int insType, int source, int dest){
	int instruction = 0;
	int offset = 0;
	instruction |= (insType << 6);
    /* jump instructions */
    if(insType ==9 || insType ==10 || insType==13) 
      { 
        if(source==-1)
          {
             instruction |= (dest << 2);
             offset += 1;
           }
        else
            {
              instruction |= (JUMP << 2);
              offset += 1;
	      instruction |= (dest << 10);
              offset += 1;
	      instruction |= (source << 12);
	      if(source != REG)
	          offset += 1;
              else if(dest != REG)
		  offset += 1;
		       
            } 
      }
      
    else 
       {
	       if(dest >= 0)
             {
               instruction |= (dest << 2);
               offset += 1;
	      if(source >= 0)
                  {
			        instruction |= (source << 4);
			        if(source != REG)
				           offset += 1;
			        else if(dest != REG)
				         offset += 1;			
		          }
              }
       }
        
	code[IC] = instruction;
	IC++;
	IC += offset;
	return 0;
} 

/* ------------------------------------------------------------------------------------------------ */

/*function add instruction to the code[] call a encodInstruction to encode instruction */

int addInstruction(int insType, char *operands){
	char *token1, *token2, *token3;
	addressType oper1 = NOTSET,oper2 = NOTSET;
    /* operation without operands  rts/stop */
	if(insType > 13)
           {
			if(operands)
				printf("Error in line %d: Redundant parameters\n",lineNumber);
			else
				return(encodeInstruction(insType, -1, -1));
			
	    }
   
    /* Operation with operands */
    else if(operands)
           {
		if(!testComma(operands))
                  return 1;
    
           /* jump instruction  jmp/bne/jsr  */
               if(insType ==9 || insType ==10 || insType==13)  
                 {   
                        token1 = strtok(operands, ",");
                        token2 = strtok(NULL, ")"); /*secon Parameter */
                        if(!operands)/* No operands */
                          {
                              printf("Error in line %d: No operands for jump function  \n",lineNumber);
                              return 1;
                          }
                        
                        /* one operand only */
                        if(!token2)  
                          {
                             oper1 = getOperandType(token1);
                             if(oper1==IMMEDIATE)/* illegal operand */
                               {
                                 printf("Error in line %d: Illegal type operand for jump function\n",lineNumber);
                                 return 1;
                               }
                             return(encodeInstruction(insType, -1, oper1));  
                           }
            
                        token3 = strtok(token1, "("); /*sybmol */
                        token1=strtok(NULL," "); /* first Parameter */
                        if(!token1)  /* example of error case: jmp L,K */
                          {
                                 printf("Error in line %d: Illegal operand declaration for jump function\n",lineNumber);
                                 return 1;
                           }
                        
                        oper1 = getOperandType(token1);
                        oper2 = getOperandType(token2);
                        return (encodeInstruction(insType, oper1, oper2));
                  }/* end of jump instruction */
        
		token1 = strtok(operands, ",");
		token2 = strtok(NULL, " ,");
		token3 = strtok(NULL, ",");
		if(token1 && !token3)  /* check the amount of operands */
                  {
			oper1 = getOperandType(token1);          
			if((insType>3 && insType != 6))/* operations with one operand */
                           {
				if(token2)
				    printf("Error in line %d: Incorrect num of operands\n",lineNumber);
				
                                else
                                   {
		                      if(isOperandLegal(insType, -1,oper1))
				         return(encodeInstruction(insType, -1, oper1));
					  
		                   }
                            }
           
                        /* operation with two operands */
                       else
                          {
		             if(token2)
                                {
                                   oper2 = getOperandType(token2);
		                   if(isOperandLegal(insType, oper1,oper2))
                                       return (encodeInstruction(insType, oper1, oper2));
						 
                                }
                             else
			         printf("Error in line %d: Destination operand is missing\n",lineNumber);
				
	                  }/* end of operation with two operands  */
 
		}/*end of check the amount of operands */
             else
	         printf("Error in line %d: Incorrect number of operands\n",lineNumber);
		
	}/* end of operation with operands */
     else
         printf("Error in line %d: Missing operands\n",lineNumber);
         return 1;
}



/* ------------------------------------------------------------------------------------- */

void moveWhiteSpaces(char *line){
	char *tempBuf = line;
	int comma = 0; 
        int space = 0;
	while(*line != EOF && *line != '\n' && *line!='\0')
            {
		  if(*line == ' ' || *line == '\t')
			space = 1;
		else if(*line == ',')
                     {
			if(comma == 0)
				comma = 1;
			else
                            {
				*tempBuf = ',';
				tempBuf++;
				space = 0;
			     }
		      }
       
                else
                    {
			if(comma == 1)
                          {
				*tempBuf = ',';
				tempBuf++;
				comma = 0; space = 0;
			   }
                       else if(space == 1)
                           {
				 *tempBuf = ' ';
				  tempBuf++;
				  space = 0;
			    }
			if(*line == '"')
                          {
				*tempBuf = *line;
				line++;tempBuf++;
				while(*line != '"')
                                    {
					   *tempBuf = *line;
                                            line++;tempBuf++;
				     }
			  }
			*tempBuf = *line;
			tempBuf++;
		     }/* end of else */
		line++;
	    }/* end of while */
	 if(comma == 1)
            {
		*tempBuf = ',';
		tempBuf++;
            }
	*(tempBuf) = '\0';
   
}

/*---------------------------------------------------------------------------------------------- */

/* Encode a operand in memory word */
int encodeOperand(symPtr symbolPtr, char *oper, int operType, int sourceFlag){
	symPtr sybmol;
	int numOperand;
	char *endptr;
	switch(operType){

		case 0: /* immediate */
			numOperand = (int) strtol(oper+1, &endptr, 10);
                        if(*endptr != '\0')
                            {     
				printf("Error in line %d: Format of natural number is illegal %s\n", lineNumber, oper);
				IC++;
				return 1;
			    }
			if(numOperand >= 2047 || numOperand < -2048)
                             {
			         printf("Error in line %d: immediate operand  %s is out of memory\n", lineNumber, oper);
			         IC++;
				 return 1;
		             }
		        code[IC] = numOperand << 2;
			IC++; 
                        break;              
              
	          case 1: /* direct */ 
                   
		       if(!sybmolShyntaxTest(oper)) /* checking syntax */
                              {
			          printf("Error in line %d: Use of illegal Sybmol %s\n", lineNumber, oper);
			          IC++;
			          return 1;
			       }
                  
			sybmol = findSybmol(symbolPtr, oper,-1);
		        if(!sybmol) 
                             {
			        printf("Error in line %d:Sybmol %s  not found\n", lineNumber, oper);
			        IC++;
			        return 1;
		             }
                    
			if((*sybmol).type == EXTERNAL)  /* external symbol  */ 
                             {
		                 code[IC] = 1;
			         if(addAdresNode(&((*sybmol).head) , IC))
                                   {
				    printf("Error in line %d: Failed store address for external sybmol\n",lineNumber); 
				    IC++;
				    return 1;
			           }
			      }
                          else
		                  code[IC] = ((*sybmol).address << 2) | 2;
		           IC++; 
                           break;
				
               
	           case 3: /* register */
		          if(sourceFlag)
			      code[IC] =  atoi(oper+1) << 8;
		          else
			      code[IC] |=  atoi(oper+1) << 2;
						
		          IC++;
			  break;
	}
	return 0;
}

/*--------------------------------------------------------------------------------------------*/

/*function add operand to the code[] call a encodOperand to encode operand */

int addOperand(symPtr labelPtr, char *source, char *destination){
        int maskDest = 12;
	int maskSource = 48;
        int adressType,adressType2;
	char *firstParam,*symbol;
	int curInstruct = code[IC];
	int errorFlag = 0;
	IC++;
       
        if(source==NULL) /* Only one operand  */
           {
		adressType=(curInstruct & maskDest) >> 2;
		return encodeOperand(labelPtr, destination, adressType,0);
		
            }
        else  /* Two operand instruction */
            {   
                adressType=(curInstruct & maskDest) >> 2;
                if(adressType==2) /* jump instruction  */
	          {
		        symbol=strtok(source,"(");
			firstParam=strtok(NULL," ");
                        errorFlag+=encodeOperand(labelPtr, symbol, 1,0);
                        adressType=(curInstruct & 12288) >> 12;  /* 12288=2^13+2^12 mask for first parameter */
		        errorFlag+=encodeOperand(labelPtr, firstParam, adressType,1);
		        adressType2=(curInstruct & 3072) >> 10;  /* 3072=2^11+2^10 mask for second parameter */
                        if(adressType2==3 && adressType2==adressType)
		           IC--;
			 errorFlag+=encodeOperand(labelPtr, destination, adressType2,0);
			 return errorFlag;
                  }
                adressType2=(curInstruct & maskSource) >> 4;
                errorFlag+=encodeOperand(labelPtr,source, adressType2,1);
                if(adressType==3 && adressType2==adressType)
	            IC--;
                errorFlag+=encodeOperand(labelPtr,destination,adressType,0);
		return errorFlag;
            }
		
}

/* ------------------------------------------------------------------------------------------- */

void dotSlashEncoder(int input, int memIndex, char *output){
     /* 8192=2^13 the last bit in memory word */
     int i,len,temp,mask=8192;
     char dot='.';
     char frontSlash='/';
     char adress[4];
     strcpy(output,"  ");
     sprintf(adress, "%d", memIndex);
     strcat(output,adress);
     strcat(output,"    ");
     len=strlen(output);
    for(i=0;i<14;i++,len++)
        { 
           temp=input&(mask>>(i));
           if (temp==0)
               output[len]=dot;
           else
               output[len]=frontSlash;
	
        }
      output[len]='\n';  
        
   } 

	

/*--------------------------------------------------------------------------------------------------*/

int newObjFile(char *fileName){
	int i;
	FILE *fp;
	char line[25];
        char adress[4];
        char objFileName[MAX_FILE_NAME];
        strcpy(objFileName, fileName);
	strcat(objFileName,".ob");
        memset(line, '\0', sizeof(line));
	fp=fopen(objFileName, "w");
	if(!fp)
          {
	      printf("Entry file %s creation failed,aborting assemble\n",objFileName);
	      return 1;
	  }
       strcat(line,"      ");
       sprintf(adress, "%d", IC);
       strcat(line,adress);
       strcat(line,"    ");
       sprintf(adress, "%d", DC);
       strcat(line,adress);
       strcat(line,"\n\0");
       fputs(line,fp);
	for(i=0; i<IC; i++)
           {
		dotSlashEncoder(code[i], OFFSET+i, line);
		fputs(line , fp);
	   }
	for(i=0; i<DC; i++)
          {
            dotSlashEncoder(data[i], OFFSET+i+IC, line);
	    fputs(line , fp);  
           } 
	fclose(fp); 
	return 0;
}

/*--------------------------------------------------------------------------------------------------*/

int newEntFile(char *fileName, symPtr symbolPtr){
	FILE *fp;
	char line[40];
	char adress[5];
	char entryFileName[MAX_FILE_NAME];
        strcpy(entryFileName, fileName);
	strcat(entryFileName,".ent");
	symbolPtr = findSybmol(symbolPtr,NULL,ENTRY);
        
	if(symbolPtr)
           {
	       fp=fopen(entryFileName, "w");
	       if(!fp)
                 {
	            printf("Entry file %s creation failed,aborting assemble\n",entryFileName);
		    return 1;
	         }
	      while(symbolPtr)
                  {
                     strcpy(line,"  ");
                     strcat(line,(*symbolPtr).name);
		     strcat(line, "    ");
                     sprintf(adress, "%d\n", (*symbolPtr).address);
                     strcat(line, adress);
	             fputs(line, fp);
		     symbolPtr = findSybmol((*symbolPtr).next,NULL,ENTRY);
	           }
	      fclose(fp);
	    }
        return 0;
}

/*--------------------------------------------------------------------------------------------------*/

int newExtFile(char *fileName, symPtr symbolPtr){
	FILE *fp;
	char line[40];
	char adress[5];
        adrPtr tempHead;
        char externFileName[MAX_FILE_NAME];
        strcpy(externFileName, fileName);
	strcat(externFileName,".ext");
	symbolPtr = findSybmol(symbolPtr,NULL,EXTERNAL);
	if(symbolPtr)
           {
		
			fp=fopen(externFileName, "w");
			if(!fp)
                           {
				printf("Extern file %s creation failed,aborting assembler\n",externFileName);
				return 1;
			    }
			 while(symbolPtr)
                              {
                                tempHead=(*symbolPtr).head;
                                while(tempHead)
                                    {
                                       strcpy(line,"  ");
				       strcat(line, (*symbolPtr).name);
				       strcat(line, "    ");
                                       sprintf(adress, "%d\n", (*tempHead).adres+OFFSET );
                                       strcat(line, adress);
			      	       fputs(line, fp);
                                       tempHead=(*tempHead).next;
                                       
			    	    }
				symbolPtr = findSybmol((*symbolPtr).next,NULL,EXTERNAL);
			      }
			fclose(fp);
		    
	    }
      
	return 0;
}




