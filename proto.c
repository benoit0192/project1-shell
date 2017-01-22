#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "proto.h"

/**
Display the current directory
**/
void prompt(){
	char curr_dir[100];
	if(getcwd(curr_dir, 100) == NULL){
		fprintf(stderr, "Error getting current directory: %s\n", strerror( errno ));
	}
	else{
		printf("%s$", curr_dir);
	}
}

/**
Arguments:
	cmd: String of the command
	args: Array of strings
Process the command by splitting the string for each argument
**/
void string_processing(char *cmd, char **args){

	char *token, *delimiter=" ";

	cmd[strcspn(cmd, "\r\n")] = 0;	//get rid of the trailing new line character
	char *dup_cmd = strdup(cmd);
	for(int i=0;(token = strsep(&dup_cmd, delimiter)) != NULL; i++){
		args[i] = token;
		//printf("%s\n", args[i]);
	}
}