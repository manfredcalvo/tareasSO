


#include<stdio.h>
#include <stdlib.h>
#include <string.h>

//usar la opcion -sortby de strace

char *commandStrace = "strace -t -i -qq -q %s %s 2>&1"; 

char *messageTemplate = "Invalid option: %s, Available options: -V -S -s\n";

enum OPTIONS{ S, V, v, NUMBER_OF_OPTIONS};

static const char* options[] = { "-S", "-V", "-v"};

char * concatCharToString(char *a, char c){

	int sizeInput = strlen(a);
		
	char *result = (char*) malloc(sizeInput + 2);

	strcpy(result, a);
	result[sizeInput++] = c;
	result[sizeInput] = '\0';
	
	return result;

}

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

void pauseProgram(FILE *in){

 printf ( "Press [Enter] to continue . . ." );
 fflush ( stdout );

 int ch;

 do
    ch = fgetc ( in ); 
 while ( ch != EOF && ch != '\n' ); 

 clearerr ( in );

 printf("\n");

//while ( getchar() != '\n' );

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
				
			if(argv[x][0] == '-'){
				//i read an invalid option
				char *finalMessage = (char *) malloc(strlen(messageTemplate) + strlen(argv[x]));
				sprintf(finalMessage, messageTemplate, argv[x]);
 				fprintf(stderr, "%s", finalMessage); exit(EXIT_FAILURE);				
			}else{
				//i read the last option to rastreador
				
				break;			
			}
		}
	}
		
      	char * commandBuilder = "";
	char *aux;	
	int c;

	//continue to read the arguments, i think that is the process to rastrear
	 
	for(c = 0; x < argc; x++, c++){

		aux = commandBuilder;
		commandBuilder = (char *) malloc(2 + strlen(commandBuilder)+ strlen(argv[x]));
		strcpy(commandBuilder, aux);
		if(c > 0){
			strcat(commandBuilder, " ");		
		}
		strcat(commandBuilder, argv[x]);  
		
	}	
	
	char outputOption[3] = {'-', 'c', '\0'};

	int stopAfterEachCall = 0;
	
	for(x = 0; x < totalOptions; x++){
	
		switch(values[x]){
			
			case V: {
					stopAfterEachCall = 1; 
					strcpy(outputOption, "-C");
				}					
				break;
			
			case v: strcpy(outputOption, "-C"); break;
			
			default:  fprintf(stderr, "Invalite option...\n"); exit(EXIT_FAILURE);	
		}
		
	}	

	//printf("Program to execute:%s\n", commandBuilder);

	char *finalCommand = (char*) malloc(strlen(commandBuilder) + strlen(commandStrace));
	
	sprintf(finalCommand, commandStrace, outputOption, commandBuilder);
	
	//printf("Final command:%s\n", finalCommand);
	
	FILE *fp;
		
	int status;

	char path[1000];

	fp = popen(finalCommand, "r");
		
	if(fp == NULL){
	
	}
	
	char valueToFound = '=';
	
	char *systemCall = "";
	
	char *title = "Time     Instruction Pointer System Call";

	while ((c = fgetc(fp)) != EOF) {
		
		char v = (char) c;
		
		systemCall = concatCharToString(systemCall, v);
		
		if(v == valueToFound){
			
			if(valueToFound == '='){
				valueToFound = '\n';
			}else{
				valueToFound = '=';
				printf("%s\n", title);
				printf("%s", systemCall);
				free(systemCall);
				systemCall = "";
				if(stopAfterEachCall){
					pauseProgram(stdin);			
				}
			
				
			}
					
		}
		
    	}
		
	printf("Cumulative Table\n\n");
	
	printf("%s\n", systemCall);

	status = pclose(fp);

	return 0;

}


