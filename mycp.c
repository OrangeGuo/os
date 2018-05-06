#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char*argv[]){
  if(argc>1){
    int fd=open(argv[1], O_RDONLY);
    int td=creat(argv[2], 777);
    if(fd==-1||td==-1)
      printf("can't read or create the file\n");
    else{
      char buf[1024];
      int result=read(fd, buf, 1024);
      if(result==-1)
	printf("cant't read the file\n");
      else{
	while(result>0){
	  write(td, buf,result);
	  result=read(fd, buf, 1024);
	}
      }
      
    }
    close(fd);
    close(td);
  }
  return 0;
}
