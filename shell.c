#define _GNU_SOURCE
#include "shell.h"

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
	
	i=0;
	prev=i;
	while(i<length)
	{
		if(buf[i]==' ')
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev));
			splitString[splitCount][i]='\0';
			splitCount++;
			prev=i+1;
		}
		else if(buf[i+1]==EOFile || buf[i+1]==EOLine)
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev+1));
			splitString[splitCount][i-prev+1]='\0';
			splitCount++;
			prev=i+1;
		}
		i++;	
	}

	splitString[splitCount]=EOFile;
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

	scanf(" %c",&c);
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
	strcpy(historyTable[noOfCommands].command,buf);

}

void getHistory()
{
	int i=0;
	printf("     PID   TIME");
	for(i=0;i<(strlen(historyTable[0].time)-3);i++)
	{
		printf(" ");
	}
	printf("CMD\n");
	for(i=0;i<noOfCommands;i++)
	{
		if(i>9)
		{
			printf("  %d %d  %s %s\n",(i+1),historyTable[i].commandPid,historyTable[i].time,historyTable[i].command);
		}
		else
		{
			printf("   %d %d %s %s\n",(i+1),historyTable[i].commandPid,historyTable[i].time,historyTable[i].command);
		}
	}
}


void source(char* filename)
{
 	FILE *fp;
	fp = fopen(filename ,"r");
  char c = fgetc(fp);

  char* buf=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
  aliasNames = (char**)malloc(sizeof(char*)*MAX_ALIASES);
  aliasDecoded = (char**)malloc(sizeof(char*)*MAX_ALIASES);

  int i,j,k;

  for(i=0;i<MAX_ALIASES;i++)
	{
		aliasDecoded[i]=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
    aliasNames[i]=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	}
  
  k = 0;
  j = 0;

  while(c != EOF)
  {
   	while(c != ' ')
   	{
    	c = fgetc(fp);
    }
    c = fgetc(fp);
		while(c != '=')
		{
     	buf[j] = c;
      j++;
      c = fgetc(fp);
    }
    buf[j]='\0';
    strcpy(aliasNames[k], buf);
    
    c = fgetc(fp);
    j = 0;
    strcpy(buf, "");
		while(c != EOLine)
		{
    	if (c != '\'')
    	{
       	buf[j] = c;
        j++;
      }
      c = fgetc(fp);
    }
    buf[j]='\0';
    strcpy(aliasDecoded[k], buf);
    k++;
		strcpy(buf, "");
    c = fgetc(fp);
    j=0;
	}
}


void openEditor(char filename[])
{
	int i=0;
	char prev;
	char curr;
	char* buf=(char*)malloc(sizeof(char)*MAX_FILE_SIZE);
	char c;
	FILE *fp;
	
	fp = fopen(filename, "a+");
	if(fp==NULL)
	{
		fprintf(stderr, "Shell: couldn't open file %s: %s\n", filename, strerror(errno));
	}
	else
	{
		int j=0;
		while((c = getc(fp)) != EOF)
		{
    	putchar(c);
		}
		scanf(" %c",&prev);
		curr=getchar();
		int len=0;
		buf[len++]=prev;
		while(prev!=':' ||  curr!='q')
		{
			buf[len++]=curr;
			prev=curr;
			curr=getchar();
		}
		buf[len-1]='\0';
		
		fprintf(fp,"%s",buf);
	}
	int fd=fileno(fp);
		
	fclose(fp);
}

void updatePid(pid_t pid)
{
	historyTable[noOfCommands].commandPid=pid;
}

void updateTimestamp()
{
	time_t ltime;
	struct tm *newtime;
	time(&ltime);
  newtime = localtime(&ltime);
  char* timest = asctime(newtime);
  strcpy(historyTable[noOfCommands].time,timest);
	int len=strlen(historyTable[noOfCommands].time);
	historyTable[noOfCommands].time[len-1]='\0';
}

void executeShell()
{
	pid_t pid;
	int status;
	int flag=0;
	char* buf=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	int i=0;
	char c;
	char* prompt=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char* path=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char*dir=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char*to=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char** splitString;
	
	prompt=getenv("USERNAME");
	path=getenv("PWD");
	i=0;

	while(1)
	{
		noOfCommands++;
		displayPrompt(prompt,path);
		buf=getInput();	
		saveLogs(buf);
		pid=getpid();
		splitString=split(buf,strlen(buf));
		if(flag)
		{
			for(i=0; i<MAX_ALIASES; i++)
			{
				if(!strcmp(splitString[0], aliasNames[i])){
					splitString = split(aliasDecoded[i], strlen(aliasDecoded[i]));
					printf("%s\n", aliasDecoded[i]);
					printf("%s\n", splitString[0]);
					break;
				}
			}
		}
		if (!strcmp(splitString[0], "exit"))
		{
		  updateTimestamp();  
			updatePid(pid);	
			exit(0);
		}  
		if(!strcmp(splitString[0],"history"))
		{
		  updateTimestamp();  
			updatePid(pid);	
			getHistory();
			continue;
		}
		if(!strcmp(splitString[0],"m3"))
		{
		  updateTimestamp();  
			updatePid(pid);	
			char* filename=(char*)malloc(sizeof(char)*MAX_FILENAME_LENGTH);
			strcpy(filename,splitString[1]);
			openEditor(filename);
			continue;
		}
		if(!strcmp(splitString[0], "source"))
		{
		  updateTimestamp();  
			updatePid(pid);
			char* filename=(char*)malloc(sizeof(char)*MAX_FILENAME_LENGTH);
			strcpy(filename,splitString[1]);
			source(filename);
			flag = 1;
			continue;
		}
    if(!strcmp(splitString[0], "cd"))
		{
		  updateTimestamp();  
			updatePid(pid);	
			char* currentPath=(char*)malloc(sizeof(char)*PATH_LENGTH);
			strcpy(currentPath,path);
			if(!strcmp(splitString[1],".."))
			{
				path=getPreviousPath(path);
			}
			else
			{
        dir = strcat(path,"/");	
				to = strcat(dir, splitString[1]);
				path=to;
			}
			chdir(path);
			continue;
   	} 
   	updateTimestamp();  
		if((pid=fork()) ==-1)
		{
			fprintf(stderr, "Shell: can’ t fork: %s\n", strerror(errno));
			continue;
		}	
		else if (pid == 0)
		{
			//Child
			execvpe(splitString[0],splitString, env);
			fprintf(stderr, "Shell: couldn’t exec %s: %s\n", buf, strerror(errno));
			exit(0);
		}
		updatePid(pid);	
		if ((pid=waitpid(pid,&status, 0))<=0)
		{	
			fprintf(stderr,"shell: waitpid error: %s\n",strerror(errno));
		}

	}	
	
}

void initializeShell()
{
	for(int i=0;i<MAX_NO_OF_COMMANDS;i++)
	{
		historyTable[i].command=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
		historyTable[i].time=(char*)malloc(sizeof(char)*TIMESTAMP_LENGTH);
	}
	noOfCommands=-1;
}

int main(int argc, char** argv, char* envp[])
{
	env=(char**)malloc(sizeof(char*)*NO_OF_TOKENS);
	env[0] = getenv("USERNAME");
	if(env[0]==NULL)
		env[0] = getenv("LOGNAME");
	
	env[1] = getenv("PWD");
	env[2] = getenv("PATH");
	env[3] = getenv("SHELL");
	initializeShell();
	executeShell();
	return(0);	
}
