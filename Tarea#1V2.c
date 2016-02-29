


#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/reg.h>
#include <errno.h>
#include <sys/syscall.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include "syscalls.h"
#include "syscallents.h"

struct counter{
	
	int syscall;
	int count;
	
};

#ifdef __amd64__
#define eax rax
#define orig_eax orig_rax
#else
#endif


#define offsetof(a, b) __builtin_offsetof(a,b)
#define get_reg(child, name) __get_reg(child, offsetof(struct user, regs.name))

long __get_reg(pid_t child, int off) {
    long val = ptrace(PTRACE_PEEKUSER, child, off);
    return val;
}


long get_syscall_arg(pid_t child, int which) {
    switch (which) {
#ifdef __amd64__
    case 0: return get_reg(child, rdi);
    case 1: return get_reg(child, rsi);
    case 2: return get_reg(child, rdx);
    case 3: return get_reg(child, r10);
    case 4: return get_reg(child, r8);
    case 5: return get_reg(child, r9);
#else
    case 0: return get_reg(child, ebx);
    case 1: return get_reg(child, ecx);
    case 2: return get_reg(child, edx);
    case 3: return get_reg(child, esi);
    case 4: return get_reg(child, edi);
    case 5: return get_reg(child, ebp);
#endif
    default: return -1L;
    }
}

char *read_string(pid_t child, unsigned long addr) {
    char *val = malloc(4096);
    int allocated = 4096;
    int read = 0;
    unsigned long tmp;
    while (1) {
        if (read + sizeof tmp > allocated) {
            allocated *= 2;
            val = realloc(val, allocated);
        }
        tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
        if(errno != 0) {
            val[read] = 0;
            break;
        }
        memcpy(val + read, &tmp, sizeof tmp);
        if (memchr(&tmp, 0, sizeof tmp) != NULL)
            break;
        read += sizeof tmp;
    }
    return val;
}

void print_syscall_args(pid_t child, int num) {
    struct syscall_entry *ent = NULL;
    int nargs = SYSCALL_MAXARGS;
    int i;
    char *strval;

    if (num <= MAX_SYSCALL_NUM && syscalls[num].name) {
        ent = &syscalls[num];
        nargs = ent->nargs;
    }
    for (i = 0; i < nargs; i++) {
        long arg = get_syscall_arg(child, i);
        int type = ent ? ent->args[i] : ARG_PTR;
        switch (type) {
        case ARG_INT:
            fprintf(stderr, "%ld", arg);
            break;
        case ARG_STR:
            strval = read_string(child, arg);
            fprintf(stderr, "\"%s\"", strval);
            free(strval);
            break;
        default:
            fprintf(stderr, "0x%lx", arg);
            break;
        }
        if (i != nargs - 1)
            fprintf(stderr, ", ");
    }
}

//usar la opcion -sortby de strace

const char* callname(long call);

char *messageTemplate = "Invalid option: %s, Available options: -V -S -s\n";

enum OPTIONS{ S, V, v, NUMBER_OF_OPTIONS};

static const char* options[] = { "-S", "-V", "-v"};

static struct termios oldt, newt;

struct counter countBySystemCall[500];

void restartBehaviour(){
	tcsetattr(0, TCSANOW, &oldt);
}

void changeBehaviour(){

    tcgetattr(0, &oldt);
    /*now the settings will be copied*/
    newt = oldt;
    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON | ECHO);          
    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr(0, TCSANOW, &newt);

}

char * concatCharToString(char *a, char c){

	int sizeInput = strlen(a);
		
	char *result = (char*) malloc(sizeInput + 2);

	strcpy(result, a);
	result[sizeInput++] = c;
	result[sizeInput] = '\0';
	
	return result;

}

char * concatChars(char *a, char *b){

	int sizeA = strlen(a);
	int sizeB = strlen(b);
	
	char *result = (char*) malloc(sizeA  + sizeB);

	strcpy(result, a);
	strcat(result, b);
	
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

void pauseProgramAnyKey(){

 fprintf (stderr, "Press any key to continue . . .\n" );
 //fflush ( stdout );
 getchar();
	
 
}

int traceeme(char *commandToExecute[]){
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 1);   
    dup2(fd, 2);    
    close(fd);
    return execvp(commandToExecute[0], commandToExecute);
}

const char *syscall_name(int scn) {
    struct syscall_entry *ent;
    static char buf[128];
    if (scn <= MAX_SYSCALL_NUM) {
        ent = &syscalls[scn];
        if (ent->name)
            return ent->name;
    }
    snprintf(buf, sizeof buf, "sys_%d", scn);
    return buf;
}

void printSysCall(pid_t child, int retVal, struct user_regs_struct regs){

	fprintf(stderr, "%s(", syscall_name(regs.orig_rax));

	long  ins = ptrace(PTRACE_PEEKTEXT, child, regs.rip,NULL);

	print_syscall_args(child, regs.orig_rax);

	
	if(retVal == -2){
		fprintf(stderr, ") = -1 ENOENT (No such file or directory)\n");
	}else{
		fprintf(stderr, ") = %d\n", retVal);
	}
}

int cmpfunc (const void * a, const void * b){
	struct counter cA = *(struct counter * )a;
	struct counter cB = *(struct counter *) b;
  	return - (cA.count - cB.count);
}

int tracer(pid_t child, int printSystemCalls, int stopAfterEachCall, int sort){
    
	changeBehaviour();
	
	int status;
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	int firstCall = 1;
	
	while(1){

		waitpid(child, &status, 0);

		 if(WIFEXITED(status))
		      break;
		
		if(firstCall){
			
			struct user_regs_struct regs;
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			int syscall =   regs.orig_rax;
			int retVal = regs.rax;
				
			countBySystemCall[syscall].count++;
			countBySystemCall[syscall].syscall = syscall;
			
			if(printSystemCalls){
				printSysCall(child, retVal, regs);
				if(stopAfterEachCall){
					pauseProgramAnyKey();				
				}
			}
			
			firstCall = 0;

		}else{
			firstCall = 1;
		}
	   	ptrace(PTRACE_SYSCALL, child, NULL, NULL);
	}
	if(printSystemCalls){
		fprintf(stderr, "exit_group(0) = ?\n");
	}
	
	int x;
	if(sort){
		qsort(countBySystemCall, 500, sizeof(struct counter), cmpfunc);
	}
	fprintf(stderr, "\n---------------%s---------------\n", "Accumulative Table");
	fprintf(stderr, "------------------------------------------------\n\n");
	fprintf(stderr, "%15s %15s\n", "Syscall", "calls");
	 
	for(x = 0; x < 500; x++){
		if(countBySystemCall[x].count){
			int syscall = countBySystemCall[x].syscall;
			
			fprintf(stderr, "%15s %15d\n", syscall_name(syscall), countBySystemCall[x].count);
		}
	}
	fprintf(stderr, "\n------------------------------------------------\n\n");
	restartBehaviour();
	
}


int main(int argc, char *argv[]){
	
	pid_t child;

	int x = 0;
	
	int *values = NULL;
	
	int totalOptions = 0;	

	if(argc < 2){

		fprintf(stderr, "Incorrect number of arguments: %i. I wait at least one.\nYou need to put the name of the program to track.\n", argc - 1); 
		
		exit(EXIT_FAILURE);
	}
	
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

	//continue to read the arguments, i think that is the process to trace
	 
        char *commandToExecute[ argc-x];
	
	for(c = 0; x < argc; x++, c++){

		commandToExecute[c] = argv[x];
	
	}	
	commandToExecute[c] = NULL;

	int stopAfterEachCall = 0;
	int printSystemCalls = 0;
	int sort = 0;
	
	for(x = 0; x < totalOptions; x++){
	
		switch(values[x]){
			
			case V: {
					stopAfterEachCall = 1; 
					printSystemCalls = 1;	
				}					
				break;
			
			case v: printSystemCalls = 1; break;
			
			case S:
				sort = 1; break;
				
			default:  fprintf(stderr, "Invalite option...\n"); exit(EXIT_FAILURE);	
		}
		
	}	
	child = fork();

	if(child == 0) {

		return traceeme(commandToExecute);

	  } else {
		
	    return tracer(child, printSystemCalls,  stopAfterEachCall, sort);

	}	
	return 0;

}
