#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "proto.h"

void prompt(){
	char curr_dir[100];
	if(getcwd(curr_dir, 100) == NULL){
		fprintf(stderr, "Error getting current directory: %s\n", strerror( errno ));
	}
	else{
		printf("%s$", curr_dir);
	}
}
