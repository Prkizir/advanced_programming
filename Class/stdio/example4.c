#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

int main(int argc, char* argv[]){
    int txt_file, img_file;

    if(argc != 3){
        fprintf(stderr, "usage: %s txt_file img_file\n", argv[0]);
        return -1;
    }

    if((txt_file = open(argv[1], O_RDONLY)) < 0){
        perror(argv[0]);
        return -2;
    }

    if((img_file = open(argv[2], O_RDWR)) < 0){
        perror(argv[0]);
        return -3;
    }

    uint width, height;
    lseek(img_file,18,SEEK_SET);
    read(img_file, &width, sizeof(uint));
    read(img_file, &height, sizeof(uint));
    printf("w: %u - h: %u\n", width/1024, height);

    ulong txt_size = lseek(txt_file, 0, SEEK_END);
    printf("txt: %lu", txt_size);

    if(txt_size > (width * height)){
        printf("%s: The size of the text must be less than the number of pixels\n", argv[0]);
        return -4;
    }

    close(txt_file);
    close(img_file);

    return 0;
}