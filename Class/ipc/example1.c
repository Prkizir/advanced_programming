#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


void execute(char * order1, char * order2, char * order3, char * program){
  int p1p2[2], p2p3[2], pid;

  if(pipe(p1p2) < 0){
    perror("pipe12");
    exit(-1);
  }

  if((pid = fork()) < 0){
    perror("fork p2");
    exit(-2);
  }else if(pid == 0){
    if(pipe(p2p3) < 0){
      perror("pipe23");
      exit(-3);
    }

    if((pid = fork()) < 0){
      perror("fork p3");
      exit(-4);
    }else if(pid == 0){
      //P3 /Order 1
      close(1); //Cerrar stdout
      dup(p2p3[1]); //Redireccionar stdout
                   // de la tuberia 0 es in || 1 es out

      close(p2p3[0]);
      close(p2p3[1]);

      execlp(order1, order1, (char *)0);
    }else{
      // P2 /Order 2
      close(0);
      dup(p2p3[0]);
      close(p2p3[0]);
      close(p2p3[1]);

      close(1);
      dup(p1p2[1]);

      close(p1p2[0]);
      close(p1p2[1]);

      execlp(order2, order2, (char *)0);
    }
  }else{
    //P1 / Order 3
    close(0);
    dup(p1p2[0]);

    close(p1p2[0]);
    close(p1p2[1]);

    execlp(order3, order3, (char *)0);
  }
}

int main(int argc, char *argv[]) {
  execute("ls","sort","more",argv[0]);
  return 0;
}
