#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

typedef struct arg_set{
	char *fname;
	int count;
}arg_s;

int main(int ac, char* av[]){
	pthread_t* threadp;
	arg_s* argp;
	void *count_words(void*);
	int total_words = 0;
	if(ac < 2){
		printf("usage: %s file1 file2 ...\n", av[0]);
		exit(1);
	}

	threadp = (pthread_t*)malloc(sizeof(pthread_t)*(ac-1));
	argp = (arg_s*)malloc(sizeof(arg_s)*(ac-1));

	for(int i = 0; i < ac-1; i++){
		argp[i].fname = av[i+1];
		argp[i].count = 0;
		pthread_create(&threadp[i],NULL, count_words,(void *)&argp[i]);
	}

	for(int i = 0; i < ac-1; i++){
		pthread_join(threadp[i], NULL);
	}
	
	for(int i = 0; i< ac-1; i++){
		printf("%5d: %s\n", argp[i].count, av[i+1]);
		total_words += argp[i].count;
	}
	printf("%5d: total words\n", total_words);
	
	free(argp);
	free(threadp);
	return 0;
}

void *count_words(void *a){
	struct arg_set *args = a;
	FILE *fp;
	int c, prevc = '\0';

	if((fp = fopen(args->fname, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(!isalnum(c) && isalnum(prevc)){
				args->count++;
			}
			prevc = c;
		}
		fclose(fp);
	}
	else 
		perror(args->fname);
	return NULL;
}
