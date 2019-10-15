/*
* Sergio Mercado
* A01020382
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int n;

  if(argc < 3){
    printf("usage: %s [PID] [pipe_name]\n",argv[0]);
    return -1;
  }

  if(atoi(argv[1]) < 1){
    printf("PID must be a positive integer greater than 1\n");
    return -3;
  }

  srand(1234);
  n = rand() % 100;

  printf("Generating %i numbers\n", n);

  int nums[n];
  int i;

  for(i = 0; i < n; i++){
    nums[i] = (rand() % 10000);
  }

  for(i = 0; i < n; i++){
    printf("%i ",nums[i]);
  }
  printf("\n");

  printf("Sending SIGUSR1 signal to [%s]\n", argv[1]);
  kill(atoi(argv[1]),SIGUSR1);

  int fd = open(argv[2],O_WRONLY);

  write(fd,&n,sizeof(int));
  write(fd,nums,n*sizeof(int));
  close(fd);



  return 0;
}
