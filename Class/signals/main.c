#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <signal.h>

int main(int argc, char const *argv[]) {
  int fd, number_of_processes, block, *data, num, i, pid;
  struct stat info;

  if(argc != 3){
    fprintf(stderr, "usage: %s [number_of_processes] [input_file]\n",argv[0]);
    return -1;
  }

  number_of_processes = atoi(argv[1]);
  if(number_of_processes < 2){
    fprintf(stderr, "%s: the number of processes must be greater than 1\n",argv[0]);
    return -1;
  }

  if(stat(argv[2],&info) < 0){
    perror("stat");
    return -3;
  }

  if((fd = open(".inter",O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
    perror("open");
    return -4;
  }

  close(fd);

  block = info.st_size/(number_of_processes * sizeof(int));

  return 0;
}
