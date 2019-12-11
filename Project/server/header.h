/*
* Sergio Isaac Mercado Silvano
* A01020382
* Advanced Programming: Final Project -> Header
* 03/12/2019
*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <pthread.h>

typedef unsigned char uchar;
typedef unsigned long ulong;

#define MUTEX 0

#define HELLO 1

#define SEND  101
#define LS    102
#define ENDCONN  103

#define DENIED       201
#define FILENOTFOUND 202
#define INTERR       203
#define CMDUNK       204
#define ISDIR        205
#define DIRNOTFOUND  206
#define NOTDIR       207
#define SNDFILE      301
#define SNDDIR       302

#define MSG_HELLO           "Welcome"
#define MSG_DENIED          "Denied access"
#define MSG_FILENOTFOUND    "File not found"
#define MSG_INTERR          "Internal error"
#define MSG_CMDUNK          "Unknown command"
#define MSG_ISDIR           "The route is a directory"
#define MSG_DIRNOTFOUND     "Directory not found"
#define MSG_NOTDIR          "The route is not a directory"

#define MSG_SEND "File sent"
#define MSG_LS "Directory contents sent"

int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif
