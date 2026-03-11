#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void f(int);
int i =0;
int main(){
	time_t start;
	start = time(NULL);
	void f(int);
	signal(SIGINT,f);
	
	printf("you can't stop me!\n");

	while(1){
		sleep(1);
		printf("haha\n");
		i++;
	}

	return 0;
}

void f(int signum){
	printf("Currently elapsed time: %d sec(s)\n",i);
	
}
