#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/wait.h>

#define NO_OF_COMMANDS 10
#define COMMAND_LENGTH 25

char** split(char buf[],int length);

