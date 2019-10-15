/*
* Sergio Mercado
* A01020382
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>

char pipename[256];

void func(){
  int size;
  int fd;
  int randnum[100];

  if((fd = open(pipename, O_RDONLY)) < 0){
    perror("read");
    exit(-1);
  }

  read(fd,&size,sizeof(int));
  read(fd,randnum,size*sizeof(int));

  int i;
  for(i = 0; i < size; i++){
    printf("%i ",randnum[i]);
  }

  int acum = 0;
  for(i = 0; i < size; i++){
    acum = acum + randnum[i];
  }

  float end = acum/size;

  printf("Average: %f\n", end);
}

void handler(int sig){

  switch(sig){
    case SIGUSR1:
    func();
      break;
    case SIGUSR2:
      remove(pipename);
      exit(0);
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {

  int fd_pipe;

  if(argc < 2){
    printf("usage: %s [pipe_name]\n",argv[0]);
    return -1;
  }

  printf("Creating pipe file \"%s\"\n", argv[1]);

  strcpy(pipename,argv[1]);

  mkfifo(argv[1], 0666);

  printf("Process with ID [%i] waiting...\n",getpid());

  signal(SIGUSR1,handler);
  signal(SIGUSR2,handler);

  while(1){
    pause();
  }

  return 0;
}
