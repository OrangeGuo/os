#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int mysys(char* command)
{
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

int main(){
    int mysys(char* command);
    printf("--------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------\n");
    mysys("ls /");
    printf("--------------------\n");
    return 0;
}
