#include "header.h"

int main(int argc, char *argv[]) {
  int semid, i;
  unsigned short final_values[4];
  key_t key;

  printf("%s: Creating the resources\n", argv[0]);

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(argv[0]);
    return -1;
  }

  if((semid = semget(key, 4, 0666 | IPC_CREAT)) < 0){
    perror(argv[0]);
    return -1;
  }

  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, EMPTY, SETVAL, 0);
  semctl(semid, FULL, SETVAL, 0);
  semctl(semid, SERVINGS, SETVAL,0);

  semctl(semid, 0, GETALL, final_values);
  printf("values:\n");
  for(i = 0; i < 4; i++){
    printf("%3i\n", final_values[i]);
  }

  return 0;
}
