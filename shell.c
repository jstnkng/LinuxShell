#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>


int main(){
	printf("\n-------------------------\n");
	printf("Welcome, this is a basic linux shell\n");
	printf("Authored by Nathan Wichman and Justin King\n");
	printf("Enter valid linux commands!\n");
	printf("When you're done, just type 'quit'\n");
	printf("--------------------------\n");

	while(1){
	//will hold the linux command and its parameters
	char* params[10];
	//raw user input
	char input[100];
	printf("Enter Command and its parameters (if any): ");
	fgets(input, 100, stdin);
	char* token = strtok(input, " ");
	int i = 0;

	//parsing input string into parameters
	while(token){
		params[i] = token;
		for(int j = 0; j < strlen(params[i]); j++){
			//removing any newline charachters
			if(params[i][j] == '\n'){
				params[i][j] = '\0';
				params[i+1] = NULL;
				token = NULL;
				break;
			}
		}
		token = strtok(NULL, " ");
		i++;
	}
	
	//Trimming down unnececary elements of params
	char* temp[10];
	for(int i = 0; i < 10; i++){
		temp[i] = params[i];
		if(params[i] == NULL){
			break;
		}
	}
	
	//Checking for exit conditions
	if(strcmp(temp[0], "quit")==0){
		printf("Exiting Shell\n");
		printf("------------------------\n");
		exit(0);
	}

	//Forking this parent process into a parent and a child
	int pid = fork();
	struct rusage process;
	if(pid < 0){
		//error has occured
		printf("Error forking\n");
		return -1;
	}else if(pid > 0){
		//parent
		//waiting for child to terminate
		wait(NULL);
		getrusage(RUSAGE_CHILDREN, &process);
		printf("CPU Time used:%f\n", process.ru_utime);
		printf("Involuntary context switches: %d\n\n", process.ru_nivcsw);
	}else{
		//child
		//executing command
		if(execvp(temp[0], temp) != 0){
			printf("Invalid Linux Command\n");
		}
	}
	}
	return 0;
}
