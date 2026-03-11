#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define LINELEN 512

void do_more(FILE *, int);
int see_more(int);
int main(int ac, char *av[]){
	FILE *fp;
	struct winsize wbuf;
	int pagelen = 0;
	if (ioctl(0,TIOCGWINSZ, &wbuf)!= -1)
		pagelen = wbuf.ws_row;

	if (ac == 1)
		do_more(stdin,pagelen);
	else
		while(--ac){
			if ((fp = fopen(*++av, "r"))!=NULL){
				do_more(fp,pagelen);
				fclose(fp);
			}
			else
				exit(1);
		}	
	return 0;
}

void do_more(FILE*fp, int PAGELEN){
	char line[LINELEN];
	int num_of_lines = 0;
	int see_more(), reply;

	while(fgets(line,LINELEN, fp)){
		if(num_of_lines == PAGELEN){
			reply = see_more(PAGELEN);
			if (reply == 0)
				break;
			num_of_lines = reply;
		}
		if (fputs(line,stdout ) == EOF)
			exit(1);
		num_of_lines++;
	}

}

int see_more(int PAGELEN){
	int c;
	printf("\033[7m more? \033[m");
	while((c=getchar()) != EOF){
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGELEN;
		if ( c == '\n')
			return 1;

	}
	return 0;
}
