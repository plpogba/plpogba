#include <stdio.h>
#include <curses.h>
#include <unistd.h>
int main(){
	initscr();

	clear();
	
	for(int i =0 ;;i++){
	move(10,20);
	addstr("Hello world");
	move(LINES-1,0);
	refresh();
	sleep(1);
		
	move(10,20);
	addstr("               ");
	move(LINES-1,0);
	refresh();
	sleep(1);
	

	}
	endwin();
	
	return 0;
}
