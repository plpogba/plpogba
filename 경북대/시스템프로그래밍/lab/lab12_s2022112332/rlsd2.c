#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>
#include <strings.h>

#define PORTNUM 15000
#define HOSTLEN 256
#define oops(msg) {perror(msg) ; exit(1);}

void sanitize(char *);

int main(int ac, char* av[]){
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, sock_fd;
	FILE *sock_fpi, *sock_fpo;
	FILE *pipe_fp;
	char dirname[BUFSIZ-5];
	char command[BUFSIZ];
	char string[BUFSIZ];
	int dirlen, c, pid, thepipe[2];

	sock_id = socket(PF_INET, SOCK_STREAM,0);
	if(sock_id == -1)
		oops("socket");

	bzero((void*)&saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;

	if(bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
		oops("bind");

	if(listen(sock_id, 1) != 0)
		oops("listen");

	while(1){
		sock_fd = accept(sock_id, NULL, NULL);

		if (sock_fd == -1)
			oops("accept");
		if (pipe(thepipe) == -1)
			oops("pipe failed");
		if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
			oops("fdopen reading");

		if (fgets(dirname, BUFSIZ - 5, sock_fpi) == NULL)
			oops("reading dirname");
		sanitize(dirname);

		if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
			oops("fdopen writing");
		if ((pid = fork()) == -1)
			oops("caanot fork");
		if (pid == 0) {
			close(thepipe[0]);
			if (dup2(thepipe[1], 1) == -1)
				oops("could not redirect stdin");
			close(thepipe[1]);
			execlp("ls", "ls", dirname, NULL);
			oops("cannot run ls");
		}
		else {
			close(thepipe[1]);
			wait(NULL);
			while (1) {
				int read_len = read(thepipe[0], string, BUFSIZ);
				if (read_len <= 0)
					break;
				fprintf(sock_fpo, "%s", string);
			}
			fclose(sock_fpo);
			fclose(sock_fpi);
		}
		
	}
}

void sanitize(char *str){
	char *src, *dest;

	for(src = dest = str; *src; src++)
		if(*src == '/' || isalnum(*src))
			*dest++ = *src;
	*dest = '\0';
}
