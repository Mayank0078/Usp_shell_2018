#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/wait.h>

#define NO_OF_COMMANDS 10
#define COMMAND_LENGTH 25

char** split(char buf[],int length);

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
	while(buf[i]!='\0')
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
	
	return(splitString);
}


int main(int argc, char** argv)
{ 
	pid_t pid;
	int status;
	char* buf=(char*)malloc(sizeof(char)*100);
	int i=0;
	char c;
	
	c=getchar();
	while(c!='\n')
	{
		buf[i++]=c;
		c=getchar();
	}
	buf[strlen(buf)] = '\0';
	
	i=0;
	char** splitString=split(buf,strlen(buf));
	for(i=0;i<2;i++)
	{
		printf("%s\n",splitString[i]);
	}
	
	while(0)
	{
		buf[strlen(buf)] = '\0';
		i=0;
		while(buf[i]!=' ')
		{
			
		}
		printf("%s\n",buf);
		if((pid=fork()) ==-1)
		{
			fprintf(stderr, "Shell: can’ t fork: %s\n", strerror(errno));
			continue;
		}	
		else if (pid == 0)
		{
			//Child
			execlp(buf, buf, (char *)0);
			fprintf(stderr, "Shell: couldn’t exec %s: %s\n", buf, strerror(errno));
			exit(0);
		}
		c=getchar();
		while(c!='\n')
		{
			buf[i++]=c;
			c=getchar();
		}
	}	
	if ((pid=waitpid(pid,&status, 0)) < 0)
	{	
		fprintf(stderr,"shell: waitpid error: %s\n",strerror(errno));
	}	
	
	exit(0);
	
}
