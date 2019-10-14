#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
  int pid;

  if(argc != 1){
    fprintf(stderr, "Usage: %s\n", argv[0]);
    return -1;
  }

  if((pid = fork()) < 0){
    perror("Fork");
    return -2;
  }else if(pid == 0){
    printf("id = %i - CHILD PROCESS\n", getpid());
    sleep(1);
    exit(0);
  }else{
    waint(NULL);
    printf("id = %i - PARENT PROCESS\n",getpid());
  }

  return 0;
}
