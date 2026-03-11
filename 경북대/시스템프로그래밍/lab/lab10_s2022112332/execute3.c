#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"
#include <string.h>
int execute(char * argv[]){
	extern char **environ;
	int pid;
	int child_info = -1;
	int i, check = 0;
        for( i =0; argv[i]; i++);
	if(strcmp(argv[i-1],"&") == 0){
		check = 1;
		argv[i-1] = NULL;
	}
	if (argv[0] == NULL)
		return 0;

	if ((pid = fork()) == -1)
		perror("fork");

	else if (pid == 0){
		environ = VLtable2environ();
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else {
		if(check == 0)	
			if(wait(&child_info) == -1)
				perror("wait");
		else 
			wait(NULL);

	}
	return child_info;
}
