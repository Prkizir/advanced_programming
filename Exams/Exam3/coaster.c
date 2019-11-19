/*
* Sergio Mercado A01020382
* Exam 3 : Coaster [resource creation]
* 19/11/2019
*/

#include "header.h"

int main(int argc, char *argv[]) {
  int semid, i;
  unsigned short final_values[8];
  key_t key;

  printf("%s: Creating the resources\n", argv[0]);

  if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

  if ( (semid = semget(key, 8, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

  semctl(semid, MUTEX1, SETVAL, 1);
  semctl(semid, MUTEX2, SETVAL, 1);
  semctl(semid, BOARDERS, SETVAL, 0);
  semctl(semid, UNBOARDERS, SETVAL, 0);
  semctl(semid, BOARDQUEUE, SETVAL, 0);
  semctl(semid, UNBOARDQUEUE, SETVAL, 0);
  semctl(semid, ALLABOARD, SETVAL, 0);
  semctl(semid, ALLASHORE, SETVAL, 0);

  semctl(semid, 0, GETALL, final_values);
  printf("values: \n");
  for(i = 0; i < 8; i++){
    printf("%3i\n", final_values[i]);
  }

  return 0;
}
