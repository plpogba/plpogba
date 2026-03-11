#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <sys/types.h>

#define PORTNUM 13000
#define MAX_N 15
#define DRAW_BLACK addch('O')
#define DRAW_WHITE addch('X')
#define oops(msg) {perror(msg); exit(1);}
#define WHITE 2
#define BLACK 1

void readmessage(int , char*);
void inputCode(int* y, int* x);
int drawStone(int y, int x, int stone);
void drawInit();
void gotoxy(int x, int y);
void gotoInput();
int board[MAX_N][MAX_N] = { 0 };

int main(int ac, char *av[]){
	struct sockaddr_in servadd;
	struct hostent *hp;
	int utmpfd;
	int sock_id, sock_fd;
	char message[BUFSIZ], buffer[10];
	int messlen;
	int isFin = 0;//0: game running, 1: Black win, 2: White win
	int BW;
	int x, y, i = 1;
	int c;
	char** temp;
	int uid;
	
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_id == -1)
		oops("socket");

	bzero(&servadd, sizeof(servadd));

	hp = gethostbyname(av[1]);
	if(hp == NULL)
		oops(av[1]);
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM);
	servadd.sin_family = AF_INET;

	if(connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
		oops("connect");

	uid = getuid();
	sprintf(message, "%d", uid);
	write(sock_id, message, 10);
	initscr();
	clear();
	move(0, 0);

	drawInit();

	refresh();
	messlen = 0;
	while (messlen == 0)
		messlen = read(sock_id, message, BUFSIZ);
	BW = message[0] - '0'; //Server sends whether Black or White
	while (1) {
		messlen = 0;
		while (messlen == 0)
			messlen = read(sock_id, message, BUFSIZ); //message: isFin, white/black, x,y
		isFin = message[0] - '0';
		if (isFin && isFin != BW) {
			drawStone(message[2] - '0', message[3] - '0', BW % 2 + 1);
			break;
		}
		else if (isFin)
			break;
		if (message[1] - '0' == BW) {
			{
				int draw = 0;
				while (!draw) {
					inputCode(&y, &x);
					draw = drawStone(y, x, BW);
				}
				char s[10];
				s[0] = y + '0'; s[1] = x + '0';
				write(sock_id, s, 10);
				refresh();
			}

		}
		else {
			gotoInput();
			addstr("Oponent's turn\t\t");
			refresh();
			messlen = 0;
			while (messlen == 0)
				messlen = read(sock_id, message, BUFSIZ);
			drawStone(message[2] - '0', message[3] - '0', BW % 2 + 1);
		}

	}
	gotoInput();
	if (isFin == BW) {
		addstr("YOU WIN!! \t\t\t");
	}
	else {
		addstr("YOU LOST!! \t\t\t");
	}
	refresh();
	close(sock_id);
	getch();
	endwin();
	return 0;

}


void inputCode(int* y, int* x) {
	gotoInput();
	addstr("Input Coordinate (A19): ");
	char s[10];
	getnstr(s, 4);
	if (!strcmp(s, "exit")) {
		endwin();
		exit(1);
	}

	*x = s[0] - 'A';
	if (strlen(s) > 2)
		return;
	else if (strlen(s) == 2) {
		*y = s[1] - 'a';
	}
	else
		*y = *x = -1;
	gotoInput(); addstr("Input Coordinate (Gm): \t\t\t");
	return;
}

int drawStone(int y, int x, int stone) { // stone이 1이면 흑, 2면 백
	if (board[y][x] || x < 0 || x >= MAX_N || y < 0 || y >= MAX_N)
		return 0;
	gotoxy(x, y);
	if (stone == BLACK) {
		DRAW_BLACK;
		board[y][x] = BLACK;
	}
	else if (stone == WHITE) {
		DRAW_WHITE;
		board[y][x] = WHITE;
	}
	else
		return 0;
	refresh();
	return 1;
}

void drawInit() {
	gotoxy(0, 0);
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			gotoxy(i, j);
			addstr("+ ");
			if (j == MAX_N - 1) {
				gotoxy(i, j + 1);
				addch(i + 'A');
			}
		}

	}
	for (int i = 0; i < MAX_N; i++) {
		gotoxy(MAX_N, i);
		addch(i + 'a');
	}
	gotoInput();
	addstr("fin.");
}

void gotoxy(int x, int y) {
	move(x, y * 2);
}

void gotoInput() {
	move(20, 20);
}
