#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define BUFFERSIZE 4096
#define COPYMODE 0644


int dostat(char *);
void oops(char *, char *);
int isDir(int );
int show_file_info(char *, struct stat*);
char buffer[100];
int main(int ac, char *av[]){
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];
	if (ac != 3){
		fprintf(stderr,"usage: %s source destination\n",*av);
		exit(1);
	}
	
	if (strcmp(av[1],av[2])==0){
		fprintf(stderr,"%s and %s are the same Name!!!\n", av[1],av[2]);
		exit(1);
	}
	
	if ((in_fd=open(av[1],O_RDONLY)) == -1)
		oops("cannot open",av[1]);
	if (dostat(av[2]))
		sprintf(buffer,"%s%s",av[2],av[1]);
	else{
		strcpy(buffer,av[2]);
	}
	if ((out_fd=creat(buffer,COPYMODE)) == -1)
		oops("cannot creat",av[2]);

	while ((n_chars = read(in_fd,buf,BUFFERSIZE))>0)
		if (write(out_fd,buf, n_chars) != n_chars)
			oops("Write error to", av[2]);
	if ( n_chars == -1)
		oops("Read error from",av[1]);

	if (close(in_fd) == -1 || close(out_fd) == -1)
		oops("error closing files","");

	return 0;
}

void oops(char *s1, char *s2){
	fprintf(stderr,"Error: %s\n",s1);
	perror(s2);
	exit(1);
}

int dostat(char *filename){
	struct stat info;
	int k=0;
	stat(filename,&info);
	//if(stat(filename, &info) == -1)
	//	perror(filename);
	//else {
		k = show_file_info(filename,&info);
	//}
	return k;
}
int show_file_info(char *filename,struct stat *info_p){
	int a = isDir(info_p->st_mode);
	return a;
}

int isDir(int mode){
	if (S_ISDIR(mode))
		return 1;
	return 0;
}

