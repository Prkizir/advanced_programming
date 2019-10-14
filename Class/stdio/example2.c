#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

//#define SIZE 4096

typedef unsigned char uchar;
typedef unsigned long ulong;


int main(int argc, char * argv[]){
    int source, destination;

    if(argc != 3){
        printf("usage: %s source destination\n", argv[0]);
        return -1;
    }

    if((source = open(argv[1],O_RDONLY))<0){
        perror(argv[0]);
        return -2;
    }

    if((destination = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
        perror(argv[0]);
        return -3;
    }

    int bytes;
    uchar* buffer;
    //uchar buffer[4096];
    ulong size = lseek(source, 0, SEEK_END);
    buffer = (uchar *) malloc(sizeof(uchar)*size);

    /*while(read(source, buffer, sizeof(uchar)* SIZE) != 0){
        write(destination, buffer, bytes);
    }*/

    lseek(source, 0, SEEK_SET);
    read(source, buffer, size);
    write(destination, buffer, size);

    free(buffer);
    close(source);
    close(destination);

    return 0;
}