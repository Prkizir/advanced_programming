/*
* Sergio Mercado A01020382
* Exam 3 : Car
* 19/11/2019
*/
#include "header.h"

int main(int argc, char *argv[]) {
  int t, passengers, i;

  if(argc != 2){
    printf("usage: %s [number_of_rides]\n", argv[0]);
    return -1;
  }

  t = atoi(argv[1]);

  int semid;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(argv[0]);
    return -1;
  }

  if((semid = semget(key, 8, 0666)) < 0){
    perror(argv[0]);
    return -1;
  }

  for(i = 0; i < t; i++){
    printf("Car waiting for people to board...\n");
    sem_signal(semid, BOARDQUEUE, MAX);
    mutex_wait(semid, ALLABOARD);

    printf("The car is running...\n");

    printf("Car waiting for people to unboard...\n");
    sem_signal(semid, UNBOARDQUEUE, MAX);
    mutex_wait(semid, ALLASHORE);

    sleep(1);
  }

  printf("Car going into maintenance...\n");

  return 0;
}
