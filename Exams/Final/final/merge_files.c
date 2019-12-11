/*
* Sergio Isaac Mercado Silvano
* A01020382
* Examen Filan 03/12/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#define SIZE 4096

typedef  unsigned char uchar;
typedef unsigned long ulong;

int current_level = 0;

void read_file(char * filename, char * output, char * directory){
  int in_fd, out_fd, check;
  int bytes;
  struct stat info;
  uchar buffer[SIZE];
  char filen[PATH_MAX + NAME_MAX + 1];

  sprintf(filen,"%s/%s", directory, filename);
  lstat(filen,&info);

  if((info.st_mode & S_IFMT) == S_IFDIR){

  }else{
    if((in_fd = open(filen, O_RDONLY)) < 0){
      perror(filen);
      exit(-3);
    }

    if((out_fd = open(output, O_WRONLY, 0666)) < 0){
      perror(output);
      exit(-4);
    }

    if((check = access(filen, R_OK)) != 0){
      printf("%s: could not read/access file\n", filename);
    }

    printf("Working on %s\n",filen);

    ulong size = lseek(out_fd,0,SEEK_END);
    lseek(out_fd,size,SEEK_SET);

    while((bytes = read(in_fd, buffer, sizeof(uchar) * SIZE)) != 0){
      write(out_fd, buffer, bytes);
    }

    close(in_fd);
    close(out_fd);
  }

}

void merge(char * program, char * directory, char * output, int max){
  DIR * dir;
  struct dirent* direntry;
  struct stat info;
  char path[PATH_MAX + NAME_MAX + 1];

  if((dir = opendir(directory)) == NULL){
    perror(directory);
    exit(-2);
  }

  printf("Working on %s\n", directory);

  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name,".")  != 0 &&
       strcmp(direntry->d_name,"..") != 0){

         read_file(direntry->d_name,output,directory);
       }
  }

  current_level++;
  if(current_level == max){
    exit(0);
  }

  rewinddir(dir);
  while((direntry = readdir(dir)) != NULL){

    if(strcmp(direntry->d_name,".")  != 0 &&
       strcmp(direntry->d_name,"..") != 0){

      sprintf(path, "%s/%s",directory,direntry->d_name);
      lstat(path,&info);


      if((info.st_mode & S_IFMT) == S_IFDIR){
        merge(program, path, output, max);
      }
    }
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int max_level, fd;

  if(argc != 4){
    printf("Usage: %s [directory] [output_filename] [max_level]\n", argv[0]);
    return -1;
  }

  max_level = atoi(argv[3]);

  if(max_level <= 0){
    printf("%s: the max level must be a positive integer greater than 0", argv[0]);
    return -1;
  }

  if((fd = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0){
    perror(argv[2]);
    return -1;
  }

  merge(argv[0], argv[1], argv[2], max_level);

  return 0;
}
