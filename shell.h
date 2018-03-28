#define _GNU_SOURCE
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<time.h>
#include<stdint.h>
#include <sys/types.h>
#include <sys/time.h>

#define MAX_NO_OF_COMMANDS 25
#define NO_OF_TOKENS 10
#define COMMAND_LENGTH 40
#define MAX_ALIASES MAX_NO_OF_COMMANDS
#define OPTION_LENGTH COMMAND_LENGTH/2
#define PATH_LENGTH 40
#define MAX_FILE_SIZE 256
#define MAX_FILENAME_LENGTH 25
#define TIMESTAMP_LENGTH 25

#define EOFile '\0'
#define EOLine '\n'
#define PATH_SEPARATOR '/'



char** split(char buf[],int length);
char** aliasNames;
char** aliasDecoded;

struct history
{
	char* command;
	pid_t commandPid;
	char* time;
};

struct history historyTable[MAX_NO_OF_COMMANDS];

int noOfCommands;

char* env[]={"USER=student","PATH=~/Usp_shell_2018","PWD=~/USP/Usp_shell_2018","SHELL=~/USP",NULL};
