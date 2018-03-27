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
#define MAX_ALIASES MAX_NO_OF_COMMANDS
#define MAX_FILE_SIZE 256
#define MAX_FILENAME_LENGTH 25

#define EOFile '\0'
#define EOLine '\n'
#define PATH_SEPARATOR '/'

char** split(char buf[],int length);

char** commandHistory;
int noOfCommands;
char* env[]={"USER=student",NULL};

char** aliasNames;
char** aliasDecoded;
