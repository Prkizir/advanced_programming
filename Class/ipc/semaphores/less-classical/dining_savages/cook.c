#include "header.h"

int main(int argc, char *argv[]) {
  int m;
  int semid, i;
  key_t key;

  if(argc != 2){
    printf("usage: %s [number_of_servings]\n", argv[0]);
    return -1;
  }

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(argv[0]);
    return -2;
  }

  if((semid = semget(key, 4, 0666)) < 0){
    perror(argv[0]);
    return -3;
  }

  m = atoi(argv[1]);

  while(1){
    printf("Cook is sleeping...\n");
    sem_wait(semid, EMPTY, 1);

    printf("Cook wakes up\n");
    sem_signal(semid, SERVINGS, m);

    printf("Cook is putting food in the pot...\n");
    sem_signal(semid, FULL, 1);

    sleep(1);
  }

  return 0;
}
