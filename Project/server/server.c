/*
* Sergio Isaac Mercado Silvano
* A01020382
* Advanced Programming: Final Project -> Server
* 03/12/2019
*/

#include "header.h"

void init(char * program){
  int semid;
  key_t key;
  unsigned short final_values[1];

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
    perror(program);
    exit(-1);
  }

  semctl(semid, MUTEX, SETVAL, 1);
  semctl(semid, 0, GETALL, final_values);
}

char * cat(char * str1, char * str2){
  char * out = malloc(strlen(str1) + strlen(str2) + 1);
  strcpy(out,str1);
  strcat(out,str2);

  return out;
}

/*void write_to_log(char * message, char * logfile){
  int fd;
  char dt[100];
  char * final;

  time_t t = time(NULL);
  struct tm time = *localtime(&t);

  if(access(logfile, F_OK) != -1){
    if((fd = open(logfile, O_WRONLY)) < 0){
      perror(logfile);
      exit(-1);
    }

    ulong size = lseek(fd, 0, SEEK_END);
    lseek(fd, size, SEEK_SET);

    sprintf(dt, "@ %d-%d-%d %d:%d:%d\n",time.tm_year + 1900,
                                        time.tm_mon + 1,
                                        time.tm_mday,
                                        time.tm_hour,
                                        time.tm_min,
                                        time.tm_sec);


    final = cat(message, dt);

    printf("%ld\n", sizeof(message));
    printf("%ld\n", sizeof(dt));
    printf("%s\n", message);
    printf("%s\n", dt);

    write(fd, final, sizeof(final));

    close(fd);
  }else{
    if((fd = open(logfile, O_WRONLY | O_CREAT, 0666)) < 0){
      perror(logfile);
      exit(-1);
    }
  }
}*/

void write_to_log(char * message, char * logfile){
  time_t t = time(NULL);
  struct tm time = *localtime(&t);
  FILE * fp;

  if(access(logfile, F_OK) != 1){
    fp = fopen(logfile,"a");
  }else{
    fp = fopen(logfile,"w+");
  }

  fseek(fp, 0, SEEK_END);
  fprintf(fp, "IP: %s @ %d-%d-%d %d:%d:%d\n",message,
                                             time.tm_year + 1900,
                                             time.tm_mon + 1,
                                             time.tm_mday,
                                             time.tm_hour,
                                             time.tm_min,
                                             time.tm_sec);

  fflush(fp);
  fclose(fp);
}

void handler(int nsfd, char * program, char * ip, char * root){
  int semid, check;
  key_t key;

  if((key = ftok("/dev/null", 65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 1, 0666)) < 0){
    perror(program);
    exit(-1);
  }

  char * log = "";
  char logfile[PATH_MAX + NAME_MAX + 1];

  sprintf(logfile, "%s/log.txt", root);
  log = cat(ip, " New connection established");

  mutex_wait(semid, MUTEX);
    write_to_log(log,logfile);
  mutex_signal(semid, MUTEX);

  char * message = MSG_HELLO;
  char * message_sent;

  char filename[PATH_MAX + NAME_MAX + 1];

  int code = HELLO;
  int code_sent;

  long length = strlen(message);
  long length_sent, data_read;

  write(nsfd, &code, sizeof(code));
  write(nsfd, &length, sizeof(length));
  write(nsfd, message, length * sizeof(char));

  long size;

  srand(getpid());

  do{
    message = "";
    log = "";
    size = 0;
    check = 0;

    read(nsfd, &code_sent, sizeof(code_sent));
    read(nsfd, &length_sent, sizeof(length_sent));

    message_sent = (char*)malloc(length_sent * sizeof(char));
    data_read = read(nsfd, message_sent, length_sent * sizeof(char));
    message_sent[length_sent] = '\0';

    struct stat info;

    if(code_sent == SEND){
      if(message_sent[0] != '/'){
        mutex_wait(semid, MUTEX);
          log = cat(ip, " Code: 101 Received -> Params: ");
          log = cat(log, message_sent);

          write_to_log(log, logfile);

          log = "";
          log = cat(ip, " Code: 203 Sent -> Internal Error:");

          write_to_log(log, logfile);
        mutex_signal(semid, MUTEX);

        code = INTERR;
        message = MSG_INTERR;
      }else{
        sprintf(filename, "%s%s", root, message_sent);

        int inf = lstat(filename, &info);
        if(inf < 0){
          mutex_wait(semid, MUTEX);
            log = cat(ip, " Code: 101 Received -> Params: ");
            log = cat(log, message_sent);

            write_to_log(log, logfile);

            log = "";
            log = cat(ip, " Code: 202 Sent -> File not found");

            write_to_log(log, logfile);
          mutex_signal(semid, MUTEX);

          code  = FILENOTFOUND;
          message = MSG_FILENOTFOUND;
        }else{
          if(S_ISDIR(info.st_mode)){
            mutex_wait(semid,MUTEX);
              log = cat(ip, "Code: 101 Received -> Params: ");
              log = cat(log, message_sent);

              write_to_log(log, logfile);

              log = cat(ip, " Code: 205 Sent -> Route is a directory");

              write_to_log(log, logfile);
            mutex_signal(semid,MUTEX);

            code = ISDIR;
            message = MSG_ISDIR;
          }else{
            if(access(filename, R_OK) == 0){
              FILE * fp;
              size = info.st_size;

              fp = fopen(filename, "r");


              mutex_wait(semid, MUTEX);
                write(nsfd, &message, sizeof(message));
                write(nsfd, &size, sizeof(size));

                char buffer[size];

                fread(buffer, 1, sizeof(buffer), fp);
                write(nsfd, buffer, sizeof(buffer));

                memset(buffer, 0, size);

                log = cat(ip, " Code: 101 Received -> Params: ");
                log = cat(log, message_sent);

                write_to_log(log, logfile);

                log = "";
                log = cat(ip, "Code: 301 Sent -> Sending file...");

                write_to_log(log, logfile);
              mutex_signal(semid, MUTEX);

              check = 1;
              code = SNDFILE;
              message = MSG_SEND;

              fclose(fp);
            }else{
              mutex_wait(semid, MUTEX);
                log = cat(ip, " Code: 101 Received -> Params: ");
                log = cat(log, message_sent);

                write_to_log(log, logfile);

                log = "";
                log = cat(ip, " Code: 201 Sent -> Access denied");

                write_to_log(log, logfile);
              mutex_signal(semid, MUTEX);

              code = DENIED;
              message = MSG_DENIED;
            }
          }
        }
      }
    } else if(code_sent == LS){
      if(message_sent[0] != '/'){
        mutex_wait(semid, MUTEX);
          log = cat(ip, " Code: 102 Received -> Params: ");
          log = cat(log, message_sent);

          write_to_log(log, logfile);

          log = "";
          log = cat(ip, " Code: 203 Sent -> Internal Error");

          write_to_log(log, logfile);
        mutex_signal(semid, MUTEX);

        code = INTERR;
        message = MSG_INTERR;
      }else{
        sprintf(filename, "%s%s", root, message_sent);

        if(lstat(filename, &info) != 0){
          mutex_wait(semid, MUTEX);
            log = cat(ip, " Code: 102 Received -> Params: ");
            log = cat(log, message_sent);

            write_to_log(log, logfile);

            log = "";
            log = cat(ip, " Code: 206 Sent -> Directory not found");

            write_to_log(log, logfile);
          mutex_signal(semid, MUTEX);

          code = DIRNOTFOUND;
          message = MSG_DIRNOTFOUND;
        }else{
          DIR * dir;

          if((dir = opendir(filename)) == NULL){
            mutex_wait(semid, MUTEX);
              log = cat(ip, " Code: 102 Received -> Params: ");
              log = cat(log, message_sent);

              write_to_log(log, logfile);

              log = "";
              log = cat(ip, " Code: 207 Sent -> Route is not a directory");

              write_to_log(log, logfile);
            mutex_signal(semid, MUTEX);
          }else{
            struct dirent* direntry;

            while((direntry = readdir(dir)) != NULL){
              if(strcmp(direntry->d_name, ".") != 0 &&
                 strcmp(direntry->d_name,"..") != 0){
                   message = cat(message, direntry -> d_name);
                   message = cat(message, "\n");
                 }
            }

            message = cat(message, "\0");
            length = strlen(message);

            write(nsfd, &code, sizeof(code));
            write(nsfd, &length, sizeof(length));
            write(nsfd, message, length * sizeof(char));

            mutex_wait(semid,MUTEX);
              log = cat(ip, " Code: 102 Received -> Params: ");
              log = cat(log, message_sent);

              write_to_log(log, logfile);

              log = "";
              log = cat(ip, " Code: 302 Sent -> Sending directory content...");
              log = cat(log, message);

              write_to_log(log, logfile);
            mutex_signal(semid,MUTEX);
          }

          check = 1;
          code = SNDDIR;
          printf("%i\n", SNDDIR);
          message = MSG_LS;
          closedir(dir);
        }
      }
    }else{
      if(code_sent == ENDCONN){
        printf("%s: connection terminated\n", ip);

        mutex_wait(semid, MUTEX);
          log = cat(ip, " Code: 103 Received -> Closing the connection...");

          write_to_log(log, logfile);
        mutex_signal(semid, MUTEX);
      }else{
        mutex_wait(semid, MUTEX);
          log = cat(ip, " Code: 102 Received -> Params: ");
          log = cat(log, message_sent);

          write_to_log(log, logfile);

          log = "";
          log = cat(ip, " Code: 204 Sent -> Unknown command received");

          write_to_log(log, logfile);
        mutex_signal(semid, MUTEX);

        code = CMDUNK;
        message = MSG_CMDUNK;
      }
    }

    length = strlen(message);

    if(check == 0){
      write(nsfd, &code, sizeof(code));
      write(nsfd, &length, sizeof(length));
      write(nsfd, message, length * sizeof(char));
    }

    free(message_sent);
    memset(filename,0, PATH_MAX + NAME_MAX + 1);
  }while(code_sent != ENDCONN);

  close(nsfd);
}

void server(char * program,char * ip, int port, char * root){
  int sfd, nsfd, pid;
  struct sockaddr_in server_info, client_info;

  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror(program);
    exit(-1);
  }

  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = inet_addr(ip);
  server_info.sin_port = htons(port);

  if(bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0){
    perror(program);
    exit(-1);
  }

  listen(sfd, 1);
  while(1){
    int len = sizeof(client_info);
    if((nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0){
      perror(program);
      exit(-1);
    }

    if((pid = fork()) < 0){
      perror(program);
    }else if(pid == 0){
      close(sfd);
      handler(nsfd, program, ip, root);
      exit(0);
    }else{
      close(nsfd);
    }

  }
}

int main(int argc, char *argv[]) {
  DIR * dir;
  char ip[15];
  int port;

  if(argc != 4){
    printf("usage: %s [ip_address] [port_number] [root_directory]\n", argv[0]);
    return -1;
  }

  strcpy(ip,argv[1]);
  port = atoi(argv[2]);

  if(port < 5000){
    printf("%s: Port must me greater or equal to 5000",argv[0]);
    return -1;
  }

  if((dir = opendir(argv[3])) == NULL){
    perror(argv[3]);
    return -1;
  }

  init(argv[0]);

  server(argv[0], ip, port, argv[3]);

  return 0;
}
