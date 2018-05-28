#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main(){
    printf("current working directory: %s\n", getcwd(NULL,0));
    char input[16]="cd ..";
    char *p=strtok(input," ");
    while(p){
	puts(p);
	p=strtok(NULL," ");
    }
    chdir("/");
    printf("current working directory: %s\n", getcwd(NULL,0));
    return 0;
    
}
