#define _GNU_SOURCE
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

char* getval(char key[],char splitKey)
{
	int len=strlen(key);
	int i=0;
	int prev;
	prev=i;
	char* val=(char *)malloc(sizeof(char)*20);;
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

int main(int argc, char** argv)
{ 
	pid_t pid;
	int status;
	char* env[]={"USER=student","PATH=~/Usp_shell_2018","PWD=~/USP/Usp_shell_2018","SHELL=~/USP",NULL};
	char* buf=(char*)malloc(sizeof(char)*100);
	int i=0;
	char c;

	char* prompt=(char *)malloc(sizeof(char)*20);
	prompt=getval(env[0],'=');

	char* path=(char *)malloc(sizeof(char)*50);
	path=getenv("PWD");

	printf("%s:",prompt);
	printf("%s",path);
	printf("%% ");

	c=getchar();
	while(c!='\n')
	{
		buf[i++]=c;
		c=getchar();
	}
	buf[strlen(buf)] = '\0';
	
	char *gdir=(char *)malloc(sizeof(char)*20);
	char*dir=(char *)malloc(sizeof(char)*20);
	char*to=(char *)malloc(sizeof(char)*20);
	
	i=0;
	char** splitString=split(buf,strlen(buf));
	char dummy[1000];
	while(1)
	{
		
		//fflush(stdout);
		if (!strcmp(splitString[0], "exit"))
		{
			exit(0);
		}  

        	if (!strcmp(splitString[0], "cd"))
		{
			//To change path for cd ..
            		gdir = getcwd(dummy, sizeof(dummy));
			printf("###############################\n");
			printf("%s\n",gdir);
			printf("###############################\n");
            		dir = strcat(gdir, "/");

			printf("###############################\n");
			printf("%s\n",dir);
			printf("###############################\n");
            		to = strcat(dir, splitString[1]);

			printf("###############################\n");
			printf("%s\n",to);
			printf("###############################\n");
           		chdir(to);
			if(splitString[1]=="..")
			{
				
			}
			else
			{
			path=to;
			}
			printf("%s:",prompt);
			printf("%s",path);	
			printf("%% ");
			c=getchar();
			i=0;

			while(c!='\n')
			{
				buf[i++]=c;
				c=getchar();
			}
			buf[i] = '\0';
			splitString=split(buf,strlen(buf));
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

		printf("%s:",prompt);
		printf("%s",path);	
		printf("%% ");
		c=getchar();
		i=0;

		while(c!='\n')
		{
			buf[i++]=c;
			c=getchar();
		}
		buf[i] = '\0';
		splitString=split(buf,strlen(buf));	
	
	}	
	
	exit(0);	
}
