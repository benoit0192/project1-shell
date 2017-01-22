#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proto.h"

#define	MAX_CHAR	100

int main(int argc, char *argv[]){

	int pfd[2], status;
	pid_t cpid;
	while(1){
		char *args[50] = {NULL};
		pipe(pfd);
		char cmd[MAX_CHAR];

		prompt();
		fgets(cmd, MAX_CHAR, stdin);
		string_processing(cmd, args);
			
		cpid = fork();
		if(cpid > 0){	//parent process
			//printf("Parent PID: %d\n", getpid());
			//waitpid(1, &status, 0);		//wait for every child
			wait(&status);		//wait for every child
		}
		else if(cpid == 0){
			//printf("Child PID: %d\n", cpid);
			execv(args[0], args);
			exit(status);
		}
		else{
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}	
