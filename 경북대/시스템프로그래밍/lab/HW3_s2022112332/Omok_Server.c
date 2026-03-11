#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg) ; exit(1);}
#define WHITE 2
#define BLACK 1
#define MAX_N 15

typedef struct node* LINK;
typedef struct node {
	char userid[10];
	int win;
	LINK next;
}NODE;

int checkFinish(int y, int x);
int drawStone(int y, int x, int stone);
int board[MAX_N][MAX_N] = { 0 };
void writemessage(FILE* sock_fp, char* message, int sock_fd);
void readmessage(FILE* sock_fp, char* buff, int sock_fd);
void initializeBuff(char* buff, int len);
void record(char* userid);
LINK createNode(char* userid, int win);
void freeNode(LINK head);

int main(int ac, char* av[]){
	signal(SIGINT, SIG_IGN);
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, isFin = 0;
	int sock_fd[2];
	char message[10];
	char buffer[10];
	char turn[10];
	char userid[2][20];

	FILE* sock1_in, *sock2_in, *sock1_out, *sock2_out;

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
		for (int i = 0; i < 2; i++) {
			sock_fd[i] = accept(sock_id, NULL, NULL);
			if (sock_fd[i] == -1)
				oops("accept");
			printf("player %d connected\n", i + 1);
		}
		
		if ((sock1_out = fdopen(sock_fd[0], "w")) == NULL)
			oops("fdopen writing");
		if ((sock2_out = fdopen(sock_fd[1], "w")) == NULL)
			oops("fdopen writing");
		if ((sock1_in = fdopen(sock_fd[0], "r")) == NULL)
			oops("fdopen reading");
		if ((sock2_in = fdopen(sock_fd[1], "r")) == NULL)
			oops("fdopen reading");
		isFin = 0;
		for (int i = 0; i < MAX_N; i++) {
			for (int j = 0; j < MAX_N; j++) {
				board[i][j] = 0;
			}
		}
		readmessage(sock1_in, userid[0], sock_fd[0]);
		readmessage(sock2_in, userid[1], sock_fd[1]);
		printf("Game start player1 %s, player2 %s\n",userid[0], userid[1]);

		writemessage(sock1_out, "1",sock_fd[0]);
		writemessage(sock2_out, "2",sock_fd[1]);
		int i = 0;
		while (1) {
			
			initializeBuff(message, 10);
			initializeBuff(buffer, 10);

			sprintf(turn, "%c%c", isFin + '0', i % 2 + 1 + '0');
			writemessage(sock1_out, turn, sock_fd[0]);
			writemessage(sock2_out, turn, sock_fd[1]);

			if (i % 2 == 0) {//BLACK turn
				readmessage(sock1_in, buffer,sock_fd[0]);
				drawStone(buffer[1]-'0', buffer[0]-'0', i % 2 + 1);
				isFin = checkFinish(buffer[0] - '0', buffer[1] - '0');
				sprintf(message, "%c%c%c%c", isFin+'0', (i % 2) + 1+'0', buffer[0], buffer[1]);
				writemessage(sock2_out, message,sock_fd[1]);
			}
			else {//WHITE turn
				readmessage(sock2_in, buffer,sock_fd[1]);
				drawStone(buffer[1]-'0', buffer[0]-'0', i % 2 + 1);
				isFin = checkFinish(buffer[0] - '0', buffer[1] - '0');
				sprintf(message, "%c%c%c%c", isFin+'0', (i % 2) + 1+'0', buffer[0], buffer[1]);
				writemessage(sock1_out, message,sock_fd[0]);
			}
			if (isFin)
				break;
			i++;
		}
		sprintf(message, "%c%c", isFin + '0', i % 2 + 1 + '0');
		writemessage(sock1_out, message,sock_fd[0]);
		writemessage(sock2_out, message,sock_fd[1]);
		printf("player%d WON!\n\n\n", isFin);

		record(userid[isFin-1]);

		fclose(sock1_in);
		fclose(sock2_in);
		fclose(sock1_out);
		fclose(sock2_out);
		close(sock_fd[0]);
		close(sock_fd[1]);
	}
}

LINK createNode(char* userid, int win) {
	LINK cur = (NODE*)malloc(sizeof(NODE));
	cur->win = win;
	strcpy(cur->userid, userid);
}

LINK appendNode(LINK head, LINK cur) {
	LINK nextnode = head;
	if (!head) {
		head = cur;
		return head;
	}
	while (nextnode->next) {
		nextnode = nextnode->next;
	}
	nextnode->next = cur;
	return head;
}

int checklist(LINK head, char* uid) {
	LINK nextnode = head;
	while (nextnode) {
		if (!strcmp(nextnode->userid, uid)) {
			nextnode->win += 1;
			return 1;
		}
		nextnode = nextnode->next;
	}
	return 0;

}
void record(char* uid) {
	LINK head = NULL, cur;
	char temp[20];
	int win;
	char userid[10];
	int fd = open("record.txt", O_RDONLY, S_IRUSR | S_IWUSR);
	FILE* fp = fdopen(fd, "r");
	if (fp != NULL) {
		while (!feof(fp)) {
			initializeBuff(userid, 10);
			fscanf(fp, "%s%d", userid, &win);
			if (userid[0] != '\0') {
				cur = createNode(userid, win);
				head = appendNode(head, cur);
				printf("%s%d\n", userid, win);
			}

		}
		
		fclose(fp);
	}
	close(fd);
	if (!checklist(head, uid)) {
		cur = createNode(uid, 1);
		head = appendNode(head, cur);
	}

	fd = open("record.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	fp = fdopen(fd, "w");
	if (fp == NULL)
		oops("fpwrite");
	cur = head;
	while (cur) {
		fprintf(fp, "%s %d\n", cur->userid, cur->win);
		cur = cur->next;
	}

	freeNode(head);
	fclose(fp);
	close(fd);
}

void freeNode(LINK head) {
	LINK cur = head, nextnode = head->next;

	while (nextnode) {
		free(cur);
		cur = nextnode;
		nextnode = nextnode->next;
	}
	free(cur);

	printf("complete free\n");
}
void writemessage(FILE* sock_fp, char* message,int sock_fd) {
	fprintf(sock_fp, "%s", message);
	fflush(sock_fp);
	sleep(0.7);
}

void initializeBuff(char* buff, int len) {
	for (int i = 0; i < len; i++)
		buff[i] = '\0';
}
void readmessage(FILE* sock_fp, char* buff, int sock_fd) {
	fread(buff, sizeof(char), 10, sock_fp);
	fflush(sock_fp);
}
int checkFinish(int x, int y) {//0: 1: Black win, 2: White win
	int cur[2];
	int dir[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
	int updown = 0, rightleft = 0, crossL = 0, crossR = 0;
	for (int d = 0; d < 8; d++) {
		cur[0] = x; cur[1] = y;
		while (board[cur[1]][cur[0]] == board[y][x]) {
			cur[0] += dir[d][0]; cur[1] += dir[d][1];
			if (d == 0 || d == 4) {
				updown += 1;
			}
			else if (d == 1 || d == 5) {
				crossR += 1;
			}
			else if (d == 2 || d == 6) {
				rightleft += 1;
			}
			else if (d == 3 || d == 7) {
				crossL += 1;
			}
		}
	}
	if (updown > 5 || crossR > 5 || rightleft > 5 || crossL > 5)
		return board[y][x];
	return 0;
}

int drawStone(int y, int x, int stone) { // stone이 1이면 흑, 2면 백
	if (board[y][x] || x < 0 || x >= MAX_N || y < 0 || y >= MAX_N)
		return 0;
	if (stone == BLACK) {
		board[y][x] = BLACK;
	}
	else if (stone == WHITE) {
		board[y][x] = WHITE;
	}
	else
		return 0;
	return 1;
}