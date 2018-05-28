#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


int mysys(char* command)
{
	chdir("/home/orange");
    pid_t pid;
    if(command==NULL)
	return 1;
    else if((pid=fork())<0)
	return -1;
    else if(pid==0){
		
	execl("/bin/sh", "sh", "-c",command,NULL);
	_exit(127);
    }else{
	pid_t wait_pid;
	while((wait_pid=wait(NULL))<0){
	    if(errno=EINTR)
		return 0;
	}
    }
    return 0;
}

int quit(char* exit){
    char q[]="exit";
    printf("%s,%d\n",exit, strcmp(q,q));
    if(strcmp(q,exit)!=0)
	return 1;

    return 0;
}
int main(){
    int mysys(char* command);
    int quit(char* exit);
    while(1){
	char command[100];
	printf("$");
	//fgets(command,99,stdin);
	int index=0;
	command[index]=getchar();
	while(command[index]!='\n'){
	    index++;
	    command[index]=getchar();
	}
	command[index]='\0';
	if(strcmp(command,"exit")==0)
	    break;
	else{
	    char* p=strtok(command," ");
	    if(strcmp(p,"cd")==0){
		p=strtok(NULL," ");
		chdir(p);
	    }
	    else if(strcmp(p,"pwd")==0){
		printf("current working directory: %s\n", getcwd(NULL,0));
	    }
	    else if(strcmp(p,"ls")==0){
		if(strcmp(p,command)==0){
		    mysys(strcat(strcat(command," "),getcwd(NULL, 0)));
		}
		else
		    mysys(command);
	    }
	    else mysys(command);
	}
	
    }
    return 0;
}
