#include "header.h"

void customer(char * program){
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

    int num = semctl(semid, CUSTOMERS, GETVAL, 0);

    if(num == MAX){
      mutex_signal(semid, MUTEX);
      printf("Customer %i is balking\n", getpid());
    }

    sem_signal(semid, CUSTOMERS, 1);
    num = semctl(semid, CUSTOMERS, GETVAL, 0);

    mutex_signal(semid, MUTEX);

    sem_signal(semid, CUSTOMER, 1);
    sem_wait(semid, BARBER, 1);

    printf("Customer %i is getting a haircut\n", getpid());

    sem_signal(semid, CUSTOMER_DONE, 1);
    sem_wait(semid, BARBER_DONE, 1);

    mutex_wait(semid, MUTEX);

    printf("Customer %i is leaving the shop\n", getpid());
    sem_wait(semid, CUSTOMERS, 1);
    num = semctl(semid, CUSTOMERS, GETVAL, 0);

    mutex_signal(semid, MUTEX);

    sleep(1);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int i, n_customers, pid;

  if(argc != 2){
    printf("usage: %s [number_of_customers]\n", argv[0]);
    return -1;
  }

  n_customers = atoi(argv[1]);

  for(i = 0; i < n_customers; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid ==0){
      customer(argv[0]);
    }else{
      //donothing
    }
  }

  return 0;
}
