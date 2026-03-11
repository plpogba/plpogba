#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"
#include "varlib.h"
#define DFL_PROMPT "> "

int main(){
	char *cmdline, *prompt, **arglist;
	int result, process(char **);
	void setup();
	char * tmp ;
	prompt = DFL_PROMPT;
	setup();

	while((cmdline = next_cmd(prompt, stdin)) != NULL){
		if(strcmp(cmdline,"exit") == 0){
			printf("exit\n");
			exit(0);
		}
		tmp = strtok(cmdline, ";");
		while(tmp != NULL){
			if((arglist = splitline(tmp)) != NULL){
				result = process(arglist);
				freelist(arglist);
			}
			tmp = strtok(NULL,";");
		}
		free(cmdline);
	}
	return 0;
}

void setup(){
	extern char **environ;
	VLenviron2table(environ);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

}
void fatal(char *s1, char *s2, int n){
	fprintf(stderr, "Error: %s, %s\n",s1,s2);
	exit(n);
}
