#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	int i, j, max;
	FILE * fileA, *fileB;
  	float num;
  	
    if (argc != 3) {
        printf("usage: %s matrix_file_a matrix_file_b\n", argv[0]);
        return -1;
    }
    
    fileA = fopen (argv[1],"w");
    if (fileA == NULL) {
		perror("fopen");
		return -1;
	}
	
	fileB = fopen (argv[2],"w");
    if (fileB == NULL) {
		perror("fopen");
		return -1;
	}
    
    srand(time(0));
    max = (rand() % 201) + 1100;
    
    fprintf(fileA, "%i\n", max);
    for (i = 0; i < max; i++) {
    	for (j = 0; j < max; j++) {
    		num = ((float)rand() / (float)(RAND_MAX)) * 100;
    		fprintf(fileA, "%.3f\t", num);
    	}
    	fprintf(fileA, "\n");
    }
    fprintf(fileA, "\n");
    fclose(fileA);
    
	fprintf(fileB, "%i\n", max);
    for (i = 0; i < max; i++) {
    	for (j = 0; j < max; j++) {
    		num = ((float)rand() / (float)(RAND_MAX)) * 100;
    		fprintf(fileB, "%.3f\t", num);
    	}
    	fprintf(fileB, "\n");
    }
    fprintf(fileB, "\n");
    fclose(fileB);
    
	return 0;
}

