#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolLinkedList.h"
#include "assemblerData.h"
#include "assemblerFunct.h"
#include "firstScan.h"
#include "secondScan.h"



int main(int argc, char *argv[]){
	int i;
	FILE *fp;
	char fileName[MAX_FILE_NAME];
	symPtr head;


	for (i=1 ; i < argc; i++) {
		strcpy(fileName, argv[i]);
		fp=fopen(fileName, "r");
		if(!fp)
                   {
			printf("File %s not found\n",fileName);
			continue;
		   }

	DC = 0;
        IC = 0;
        head = NULL;
		printf("Starting a scan file %s .\n", fileName);

		if(firstScan(fp, &head))
                   {
			printf("First Scan failed\n");
			fclose(fp);
			freeSybmolList(head);
			continue;
		   }
                printf("First Scan pass successfully \n");
		fclose(fp);
		fp=fopen(fileName, "r");
		if(secondScan(fp,head))
                  {
			printf("Second Scan failed\n");
			fclose(fp);
			freeSybmolList(head);
			continue;
		   }
		fclose(fp);
                printf("Second Scan pass successfully \n");
                if(newObjFile(fileName)||newEntFile(fileName, head)||newExtFile(fileName, head))
                   {
                    freeSybmolList(head);
		    continue;
		    } 
		
		printf("Successfully create source files for %s\n", argv[i]);
		freeSybmolList(head); 
	}
	return 0;
}


