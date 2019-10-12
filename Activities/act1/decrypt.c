/*
* Activity - Files
* Author: Sergio Mercado A01020382
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SIZE 4096

typedef unsigned char uchar;

int main(int argc, char * argv[]){
    int in_fd, out_fd;

    if (argc != 3){
        printf("usage: %s [source] [destination]\n", argv[0]);
        return -1;
    }

    if((in_fd = open(argv[1], O_RDONLY)) < 0){
        perror(argv[0]);
        return -2;
    }

    if((out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
        perror(argv[0]);
        return -3;
    }

    int bytes;
    uchar buffer[SIZE];

    while((bytes = read(in_fd,buffer,sizeof(uchar) * SIZE)) != 0){

        int i;
        uchar txt;
        for(i = 0; i < bytes; i++){
            buffer[i] = (buffer[i] << 4) | (buffer[i] >> (8 - 4));
        }

        write(out_fd, buffer, bytes);
    }

    close(in_fd);
    close(out_fd);

    return 0;
}
