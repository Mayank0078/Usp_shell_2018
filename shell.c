#define _GNU_SOURCE
#include "shell.h"

int checkPipe(char** splitString)
{
	int i=0;
	int pipeCount=0;
	int redirectionCount=0;
	int flag=0;
	
	while(splitString[i]!=EOFile)
	{
		//printf("---------%s--------\n",splitString[i]);
		if(splitString[i][0]=='|')
		{
			flag=1;
			pipeCount++;
		}
		else if(splitString[i][0]=='>')
		{
			flag=1;
			redirectionCount++;
		}
		else if(splitString[i][0]=='<')
		{
			flag=1;
		}
		else if(splitString[i][0]=='&')
		{
			flag=1;
		}
		i++;
	}
	if(flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

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
		splitString[splitCount][i-prev]='\0';
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
	char hostname[1204] = "";
	gethostname(hostname, sizeof(hostname));
	printf("%s@%s:%s", prompt, hostname, path);
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

int executeShell()
{
	pid_t pid;
	int status;
	int flag=0;
	char* buf=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	int z=0;
	char c;
	char* prompt=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char* path=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char*dir=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char*to=(char *)malloc(sizeof(char)*PATH_LENGTH);
	char** splitString;
	
	//prompt=getenv("USERNAME");
	prompt=env[0];
	path=getenv("PWD");
	z=0;
	int pflag=0;

	while(1)
	{
		int i = 0;
		int j = 0;

		pflag=0;
		noOfCommands++;
		displayPrompt(prompt,path);
		buf=getInput();
		//printf("***********%s********\n",buf);	
		saveLogs(buf);
		pid=getpid();
		splitString=split(buf,strlen(buf));
		char **args;
		/*if(checkPipe(splitString))
		{
			updateTimestamp();  
			updatePid(pid);
			pipeHandler(splitString);
		}
		else
		{*/
		args=splitString;

		if(checkPipe(splitString))
		{
			pflag=1;
		}
		if(flag)
		{
			for(z=0; z<MAX_ALIASES; z++)
			{
				if(!strcmp(splitString[0], aliasNames[z]))
				{
					splitString = split(aliasDecoded[z], strlen(aliasDecoded[z]));
					//printf("%s\n", aliasDecoded[i]);
					//printf("%s\n", splitString[0]);
					break;
				}
			}
		}
		if(pflag==1)
	 	{
			updateTimestamp();  
			updatePid(pid);
			commandHandler(args);
		 	continue;
		}
		else if(!strcmp(splitString[0], "exit"))
		{
			updateTimestamp();  
			updatePid(pid);	
			exit(0);
		}  
		else if(!strcmp(splitString[0],"history"))
		{
			updateTimestamp();  
			updatePid(pid);	
			getHistory();
			continue;
		}
		else if(!strcmp(splitString[0],"m3"))
		{
			updateTimestamp();  
			updatePid(pid);	
			char* filename=(char*)malloc(sizeof(char)*MAX_FILENAME_LENGTH);
			strcpy(filename,splitString[1]);
			openEditor(filename);
			continue;
		}
		else if(!strcmp(splitString[0], "source"))
		{
			updateTimestamp();  
			updatePid(pid);
			char* filename=(char*)malloc(sizeof(char)*MAX_FILENAME_LENGTH);
			strcpy(filename,splitString[1]);
			source(filename);
			flag = 1;
			continue;
		}
	  else if(!strcmp(splitString[0], "cd"))
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
				//if(checkValidPath(path)
	      dir = strcat(path,"/");	
				to = strcat(dir, splitString[1]);
				path=to;
			}
			//printf("%s\n",path);
			if(chdir(path)!=0)
			{
				strcpy(path,currentPath);
				fprintf(stderr, "Shell: can’t cd: %s\n", strerror(errno));				
			}
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
			execvpe(splitString[0],splitString, environ);
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

int commandHandler(char * args[])
{
	int i=0;
	int j=0;
	int aux;
	char *args_aux[256];
	int background = 0;
	//printf("-----------------I am inside----------\n");
	while(args[j] != EOFile)
	{
		if ((strcmp(args[j],">") == 0) || (strcmp(args[j],"<") == 0) || (strcmp(args[j],"&") == 0))
		{
			break;
		}
		args_aux[j] = args[j];
		j++;
	}
	
	if(!strcmp(args[0],"history"))
	{
		getHistory();
	}
	
	while (args[i] != NULL && background == 0)
	{
	// If background execution was solicited (last argument '&')
	// we exit the loop
		if (strcmp(args[i],"&") == 0)
		{
			background = 1;
		// If '|' is detected, piping was solicited, and we call
		// the appropriate method that will handle the different
		// executions
		}
		else if (strcmp(args[i],"|") == 0)
		{
			pipeHandler(args);
		// If '<' is detected, we have Input and Output redirection.
		// First we check if the structure given is the correct one,
		// and if that is the case we call the appropriate method
			return 1;
		}
		else if (strcmp(args[i],"<") == 0)
		{
			aux = i+1;
			if (args[aux] == NULL || args[aux+1] == NULL || args[aux+2] == NULL )
			{
				printf("Not enough input arguments\n");
				return -1;
			}
			else
			{
				if (strcmp(args[aux+1],">") != 0)
				{
					printf("Usage: Expected '>' and found %s\n",args[aux+1]);
					return -2;
				}
			}
			fileIO(args_aux,args[i+1],args[i+3],1);
			return 1;
		}
		// If '>' is detected, we have output redirection.
		// First we check if the structure given is the correct one,
		// and if that is the case we call the appropriate method
		else if (strcmp(args[i],">") == 0)
		{
			if (args[i+1] == NULL)
			{
				printf("Not enough input arguments\n");
				return -1;
			}
			fileIO(args_aux,NULL,args[i+1],0);
			return 1;
		}
		i++;
	}
	args_aux[i] = NULL;
	launchProg(args_aux,background);	
}

void pipeHandler(char * args[])
{
	// File descriptors
	int filedes[2]; 
	int filedes2[2];
	
	int num_cmds = 0;
	
	char *command[256];
	
	pid_t pid;
	
	int err = -1;
	int end = 0;
	
	// Variables used for the different loops
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	// First we calculate the number of commands (they are separated
	// by '|')
	while (args[l] != NULL)
	{
		if (strcmp(args[l],"|") == 0)
		{
			num_cmds++;
		}
		l++;
	}
	num_cmds++;
	
	// Main loop of this method. For each command between '|', the
	// pipes will be configured and standard input and/or output will
	// be replaced. Then it will be executed
	while (args[j] != NULL && end != 1)
	{
		k = 0;
		// We use an auxiliary array of pointers to store the command
		// that will be executed on each iteration
		while (strcmp(args[j],"|") != 0)
		{
			command[k] = args[j];
			j++;	
			if (args[j] == NULL)
			{
				// 'end' variable used to keep the program from entering
				// again in the loop when no more arguments are found
				end = 1;
				k++;
				break;
			}
			k++;
		}
		// Last position of the command will be NULL to indicate that
		// it is its end when we pass it to the exec function
		command[k] = NULL;
		j++;		
		
		// Depending on whether we are in an iteration or another, we
		// will set different descriptors for the pipes inputs and
		// output. This way, a pipe will be shared between each two
		// iterations, enabling us to connect the inputs and outputs of
		// the two different commands.
		if (i % 2 != 0)
		{
			pipe(filedes); // for odd i
		}
		else
		{
			pipe(filedes2); // for even i
		}
		
		pid=fork();
		
		if(pid==-1)
		{			
			if (i != num_cmds - 1)
			{
				if (i % 2 != 0)
				{
					close(filedes[1]); // for odd i
				}
				else
				{
					close(filedes2[1]); // for even i
				} 
			}			
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0)
		{
			// If we are in the first command
			if (i == 0)
			{
				dup2(filedes2[1], STDOUT_FILENO);
			}
			// If we are in the last command, depending on whether it
			// is placed in an odd or even position, we will replace
			// the standard input for one pipe or another. The standard
			// output will be untouched because we want to see the 
			// output in the terminal
			else if (i == num_cmds - 1)
			{
				if (num_cmds % 2 != 0)
				{ // for odd number of commands
					dup2(filedes[0],STDIN_FILENO);
				}
				else
				{ // for even number of commands
					dup2(filedes2[0],STDIN_FILENO);
				}
			// If we are in a command that is in the middle, we will
			// have to use two pipes, one for input and another for
			// output. The position is also important in order to choose
			// which file descriptor corresponds to each input/output
			}
			else
			{ // for odd i
				if (i % 2 != 0)
				{
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}
				else
				{ // for even i
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}

			if(strcmp(args[0],"history"))
			{
				if (execvp(command[0],command)==err)
				{
					fprintf(stderr, "Shell: can’ t %s: %s\n",command[0], strerror(errno));		
					kill(getpid(),SIGTERM);
				}		
			}
			kill(getpid(),SIGTERM);
		}
				
		// CLOSING DESCRIPTORS ON PARENT
		if (i == 0)
		{
			close(filedes2[1]);
		}
		else if (i == num_cmds - 1)
		{
			if (num_cmds % 2 != 0)
			{					
				close(filedes[0]);
			}
			else
			{					
				close(filedes2[0]);
			}
		}
		else
		{
			if (i % 2 != 0)
			{					
				close(filedes2[0]);
				close(filedes[1]);
			}
			else
			{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}
				
		waitpid(pid,NULL,0);
				
		i++;	
	}
}

void fileIO(char * args[], char* inputFile, char* outputFile, int option)
{
	 
	int err = -1;
	
	int fileDescriptor; // between 0 and 19, describing the output or input file
	
	if((pid=fork())==-1)
	{
		printf("Child process could not be created\n");
		return;
	}
	if(pid==0)
	{
		// Option 0: output redirection
		if (option == 0)
		{
			// We open (create) the file truncating it at 0, for write only
			fileDescriptor = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600); 
			// We replace de standard output with the appropriate file
			dup2(fileDescriptor, STDOUT_FILENO); 
			close(fileDescriptor);
		// Option 1: input and output redirection
		}
		else if (option == 1)
		{
			// We open file for read only (it's STDIN)
			fileDescriptor = open(inputFile, O_RDONLY, 0600);  
			// We replace de standard input with the appropriate file
			dup2(fileDescriptor, STDIN_FILENO);
			close(fileDescriptor);
			// Same as before for the output file
			fileDescriptor = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			dup2(fileDescriptor, STDOUT_FILENO);
			close(fileDescriptor);		 
		}
		 
		//setenv("parent",getcwd(currentDirectory, 1024),1);
		if (execvp(args[0],args)==err)
		{
			printf("err");
			kill(getpid(),SIGTERM);
		}		 
	}
	//printf("======================\n");
	waitpid(pid,NULL,0);
	//printf("&&&&&&&&&&&&&&&&&&&&&\n");
}


void launchProg(char *args[], int background)
{	 
	int err = -1;
	 
	if((pid=fork())==-1)
	{
	 printf("Child process could not be created\n");
	 return;
	}
	 // pid == 0 implies the following code is related to the child process
	if(pid==0)
	{
		// We set the child to ignore SIGINT signals (we want the parent
		// process to handle them with signalHandler_int)	
		signal(SIGINT, SIG_IGN);
		
		// We set parent=<pathname>/simple-c-shell as an environment variable
		// for the child
		setenv("parent",getcwd(currentDirectory, 1024),1);	
		
		// If we launch non-existing commands we end the process
		if (execvp(args[0],args)==err)
		{
			printf("Command not found");
			kill(getpid(),SIGTERM);
		}
	 }
	 
	 // The following will be executed by the parent
	 
	 // If the process is not requested to be in background, we wait for
	 // the child to finish.
	 if (background == 0)
	 {
		 waitpid(pid,NULL,0);
	 }
	 else
	 {
		 // In order to create a background process, the current process
		 // should just skip the call to wait. The SIGCHILD handler
		 // signalHandler_child will take care of the returning values
		 // of the childs.
		 printf("Process created with PID: %d\n",pid);
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
	//printf("%s\n",env[2]);
	env[3] = getenv("SHELL");
	initializeShell();
	executeShell();
	return(0);	
}

