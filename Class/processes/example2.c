#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process(){
  int time;

  srand(getpid());

  time = rand() % 5;
  printf("PID = %i, PPID = %i, SLEEP = %i\n",getpid(),getppid(),time);

  sleep(time);
  printf("PID = %i, PPID = %i\n",getpid(),getppid());

  exit(0);
}

int main(int argc, char * argv[]){
  int pid, num, result, rid, i;

  if(argc != 2){
    fprintf(stderr, "Usage: %s [number_of_childs]\n", argv[0]);
    return -1;
  }

  num = atoi(argv[1]);

  if(num <= 1){
    fprintf(stderr, "%s: number_of_childs must be a positive number greater than 1\n", argv[0]);
    return -2;
  }

  for(i = 0; i < num; i++){
    printf("PID = %i - creating another process...\n",getpid());

    if((pid = fork()) < 0){
      perror("fork");
      exit(-3);
    }else if(pid == 0){
      child_process();
    }else{
      rid = wait(&result);
      printf("PID = %i, CPID = %i\n", getpid(), rid );
    }
  }

  return 0;
}
