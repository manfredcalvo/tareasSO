

#include<stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	FILE *fp;
		
	int status;

	char path[1000];

	fp = popen("strace -C -o myOuput.txt ls *", "r");
		
	if(fp == NULL){
	
	}
	
	char * line = NULL;
	size_t len = 0;
    	ssize_t read;
	int in;
	
	while((read = getline(&line, &len, fp)) != -1){
		
		printf("%s\n", line);
	
	}

	status = pclose(fp);

	return 0;

}


