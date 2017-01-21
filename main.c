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
		char *token, *delimiter=" ";
		pipe(pfd);
		char cmd[MAX_CHAR];

		prompt();
		fgets(cmd, MAX_CHAR, stdin);
		cmd[strcspn(cmd, "\r\n")] = 0;	//get rid of the trailing new line character
		char *dup_cmd = strdup(cmd);
		for(int i=0;(token = strsep(&dup_cmd, delimiter)) != NULL; i++){
			args[i] = token;
			//printf("%s\n", args[i]);
		}
			
		cpid = fork();
		if(cpid > 0){	//parent process
			printf("Hello, parent's PID: %d\n", getpid());
			//waitpid(1, &status, 0);		//wait for every child
			wait(&status);		//wait for every child
		}
		else if(cpid == 0){
			printf("Child's PID: %d\n", cpid);
			char *args2[3];	//null-terminated string
			execv(args[0], args);
			exit(status);
		}
		else{
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}	
