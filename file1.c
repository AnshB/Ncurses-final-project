/*Menu Page*/

#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#define MENUMAX 8

void drawmenu(int item) {
	int c;
	start_color();                        
	init_pair(1,COLOR_BLUE,COLOR_BLACK);     /*Blue text with black background*/
	char mainmenu[] = "Main Menu";
	char menu[MENUMAX][21] = {"Log in","Register","Find train","Check availaibility","Cancel ticket","Check status ac", "Check status sleeper", "Exit/Logout"};
	clear();
	attrset(A_BOLD);
	attron(COLOR_PAIR(1));          /*Initializing the colour pair and bold attribute*/
	addstr("Welcome to rail booking services");
	mvaddstr(1,0,"Main Menu");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));         /*End of bold and colour*/
	for(c = 0; c < MENUMAX; c++) {
		if( c == item )
			attron(A_REVERSE);
			/* highlight selection */
		mvaddstr(3 + (c * 2),20,menu[c]);
		attroff(A_REVERSE);
		/* remove highlight */
	}
	mvaddstr(17,25,"Use arrow keys to move; Right arrow key to select to select:");
	refresh();
}

int main() {
	int key, menuitem, ent, id;
	menuitem = 0;
	initscr();
	drawmenu(menuitem);
	keypad(stdscr,TRUE);
	
	noecho();
	do {
		key = getch();
		switch(key) {
			case KEY_DOWN: menuitem++;
			if(menuitem > MENUMAX-1) 
				menuitem = 0;
			break;

			case KEY_UP: menuitem--;
			if(menuitem < 0) 
				menuitem = MENUMAX-1;
			break;

			default: 
			break;

		}

		drawmenu(menuitem);
		
		ent = getch();
	
		if(menuitem == 0 && ent == KEY_RIGHT) { 
			login();
			refresh();
		}
		else if(menuitem == 1 && ent == KEY_RIGHT) {
			register_id();
			refresh();
		}
		else if(menuitem == 2 && ent == KEY_RIGHT) { 
			findtrain();
			refresh();
		}
		else if(menuitem == 3 && ent == KEY_RIGHT) { 
			printw("Enter train id\n");			
			scanw("%d", &id);
			CheckAvailibility(id);
			refresh();
		}	
		else if(menuitem == 4 && ent == KEY_RIGHT) { 
			cancelticket();
			refresh();
		}
		else if(menuitem == 5 && ent == KEY_RIGHT) { 
			DisplayQueueac();
			refresh();
		}
		else if(menuitem == 6 && ent == KEY_RIGHT) { 
			DisplayQueuesl();
			refresh();
		}
		else if(menuitem == 7 && ent == KEY_RIGHT) { 
			exit(1);
		}
	}	
	while(key != '\n');
		echo();      /*Shows the text*/
	
	endwin();
	return 0;

}

