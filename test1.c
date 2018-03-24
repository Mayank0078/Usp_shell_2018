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
	
	i=0;
	prev=i;
	while(i<length)
	{
		if(buf[i]==' ')
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev));
			splitString[splitCount][i]='\0';
			//printf("%s\n", splitString[splitCount]);
			splitCount++;
			prev=i+1;
		}
		else if(buf[i+1]==EOFile || buf[i+1]==EOLine)
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev+1));
			splitString[splitCount][i+1]='\0';
			//printf("%s\n", splitString[splitCount]);
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
	//c=getchar();
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
		if(i>9)
		{
			printf("  %d %s\n",(i+1),commandHistory[i]);
		}
		else
		{
			printf("  %d  %s\n",(i+1),commandHistory[i]);
		}
	}
}

void openEditor(char filename[])
{
	int i=0;
	char prev;
	char curr;
	char* buf=(char*)malloc(sizeof(char)*MAX_FILE_SIZE);
	
	/*char c;
	FILE *fp1;
	fp1 = fopen(filename, "r");
	if(fp1==NULL)
	{
		fprintf(stderr, "Shell: couldn't open file %s: %s\n", filename, strerror(errno));
	}
	else
	{
	if (fp1)
	{
    while((c = getc(fp1)) != EOF)
    	putchar(c);
    fclose(fp1);
	}
	//fprintf(b,"%s",buf);
	//}
	//fclose(fp1);
	
	//prev=getchar();
	//curr=getchar();*/
	
	char c;
	FILE *fp;
	fp = fopen(filename, "a+");
	//char* fileContent=(char*)malloc(sizeof(char)*MAX_FILE_SIZE);
	//int fileSize;
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
    	//fileContent[j]=c;
    	//j++;		
		}
		//fileContent[j]='\0';
		//fprintf(stdout,"%s",fileContent);
		//fileSize=j;
		
		scanf(" %c",&prev);
		//prev=getchar();
		/*if(c=='\b')
		{
			fileSize=fileSize-1;
			fileContent[fileSize]='\0';
			fprintf(stdout,"%s",fileContent);			
			//fflush(stdout);
		}*/
		//scanf(" %c",&curr);
		curr=getchar();
		int len=0;
		buf[len++]=prev;
		while(prev!=':' ||  curr!='q')
		{
			buf[len++]=curr;
			prev=curr;
			curr=getchar();
			//scanf(" %c",&curr);
		}
		buf[len-1]='\0';
		
		//printf("%s\n",buf);
		fprintf(fp,"%s",buf);
	}
	int fd=fileno(fp);
	//printf("%d\n",fd);
	/*if(lseek(fd,0,SEEK_SET)==-1)
	{
		fprintf(stderr, "Shell: couldn't lseek: %s\n",strerror(errno));	
	}
	*/
		//fflush(stdin);
	//fflush(stdout);	
	fclose(fp);

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
		if(!strcmp(splitString[0],"m3"))
		{
			char* filename=(char*)malloc(sizeof(char)*MAX_FILENAME_LENGTH);
			strcpy(filename,splitString[1]);
			openEditor(filename);
			continue;
		}
    if (!strcmp(splitString[0], "cd"))
		{
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
