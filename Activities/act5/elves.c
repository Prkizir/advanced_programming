/*
* Sergio Mercado A01020382
* Semaphores : Elves
* 25/10/2019
*/

#include "header.h"

void elf(char * program){
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
    sleep(1);
    mutex_wait(semid, ELFTEX);
    mutex_wait(semid, MUTEX);

    sem_signal(semid, ELVES, 1);
    printf("The elf %i has a problem\n", getpid());
    int cnt = semctl(semid,ELVES,GETVAL,0);

    if(cnt == 3){
      printf("3 elves have problems\n");
      mutex_signal(semid, SANTASEM);
    }else{
      mutex_signal(semid, ELFTEX);
    }

    mutex_signal(semid, MUTEX);

    printf("The elves are getting help\n");

    mutex_wait(semid, MUTEX);

    printf("An elf is leaving\n");
    sem_wait(semid, ELVES, 1);
    cnt = semctl(semid, ELVES, GETVAL, 0);

    if(cnt == 0){
      mutex_signal(semid, ELFTEX);
      printf("More elves arriving\n");
    }

    mutex_signal(semid, MUTEX);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int elves, i, pid;

  if(argc < 2){
    printf("usage: %s [number_of_elves]\n", argv[0]);
    return -1;
  }

  elves = atoi(argv[1]);

  if(elves < 3){
    printf("The number of elves must be a positive number greater than 3\n");
    return -1;
  }

  for(i = 0; i < elves; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      elf(argv[0]);
    }else{
      //donothing
    }
  }

  return 0;
}
