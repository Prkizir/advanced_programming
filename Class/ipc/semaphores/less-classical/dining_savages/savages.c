#include "header.h"

void savage(char * program){
  int semid, i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 4, 0666)) < 0){
    perror(program);
    exit(-1);
  }

  while(1){
    sleep(1);
    mutex_wait(semid,MUTEX);

    int servings = semctl(semid, SERVINGS, GETVAL);

    if(servings == 0){
      sem_signal(semid, EMPTY, 1);
      sem_wait(semid, FULL,1);
    }

    printf("Savage %i is getting a serving from the pot\n", getpid());
    sem_wait(semid, SERVINGS, 1);
    servings = semctl(semid, SERVINGS, GETVAL, 0);

    mutex_signal(semid,MUTEX);

    printf("Savage %i is eating the serving\n", getpid());
  }
}

int main(int argc, char *argv[]) {
  int sv, i, pid;

  if(argc != 2){
    printf("usage: %s [number_of_savages]\n", argv[0]);
    return -1;
  }

  sv = atoi(argv[1]);

  for(i = 0; i < sv; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      savage(argv[0]);
    }else{
      //donothing
    }
  }

  return 0;
}
