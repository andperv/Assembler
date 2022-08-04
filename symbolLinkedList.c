
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*#include "symbolLinkedList.h"*/
#include "assemblerData.h"



int addNewSybmol(symPtr *head, char *symbolName, symbolType Type, int address){
 symPtr iterator,current;
 symPtr temp;

 temp = (symPtr) malloc(sizeof(symbolNode));
 if(!temp)
   {
	printf("\n Memory symbol node allocation failed");
	return 1;
    }
 iterator = *head;
 strcpy((*temp).name, symbolName);
 (*temp).type = Type;
 (*temp).head = NULL; 
 (*temp).next = NULL;
 (*temp).address=address;
 
/* empty list */
 if(!iterator)
	*head = temp;
  else {
	while(iterator)
        {
		if(!strcmp((*iterator).name, (*temp).name))
                   {
			printf("Error in line %d: Label %s exist in symbols table\n",lineNumber,(*temp).name);
			free(temp);
			return 1;
		   }
		current = iterator;
		iterator =(*iterator).next;
	}
	(*current).next = temp;
 }
	return 0;
}



symPtr findSybmol(symPtr head1, char *symbolName,int symbolType){

     symPtr head=head1;
    if(symbolType==-1)
      {
	  while(head)
             {  
		  if(!strcmp((*head).name, symbolName))
			  return head;
		  head = (*head).next;
	     }
       }

     if(symbolName==NULL)
      {
	  while(head)
             {
		 
                if((*head).type==symbolType)
		   return head ;  
		head = (*head).next;
	     }
       }

	return 0;
}

/* In the end of first scan update data addresses */
void updateDataSybmols(symPtr head, int IC){
	while(head){
		if((*head).type == DATA)
			(*head).address += IC;
		
		head =(*head).next;
	}
}


/* on the second scan function change  the sybmol type to entry */
int changeSybmolType(symPtr head, char *symbolName){
    symPtr temp=findSybmol(head,symbolName,-1);
       if(temp)
         {
           (*temp).type=ENTRY;
            return 1; 
         }
	return 0;
}


int addAdresNode(adrPtr *head, int memAdres){
	 adrPtr iterator,current;
	 adrPtr temp;

	 temp = (adrPtr) malloc(sizeof(addressNode));
	 if(!temp){
		printf("\n Memory addressNode allocation error");
		return 1;
	 }
	 (*temp).adres = memAdres;
	 (*temp).next = NULL;

	 iterator= *head;
	 if(!iterator)
		*head= temp;
	 else 
          {
		while (iterator)
                    {
			current= iterator;
			iterator= (*iterator).next;
		     }
		(*current).next = temp;
	 }
	return 0;
}


void freeAdresNodeList(adrPtr head){
        adrPtr next;
	while(head!=NULL)
             {

		next = (*head).next;
		free(head);
		head = next;
	     }
}


void freeSybmolList(symPtr head){
        symPtr next;
	while(head!=NULL)
             {  
		next = (*head).next;
                if((*head).head!=NULL)
                    freeAdresNodeList((*head).head);
		free(head);
		head = next;
	     }
}
