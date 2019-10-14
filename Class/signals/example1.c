#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define SIZE 3

void handler(int sig){
  printf("PID %i - SIGNAL - %i\n", getpid(), sig);
}

void child_process(){
  signal(SIGUSR1 | SIGUSR2, handler);

  for(;;){
    pause();
  }

  exit(0);
}

int main(int argc, char const *argv[]) {
  int pids[SIZE], i;

  for(i = 0; i < SIZE; i++){
    if((pids[i] = fork()) < 0){
      perror("fork");
    }else if(pids[i] == 0){
      child_process();
    }else{
      //do nothing
    }
  }

  sleep(1);

  for(i = 0; i < (SIZE * 2); i++){
    kill(pids[i % 3], SIGUSR1);
  }

  sleep(8);

  for(i = 0; i < SIZE; i++){
    kill(pids[i], SIGKILL);
  }

  return 0;
}
