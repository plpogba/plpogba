#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int ac, char* av[]){
	int pid;
	int fd;
	
	if(ac != 2){
		printf("Usage: sortfromfile filename\n");
		exit(1);
	}
	if((pid = fork()) == -1){
		perror("fork"); exit(1);
	}

	if(pid == 0){
		printf("About to run sort with input from %s\n",av[1]);
		close(0);
		if((fd = open(av[1],O_RDONLY)) == -1){
			perror(av[0]);
			exit(1);
		}
		execlp("sort", "sort", av[1], NULL);
	}
	if(pid != 0){
		wait(NULL);
		printf("Done running sort < %s\n", av[1]);
	}
	return 0;
}
