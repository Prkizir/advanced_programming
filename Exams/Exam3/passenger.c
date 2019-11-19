/*
* Sergio Mercado A01020382
* Exam 3 : Passenger
* 19/11/2019
*/
#include "header.h"

void passenger(char * program){
  int semid;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 8, 0666)) < 0){
    perror(program);
    exit(-1);
  }

  int brds = 0;
  int unbrds = 0;

  while(1){
    sleep(1);

    printf("Passenger %i waiting to board...\n", getpid());

    mutex_wait(semid, BOARDQUEUE);

    mutex_wait(semid, MUTEX1);

      printf("Passenger %i now boarding...\n",getpid());

      sem_signal(semid, BOARDERS, 1);
      brds = semctl(semid, BOARDERS, GETVAL, 0);

      if(brds == MAX){
        mutex_signal(semid, ALLABOARD);
        semctl(semid, BOARDERS, SETVAL, 0);
      }

    mutex_signal(semid, MUTEX1);

    mutex_wait(semid, UNBOARDQUEUE);

    mutex_wait(semid, MUTEX2);

      printf("Passenger %i now unboarding...\n", getpid());

      sem_signal(semid, UNBOARDERS, 1);
      unbrds = semctl(semid, UNBOARDERS, GETVAL, 0);

      if(unbrds == MAX){
        printf("%i", unbrds);
        mutex_signal(semid, ALLASHORE);
        semctl(semid, UNBOARDERS, SETVAL, 0);
      }

    mutex_signal(semid, MUTEX2);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int i, people, pid;

  if(argc != 2){
    printf("usage: %s [number_of_people]\n", argv[0]);
    return -1;
  }

  people = atoi(argv[1]);

  for(i = 0; i < people; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      passenger(argv[0]);
    }else{
      //donothing
    }
  }

  return 0;
}
