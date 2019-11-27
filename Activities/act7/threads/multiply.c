/*
* Sergio Mercado A01020382
* Advanced Programming
* Activity: Threads
* 27/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define SIZE 4096

typedef struct params{
  int upper, lower, size;
  float **a, **b, **c;
}Parsec;

void* task(void * param){
  int i, j, k;
  float acum = 0.0;

  Parsec *item = (Parsec*) param;

  for(i = item->lower; i < item->upper; i++){
    for(j = 0; j < item->size; j++){
      for(k = 0; k < item->size; k++){
        acum += item->a[i][k] * item->b[k][j];
      }
      item->c[i][j] = acum;
      acum = 0.0;
    }
  }
}

int main(int argc, char *argv[]) {
  int i, j, threads, size;
  float number;
  FILE *file_a, *file_b;

  float **matrix_a, **matrix_b, **matrix_c;

  if(argc != 7){
    printf("usage: %s -a [matrix_file] -b [matrix_file] -t [number_of_threads]\n", argv[0]);
    return -1;
  }

  if((file_a = fopen(argv[2],"r")) < 0){
    perror(argv[0]);
    return -2;
  }

  if((file_b = fopen(argv[4],"r")) < 0){
    perror(argv[0]);
    return -2;
  }

  fscanf(file_a,"%i", &size);

  matrix_a = (float **)malloc(size * sizeof(float*));
  for(i = 0; i < size; i++){
    matrix_a[i] = (float *)malloc(size * sizeof(float));
    for(j = 0; j < size; j++){
      fscanf(file_a,"%f", &number);
      matrix_a[i][j] = number;
    }
  }

  fscanf(file_b,"%i", &size);

  matrix_b = (float **)malloc(size * sizeof(float*));
  for(i = 0; i < size; i++){
    matrix_b[i] = (float *)malloc(size * sizeof(float));
    for(j = 0; j < size; j++){
      fscanf(file_b,"%f", &number);
      matrix_b[i][j] = number;
    }
  }

  matrix_c = (float **)malloc(size * sizeof(float*));
  for(i = 0; i < size; i++){
    matrix_c[i] = (float *)malloc(size * sizeof(float));
  }

  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      printf("%f ", matrix_a[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      printf("%f ", matrix_b[i][j]);
    }
    printf("\n");
  }

  threads = atoi(argv[6]);

  Parsec params[threads];
  pthread_t pthread_id[threads];
  for(i = 0; i  < threads; i++){
    params[i].a = matrix_a;
    params[i].b = matrix_b;
    params[i].c = matrix_c;

    params[i].size = size;

    params[i].lower = i*(size/threads);
    if(i == (threads-1)){
      params[i].upper = size;
    }else{
      params[i].upper = (i + 1)*(size/threads);
    }

    pthread_create(&pthread_id[i], NULL, task, (void *) &params[i]);
  }

  for(i = 0; i < threads; i++){
    pthread_join(pthread_id[i], 0);
  }

  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      printf("%f ", matrix_c[i][j]);
    }
    printf("\n");
  }

  free(matrix_a);
  free(matrix_b);
  free(matrix_c);

  return 0;
}
