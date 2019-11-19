#include "header.h"

int main(int argc, char *argv[]) {
  int semid, i;
  unsigned short final_values[6];
  key_t key;

  printf("%s: Creating the resources\n", argv[0]);

  if((key = ftok("/dev/null", 65)) == (key_t) - 1){
    perror(argv[0]);
    return -1;
  }

  if((semid = semget(key, 6, 0666 | IPC_CREAT)) < 0){
    perror(argv[0]);
    return -1;
  }

  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, CUSTOMER, SETVAL, 0);
  semctl(semid, BARBER, SETVAL, 0);
  semctl(semid, CUSTOMER_DONE, SETVAL, 0);
  semctl(semid, BARBER_DONE, SETVAL, 0);
  semctl(semid, CUSTOMERS, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: \n");
  for(i = 0; i < 6; i++){
    printf("%3i\n", final_values[i]);
  }

  return 0;
}
