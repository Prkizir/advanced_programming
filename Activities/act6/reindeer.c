/*
* Sergio Mercado A01020382
* Semaphores : Reindeer
* 25/10/2019
*/

#include "header.h"

void reindeer(char * program){
  int semid, i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 6, 0666)) < 0){
    perror(program);
    exit(-1);
  }

  while(1){
    mutex_wait(semid, MUTEX);
    sem_signal(semid,REINDEER,1);

    printf("A reindeer is available\n");
    int cnt = semctl(semid, REINDEER, GETVAL, 0);

    if(cnt == 9){
      printf("There are 9 reindeers available\n");
      mutex_signal(semid, SANTASEM);
    }

    mutex_signal(semid, MUTEX);
    mutex_wait(semid, REINDEERSEM);

    printf("Reindeers go hitching\n");
    sleep(1);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int i, pid;

  for(i = 0; i < 9; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      reindeer(argv[0]);
    }else{
      //donothing
    }
  }

  return 0;
}
