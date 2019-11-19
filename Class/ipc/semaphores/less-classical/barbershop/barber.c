#include "header.h"

int main(int argc, char *argv[]) {
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
    sleep(1);

    sem_wait(semid, CUSTOMER, 1);
    sem_signal(semid, BARBER, 1);

    printf("Barber is cutting a customer's hair\n");

    sem_wait(semid, CUSTOMER_DONE, 1);

    printf("Barber goes back to sleep\n");
    sem_signal(semid, BARBER_DONE, 1);
  }

  return 0;
}
