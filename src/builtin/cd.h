#ifndef _CD_H_
#define _CD_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../structures/assignment.h"
#include "../structures/command.h"

int cd(struct arg_list *args);



#endif //_CD_H_
