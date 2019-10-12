#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

void get_info(char *name, DIR *dir, char *program);
void list(char *dir_name, char* program);

int main(int argc, char* argv[]){
  char dir_name[PATH_MAX + 1];
  char *dir;

  if(argc != 2){
    fprintf(stderr, "usage: %s [directoy]\n", argv[0]);
    return -1;
  }

  getcwd(dir_name,PATH_MAX);
  dir = dir_name;

  list(dir, argv[0]);

  return 0;
}

void list(char *dir_name, char *program){
  char path[PATH_MAX + NAME_MAX + 1];
  DIR *dir;
  int filecnt = 0;
  struct dirent *direntry;
  struct stat info;

  if((dir = opendir(dir_name)) == NULL){
    perror(program);
    exit(-2);
  }

  printf("%s:\n",dir_name);
  rewinddir(dir);
  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name,".") != 0 &&
       strcmp(direntry->d_name,"..") !=0){
         sprintf(path, "%s/%s",dir_name,direntry->d_name);
         lstat(path, &info);
         /*Here we print the other info in each directory*/

         //get_info(direntry->d_name, dir, program);
         while((direntry = readdir(dir)) != NULL){
           if(direntry->d_type == DT_REG){
             filecnt++;
           }
         }

         printf("Number of files: %i\n",filecnt);

         if((info.st_mode & S_IFMT) == S_IFDIR){
           list(path, program);
         }
       }
  }

  printf("\n");
}

void get_info(char *name, DIR *dir, char *program){
int filecnt = 0;
struct dirent *direntry;

  while((direntry = readdir(dir)) != NULL){
    if(direntry->d_type == DT_REG){
      filecnt++;
    }
  }

  printf("%i\n",filecnt);
}
