/*
* Sergio Mercado A01020382
* Semaphores : Santa
* 25/10/2019
*/

#include "header.h"

int main(int argc, char *argv[]) {

  if(argc != 1){
    printf("usage: %s\n",argv[0]);
    return -1;
  }

  int semid, i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(argv[0]);
    return -1;
  }

  if((semid = semget(key, 6, 0666)) < 0){
    perror(argv[0]);
    return -1;
  }

  while(1){
    printf("Santa is sleeping\n");
    mutex_wait(semid, SANTASEM);
    mutex_wait(semid, MUTEX);

    int reindeers = semctl(semid, REINDEER, GETVAL, 0);
    int elves = semctl(semid, ELVES, GETVAL, 0);

    if(reindeers >= 9){
      printf("Santa getting the sleigh ready\n");
      sem_signal(semid, REINDEERSEM, 9);
      sem_wait(semid, REINDEERSEM, 9);
    }else if(elves == 3){
      printf("Santa is helping the elves\n");
    }

    mutex_signal(semid, MUTEX);
    sleep(1);
  }

  return 0;
}
