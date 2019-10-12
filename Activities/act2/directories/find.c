/*
* Activity - Directories
* Author: Sergio Mercado A01020382
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void getinfo(char * path, char * file_name, char * program){
  struct stat filestat;
  struct stat info;
  time_t raw_time;
	struct tm *timeinfo;

  char date[50];

  raw_time = info.st_mtime;
	timeinfo = localtime( &raw_time );
	strftime(date, 50, "%c", timeinfo);

  printf("Name: %s\n", file_name);
  printf("Path: %s\n", path);
  printf("Last access: %s",ctime(&filestat.st_atime));
  printf("Last modification: %s \n", date);

}

void find(char * sub_str, char * dir_name , int recursive, char * program){
  DIR * dir;
  struct dirent* direntry;
  struct stat info;
  char path[PATH_MAX + NAME_MAX + 1];

  if((dir = opendir(dir_name)) == NULL){
    perror(dir_name);
    exit(-2);
  }

  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name,".")  != 0 &&
       strcmp(direntry->d_name,"..") != 0){
         if(strstr(direntry->d_name,sub_str) != NULL){
           getinfo(dir_name, direntry->d_name, program);
         }
       }
   }

   if(recursive == 1){
     rewinddir(dir);
     while((direntry = readdir(dir)) != NULL){
       if(strcmp(direntry->d_name,".")  != 0 &&
          strcmp(direntry->d_name,"..") != 0){
            sprintf(path, "%s/%s",dir_name,direntry->d_name);
            lstat(path,&info);
            if((info.st_mode & S_IFMT) == S_IFDIR){
              find(sub_str,path,recursive,program);
            }
          }
     }
   }

}

int main(int argc, char * argv[]){
  char dir_name[PATH_MAX + 1];
  char *dir;
  char *sub_str;
  int recursive = 0;


  if(argc > 4 || argc < 3){
    printf("Usage: string initial_directory [-r]\n");
    return -2;
  }

  if(argc == 4){
    if(strcmp(argv[3], "-r") != 0){
      printf("Usage: string initial_directory [-r]\n");
      return -3;
    }else{
      recursive = 1;
      sub_str = argv[1];
      dir = argv[2];
    }
  }else if(argc == 3){
    sub_str = argv[1];
    dir = argv[2];
  }

  find(sub_str, dir, recursive, argv[0]);
}
