#define _GNU_SOURCE
#include "test1.h"

/*struct history
{
	char** commandHistory;
	int noOfCommands;
};

typedef struct history history; 

history* logs;
*/



char** split(char buf[],int length)
{
	char** splitString=(char**)malloc(sizeof(char*)*NO_OF_TOKENS);
	int i=0;
	for(i=0;i<NO_OF_TOKENS;i++)
	{
		splitString[i]=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	}
	int prev;
	int splitCount=0;
	printf("%s\n",buf);
	i=0;
	prev=i;
	while(i<length)
	{
		if(buf[i]==' ')
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev));
			splitCount++;
			prev=i+1;
		}
		else if(buf[i+1]==EOFile)
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev+1));
			splitCount++;
			prev=i+1;
		}
		i++;	
	}

	splitString[splitCount++]=EOFile;
	return(splitString);
}

char* getval(char key[],char splitKey)
{
	int len=strlen(key);
	int i=0;
	int prev;
	prev=i;
	char* val=(char *)malloc(sizeof(char)*OPTION_LENGTH);;
	while(i<len)
	{
		if(key[i]==splitKey)
		{
			strncpy(val,&key[i+1],len-(i));
		}
		i++;
	}
	return(val);

}

char* getPreviousPath(char* path)
{
	int i=0;
	int prev=i;
	char* prevPath=(char*)malloc(sizeof(char)*strlen(path));
	while(path[i]!=EOFile)
	{
		if(path[i]==PATH_SEPARATOR)
		{
			prev=i;
		}	
		i++;
	}
	strncpy(prevPath,&path[0],prev);
	return(prevPath);
}

void displayPrompt(char* prompt,char* path)
{
	printf("%s:",prompt);
	printf("%s",path);
	printf("%% ");
}

char* getInput()
{
	int i=0;
	char c;
	char* buf=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	c=getchar();
	while(c!=EOLine)
	{
		buf[i++]=c;
		c=getchar();
	}
	buf[i] = EOFile;
	return(buf);
}

char* charToString(char c)
{
	char *pathSeparator = &c;
	return(pathSeparator);
}

void saveLogs(char* buf)
{
	if(noOfCommands>=MAX_NO_OF_COMMANDS)
	{
		noOfCommands=0;
	}
	strncpy(commandHistory[noOfCommands],&buf[0],strlen(buf));
	noOfCommands++;
}

void getHistory()
{
	int i=0;
	for(i=0;i<noOfCommands;i++)
	{
		printf("  %d  %s\n",(i+1),commandHistory[i]);
	}
}

void executeShell()
{
	pid_t pid;
	int status;

	char* buf=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	int i=0;
	char c;

	char* prompt=(char *)malloc(sizeof(char)*PATH_LENGTH);
	prompt=getval(env[0],'=');

	char* path=(char *)malloc(sizeof(char)*PATH_LENGTH);
	path=getenv("PWD");

	char*dir=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char*to=(char *)malloc(sizeof(char)*PATH_LENGTH);
	
	i=0;
	char** splitString;

	while(1)
	{

		displayPrompt(prompt,path);
		buf=getInput();
		saveLogs(buf);
		splitString=split(buf,strlen(buf));
		
		if (!strcmp(splitString[0], "exit"))
		{
			exit(0);
		}  
		if(!strcmp(splitString[0],"history"))
		{
			getHistory();
			continue;
		}
    if (!strcmp(splitString[0], "cd"))
		{
			//To change path for cd ..
      //gdir = getcwd(dummy, sizeof(dummy));
      

			if(!strcmp(splitString[1],".."))
			{
				path=getPreviousPath(path);
			}
			else
			{
        dir = strcat(path, charToString(PATH_SEPARATOR));	
				to = strcat(dir, splitString[1]);
				path=to;
			}

      chdir(path);
  
			continue;
   	} 
   	  
		if((pid=fork()) ==-1)
		{
			fprintf(stderr, "Shell: can’ t fork: %s\n", strerror(errno));
			continue;
		}	
		else if (pid == 0)
		{
			//Child
			execvpe(splitString[0],splitString,env);
			fprintf(stderr, "Shell: couldn’t exec %s: %s\n", buf, strerror(errno));
			exit(0);
		}
			
		if ((pid=waitpid(pid,&status, 0))<=0)
		{	
			fprintf(stderr,"shell: waitpid error: %s\n",strerror(errno));
		}
	
	}	
	
}

void initializeShell()
{
	commandHistory=(char**)malloc(sizeof(char*)*MAX_NO_OF_COMMANDS);
	int i=0;
	for(i=0;i<MAX_NO_OF_COMMANDS;i++)
	{
		commandHistory[i]=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	}
	noOfCommands=0;
}

int main(int argc, char** argv)
{ 
	initializeShell();
	executeShell();
	return(0);	
}
