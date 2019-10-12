/*
* Activity - Processes
* Author: Sergio Mercado A01020382
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int child_process(char* program, int nivel_actual, int nivel_max){
	int i, pid;

	sleep(1);

	for (i = 0; i < nivel_actual; i++){
		printf("  ");
	}

	printf("PPID = %i PID = %i NIVEL= %i\n", getppid(), getpid(), nivel_actual);

	if(nivel_actual == nivel_max){
		exit(0);
	}

	for(i = 0; i < nivel_actual + 1; i++){
		if((pid = fork()) < 0){
			perror(program);
			exit(-1);
		}else if(pid == 0){
			child_process(program, nivel_actual + 1, nivel_max);
		}else{
			wait(NULL);
		}
	}

	exit(0);
}

int main(char argc, char* argv[]){

int pid;

	if(argc != 2){
		fprintf(stderr,"usage: %s [level]\n",argv[0]);
		return -2;
	}

	if(atoi(argv[1]) <= 0){
		printf("%s : The parameter must be a positive integer greater than 0\n",argv[0]);
		return -3;
	}

	printf("PPID = %i PID = %i NIVEL= %i\n", getppid(), getpid(), 0);

	if((pid = fork()) < 0){
		perror(argv[0]);
		exit(-1);
	}else if(pid == 0){
		child_process(argv[0],1, atoi(argv[1]));
	}else{
		wait(NULL);
	}

	return 0;
}
