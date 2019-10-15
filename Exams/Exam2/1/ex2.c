#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

void caps(char * message, char * program){
  int fd_pipe1[2],fd_pipe2[2], pid;

  //Begin Pipe allocation

  if(pipe(fd_pipe1) < 0){
    perror("pipe 1");
    exit(-1);
  }

  if(pipe(fd_pipe2) < 0){
    perror("pipe 2");
    exit(-2);
  }

  //End Pipe allocation

  //Forking

  if((pid = fork()) < 0){
    perror("fork");
    exit(-3);
  }else if(pid == 0){ //Child Process
    char caps_message[MAX_LENGTH];

    close(fd_pipe1[1]);
    read(fd_pipe1[0],caps_message,MAX_LENGTH);
    close(fd_pipe1[0]);

    int i;
    for(i = 0; i < MAX_LENGTH; i++){
      caps_message[i] = toupper(caps_message[i]);
    }

    close(fd_pipe2[0]);
    write(fd_pipe2[1],caps_message,MAX_LENGTH);
    close(fd_pipe2[1]);

    exit(0);
  }else{  //Parent
    close(fd_pipe1[0]);
    printf("Before:\n%s\n",message);
    write(fd_pipe1[1],message,MAX_LENGTH);
    close(fd_pipe1[1]);

    wait(NULL);

    close(fd_pipe2[1]);
    read(fd_pipe2[0],message,MAX_LENGTH);
    printf("After:\n%s\n",message);
    close(fd_pipe2[0]);
  }
}

int main(int argc, char *argv[]) {

  char message[MAX_LENGTH];

  if(argc > 1){
    printf("usage: %s\n", argv[0]);
    exit(-1);
  }else{
    scanf("%[^\n]",message);
    caps(message,argv[0]);
  }

  return 0;
}
