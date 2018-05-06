#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc,char*argv[]){
  if(argc>1){
    int fd=open(argv[1], O_RDONLY);
    if(fd==-1)
      printf("can't read the file ");
    else{
      char buf[1024];
      int result=read(fd, buf, 1024);
      if(result==-1)
	printf("cant't read the file");
      else{
	while(result>0){
	  printf("%s", buf);
	  result=read(fd, buf, 1024);
	}
      }
      
    }
    close(fd);
  }
  return 0;
}
