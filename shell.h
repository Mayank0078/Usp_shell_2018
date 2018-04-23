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
#include <sys/stat.h>
#include <dirent.h>

#define MAX_NO_OF_COMMANDS 25
#define NO_OF_TOKENS 15
#define COMMAND_LENGTH 40
#define MAX_ALIASES MAX_NO_OF_COMMANDS
#define OPTION_LENGTH COMMAND_LENGTH/2
#define PATH_LENGTH 40
#define MAX_FILE_SIZE 256
#define MAX_FILENAME_LENGTH 25
#define TIMESTAMP_LENGTH 25
#define MAX_NO_FILES 50

#define EOFile '\0'
#define EOLine '\n'
#define PATH_SEPARATOR '/'

void pipeHandler(char * args[]);
void fileIO(char * args[], char* inputFile, char* outputFile, int option);
void launchProg(char **args, int background);
int commandHandler(char * args[]);
void listFiles(char*);

extern char** environ;
char* currentDirectory;
char** split(char buf[],int length);
char** aliasNames;
char** aliasDecoded;
pid_t pid;

struct history
{
	char* command;
	pid_t commandPid;
	char* time;
};

struct history historyTable[MAX_NO_OF_COMMANDS];

int noOfCommands;

char** env;
