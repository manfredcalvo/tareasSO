


#include<stdio.h>
#include <stdlib.h>
//usar la opcion -sortby de strace

char *commandStrace = "strace -t -i ";
 
int main(int argc, char *argv[]){

	char * allComand = (char *)malloc(1000);
	
	int x = 0;
	
	for(x = 1; x < argc; x++){
		
	}	

	FILE *fp;
		
	int status;

	char path[1000];

	fp = popen("strace -t -i ls * 2>&1", "r");
		
	if(fp == NULL){
	
	}
	
	char * line = NULL;
	size_t len = 0;
    	ssize_t read;
	int in;
	
	/*while((read = getline(&line, &len, fp)) != -1){
		
		//printf("%s\n", line);
	
	}*/
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

	/*printf("PrintA:%c\n", (char)fgetc(fp));
	printf("PrintB:%c\n", (char)fgetc(fp));
printf("PrintC:%c\n", (char)fgetc(fp));
printf("PrintC:%c\n", (char)fgetc(fp));
printf("PrintC:%c\n", (char)fgetc(fp));
printf("PrintC:%c\n", (char)fgetc(fp));*/

	status = pclose(fp);

	return 0;

}


