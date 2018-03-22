#define _GNU_SOURCE
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

#define MAX_NO_OF_COMMANDS 25
#define NO_OF_TOKENS 10
#define COMMAND_LENGTH 40
#define OPTION_LENGTH COMMAND_LENGTH/2
#define PATH_LENGTH 20

#define EOFile '\0'
#define EOLine '\n'
#define PATH_SEPARATOR '/'

char** split(char buf[],int length);

char** commandHistory;
int noOfCommands;
char* env[]={"USER=student","PATH=~/Usp_shell_2018","PWD=~/USP/Usp_shell_2018","SHELL=~/USP",NULL};
