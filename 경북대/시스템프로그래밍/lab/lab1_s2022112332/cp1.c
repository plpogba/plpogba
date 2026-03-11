#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char*, char*);

int main(int ac, char *av[]){
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];
	if (ac != 3){
		fprintf(stderr, "usage:\n");
		exit(1);
	}
	if ((in_fd = open(av[1], O_RDONLY)) == -1){
		printf("cannot open %s\n",av[1]);
		exit(1);
	}
	if ((out_fd = creat(av[2], O_WRONLY)) == -1){
		printf("cannot create %s\n",av[2]);
		exit(10);
	}

	while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, n_chars) != n_chars){
			printf("cannot write\n");
			exit(1);
		}
	if (close(in_fd) == -1 || close(out_fd == -1)){ 
		printf("cannot close\n");
		exit(1);
	}
	return 0;

}
