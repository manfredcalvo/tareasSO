


#include<stdio.h>
#include <stdlib.h>
#include <string.h>

//usar la opcion -sortby de strace

char *commandStrace = "strace -t -i %s 2>&1";
 
enum OPTIONS{v, V, NUMBER_OF_OPTIONS};

static const char* options[] = { "-V", "-v"};

int containsOption(char *arg){


	
	int n = NUMBER_OF_OPTIONS;
	
	int li = 0;
	int ls = n - 1;
	int medio;
	
	while(li <= ls){
	
		medio = (li + ls) / 2;
		
		int compare = strcmp(options[medio], arg); 
			
		if( compare == 0){
			
			return medio;		
		}else{
			if(compare < 0){
				
				li = medio + 1;
											
			}else{
				ls = medio - 1;			
			}		
		}
		
	}
			

	return -1;
	
}

int main(int argc, char *argv[]){
	
	int x = 0;
	
	int *values = NULL;
	
	int totalOptions = 0;	

	for(x = 1; x < argc; x++){		

		int option = containsOption(argv[x]);
		
		if(option != -1){
			
			values = (int*) realloc(values, (totalOptions + 1) * sizeof(int));
			values[totalOptions++] = option;
					
		}else{
		
			//throw exception unknown option
		}
	}
		
      	char * commandBuilder = "";
	char *aux;	
	int c;
	
	for(x = totalOptions + 1, c = 0; x < argc; x++, c++){

		aux = commandBuilder;
		commandBuilder = (char *) malloc(2 + strlen(commandBuilder)+ strlen(argv[x]));
		strcpy(commandBuilder, aux);
		if(c > 0){
			strcat(commandBuilder, " ");		
		}
		strcat(commandBuilder, argv[x]);  
		
	}	
	
	printf("Program to execute:%s\n", commandBuilder);

	char *finalCommand = (char*) malloc(strlen(commandBuilder) + strlen(commandStrace));
	
	sprintf(finalCommand, commandStrace, commandBuilder);
	printf("Final command:%s\n", finalCommand);
	/*FILE *fp;
		
	int status;

	char path[1000];

	fp = popen("strace -t -i ls * 2>&1", "r");
		
	if(fp == NULL){
	
	}
	
	int c;
	char valueToFound = '=';
	
	while ((c = fgetc(fp)) != EOF) {
		
		char v = (char) c;
		printf("%c", v);
		
		if(v == valueToFound){
			
			if(valueToFound == '='){
				valueToFound = '\n';
			}else{
				valueToFound = '=';
				printf("Nuevo system call\n");
			}
					
		}
		
    	}

	status = pclose(fp);*/

	return 0;

}


