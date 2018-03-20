#include "test1.h"

char** split(char buf[],int length)
{
	char** splitString=(char**)malloc(sizeof(char*)*NO_OF_COMMANDS);
	int i=0;
	for(i=0;i<NO_OF_COMMANDS;i++)
	{
		splitString[i]=(char*)malloc(sizeof(char)*COMMAND_LENGTH);
	}
	int prev;
	int splitCount=0;
	printf("%s\n",buf);
	i=0;
	prev=i;
	//while(buf[i]!='\0')
	while(i<length)
	{
		if(buf[i]==' ')
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev));
			//printf("----------%s\n",splitString[splitCount]);
			splitCount++;
			prev=i+1;
		}
		else if(buf[i+1]=='\0')
		{
			strncpy(splitString[splitCount],&buf[prev],(i-prev+1));
			//printf("----------%s\n",splitString[splitCount]);
			splitCount++;
			prev=i+1;
		}
		i++;	
	}

	splitString[splitCount++]='\0';
	return(splitString);
}


int main(int argc, char** argv)
{ 
	pid_t pid;
	int status;
	char* env[]={"USER=student","PATH=/home/manoj/Documents/USP/Usp_shell_2018","PWD=/home/manoj/Documents/USP/Usp_shell_2018","SHELL=/home/manoj/Documents/USP",NULL};
	char* buf=(char*)malloc(sizeof(char)*100);
	int i=0;
	char c;
	printf("%% ");
	c=getchar();
	while(c!='\n')
	{
		buf[i++]=c;
		c=getchar();
	}
	buf[strlen(buf)] = '\0';

	i=0;
	char** splitString=split(buf,strlen(buf));
	
	while(1)
	{
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
			
		if ((pid=waitpid(pid,&status, 0)) < 0)
		{	
			fprintf(stderr,"shell: waitpid error: %s\n",strerror(errno));
		}

		printf("%% ");
		c=getchar();
		i=0;
		while(c!='\n')
		{
			buf[i++]=c;
			c=getchar();
		}
		buf[strlen(buf)] = '\0';
		
		splitString=split(buf,strlen(buf));	
	
	}	
	
	exit(0);	
}
