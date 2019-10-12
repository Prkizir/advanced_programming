/*
* Activity - Signals
* Authro: Sergio Mercado A01020382
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

char filename[256];

void exe(){
  int fd;
  char buffer[256];
  char args[2][256];
  int i = 0;

  if((fd = open(filename, O_RDONLY)) < 0){
    printf("Could not open %s\n", filename);
    exit(-1);
  }else{
    read(fd,&buffer,sizeof(buffer));
    close(fd);
  }

  char * token = strtok(buffer,",");
  while(token != NULL){
    strcpy(args[i],token);
    token = strtok(NULL,",");
    i++;
  }

  execlp(args[0],args[1],(char *)0);
}

void handler(int sig){
  int pid;

  switch(sig){
    case SIGUSR1:
       strcpy(filename,"file1.txt");
      break;
    case SIGUSR2:
      strcpy(filename,"file2.txt");
      break;
    case SIGPWR:
      strcpy(filename,"file3.txt");
      break;
    default:
      break;
  }

  if((pid = fork()) < 0){
    perror("fork");
  }else if(pid == 0){
    exe();
  }else{
    printf("Process %i created\n", pid);
  }
}

int main(int argc, char const *argv[]) {

  if(argc != 4){
    fprintf(stderr, "usage: %s [file1] [file2] [file3]\n", argv[0]);
    return -2;
  }

  int i;
  for(i = 1; i < argc; i++){
    int fd;
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("%s: the file %s does not exist\n",argv[0],argv[i]);
      return -(2+i);
    }else{
      if(lseek(fd,0,SEEK_END) == 0){
        printf("%s: the file %s is empty\n",argv[0],argv[i]);
        return -(5+i);
      }
    }

    close(fd);
  }

  printf("Waiting for signal...\n");

  signal(SIGUSR1,handler);
  signal(SIGUSR2,handler);
  signal(SIGPWR,handler);

  while(1){
    pause();
  }

  return 0;
}
