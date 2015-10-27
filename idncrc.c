#include<ncurses.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define size 5      /*normal seating + those alloted for waitlist*/
#define SizeCust 50

int count = 0, fraud = 0, lgn = 0, c2 = 0, noteac = -1, notesl = -1, looksl = 0, lookac = 0, csac = 0, cssl = 0;
char str_login[10];

typedef struct queue {
	int train_no;
	char uname[5][10];	
	int total;
}queue;

typedef struct store {
	int id;
	char uname[10];	
	char pass[10];
	int phone_no;
}store;

typedef struct storer {
	int id;
	char uname[10];	
	char pass[10];
	int phone_no;
}storer;

typedef struct tdata {
	int no;
	char name[50];
	char from[50];
	char to[50];
	char day[9];
}tdata;

typedef struct tseats {
	int no;
	int sleeperseats;
	int acseats;
}tseats;

store a[SizeCust];
storer b[SizeCust];
tdata c[SizeCust];
tseats d[SizeCust], f[SizeCust];
queue e[SizeCust], g[SizeCust], l[SizeCust], h[SizeCust];
int front = -1, rear = -1, so;



void checkUserName(char unm[]) {
	int i = 0;
	char um[10];
	while (i < SizeCust) {
			if(strcmp(unm, b[i].uname) == 0) {
				printw("Username already in use. Please enter a new username\n");
				scanw("%s", um);
				refresh();
				checkUserName(um);
			}	
			i++;	
	}
}	

int writeToCustomer() {
	int i = 0;
	FILE *f2 = fopen("cust.txt", "a+");
	if(f2 == NULL) {
		perror("open failed:");
		return errno;
	}
	fwrite(&a[i], sizeof(a[i]), 1, f2);
	fclose(f2);
	return 0;
	
}
	
int readCustomer() {
	FILE *f1 = fopen("cust.txt", "r");
	if(f1 == NULL) {
		perror("open failed:");
		return errno;
	}
	fread(&b, sizeof(b[0]), SizeCust, f1);
	fclose(f1);	
	return 0;
	
}

int CheckAvailibility(int id) {
	move(0,0);
	clear();
	int i, n, ct = 0;
	FILE *f4 = fopen("hav.txt", "r");
	if(f4 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&d, sizeof(d[0]), 45, f4);
		fclose(f4);

	for(i = 0; i < n; i++) {
		if(d[i].no == id) {
			printw("Sleeper:%d\tAC:%d\n", d[i].sleeperseats, d[i].acseats);
			refresh();
			ct++;
			break;
		}
		if(ct > 0) 
			return 0;
	}
		
}	
int findtrain() {
	move(0,0);
	clrtobot();
	if(lgn == 0) {
		printw("Please login and continue\n");
		refresh();
		login();
	}	
	int i, n, choice, book, ct = 0;
	char initial[20], destination[20], option;
	printw("Enter from location and destination\n");
	refresh();
	scanw("%s%s", initial, destination);
	
	FILE *f3 = fopen("usk.txt", "r");
	if(f3 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&c, sizeof(c[0]), 45, f3);
	fclose(f3);

	for(i = 0; i < n; i++) {
		if(strcmp(c[i].from, initial) == 0 && strcmp(c[i].to, destination) == 0) {
			move(4,0);
			printw("Available trains is/are:\t Train no:%d\tTrain name:%s\n", c[i].no, c[i].name);
			printw("Available seats:\n");
			refresh();
			ct++;
			CheckAvailibility(c[i].no);
			printw("Would you like to book?\n Press 1 to book in sleeper and 2 to book in ac\n");
			refresh();				
			scanw("%d", &book);
						
			switch(book) {
				case 1 : booktrainsleeper(c[i].no);
					 break;
				case 2 : booktrainac(c[i].no);	
					 break;				
				default : break;
			}
			printw("Do you want to book the ticket? Enter 'y' to print.\n");
			refresh();	
			scanw("%c", &option);
			if(option == 'y') {
				printticket(c[i].no, c[i].from, c[i].to, c[i].name, c[i].day);
			}
			
		}
		
	}
	if(ct == 0) {
			printw("No train available.Incorrect train no. Enter 1 to find another train or 2 to exit");
			scanw("%d", &choice);
			refresh();
	}
				
		
	switch(choice) {
		case 1 : findtrain();
			 break;
		case 2 : exit(1);
	
		default :
			  break;
	}
	
}
		
int booktrainsleeper(int id) {
	int i, choice; 	 
	for(i = 0; i < SizeCust; i++) {
		if(d[i].no == id) {
			if(d[i].sleeperseats > 0) {
				d[i].sleeperseats = d[i].sleeperseats - 1;
				printw("Ticket booked.\n");
				refresh();
				Enqueuesl(str_login, id);	
				 						 
			}

			else { 
				printw("Capacity full. Press 1 to put in waitlist, or 2 to book in ac, or 3 to exit\n");	
				scanw("%d", &choice);
				refresh();
				
				switch(choice) {
					case 1 : if(d[i].sleeperseats > (-2)) {
							 d[i].sleeperseats = d[i].sleeperseats - 1;
							 Enqueuesl(str_login, id);
						 }
						 else {
							printw("Waitlist list is full. Regret\n");
							refresh();
						 }						
						 break;
					case 2 : booktrainac(id); 
						 break;
					case 3 : exit(1);
					
					default : break;
				}
		
			}
		}
	}
	updateseats();
}

int booktrainac(int id) {
	int i, choice; 	 
	for(i = 0; i < SizeCust; i++) {
		if(d[i].no == id) {
			if(d[i].acseats > 0) {
				d[i].acseats = d[i].acseats - 1;
				printw("Ticket booked. Here are your ticket details\n");
				refresh();
				Enqueueac(str_login, id);
									 
			}

			else { 
				printw("Capacity full. Press 1 to put in waitlist, or 2 to book in sleeper, or 3 to exit\n");	
				scanw("%d", &choice);
				refresh();
				
				switch(choice) {
					case 1 : if(d[i].acseats > (-2)) {
  						 	d[i].acseats = d[i].acseats - 1;
							Enqueueac(str_login, id);
						 }	
						 else {
							printw("Waitlist list is full. Regret\n");
							refresh();
						 }	
						 break;
					case 2 : booktrainsleeper(id); 
						 break;
					case 3 : exit(1);
					
					default : break;
				}
			}
		}
	}
	updateseats();
}	

printticket(int id, char frm[], char t[], char n[], char d[]) {
	move(0,0);
	clrtobot();
	attrset(A_BOLD);
	mvaddstr(0,40, "Ticket");
	attroff(A_BOLD);
	move(5,40);
	printw("Username:%s", str_login);
	move(6,40);
	printw("Train id%d", id);
	move(7,40);
	printw("Train name%s", n);
	move(8,40);
	printw("From:%s", frm);
	move(9,40);
	printw("To:%s", t);
	move(10,40);
	printw("Day:%s", d);
	refresh();
}

int updateseats() {
	int i;	
	FILE *f6 = fopen("hav.txt", "w");
	if(f6 == NULL) {
		perror("open failed:");
		return errno;
	}
	for(i = 0; i < 45; i++) {
		fwrite(&d[i], sizeof(d[i]), 1, f6);
	}
	fclose(f6);
}	

 
int readqueuesl() {
	int n;
	FILE *f8 = fopen("qsl.txt", "r");
	if(f8 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&e, sizeof(e[0]), 45, f8);
	fclose(f8);	
	return n;
	
}
int readqueueac() {
	int n;
	FILE *f10 = fopen("qac.txt", "r");
	if(f10 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&h, sizeof(h[0]), 45, f10);
	fclose(f10);	
	return n;
	
}
int writequeuesl(int n) {
	int i = 0;
	FILE *f9 = fopen("qsl.txt", "w");
	if(f9 == NULL) {
		perror("open failed:");
		return errno;
	}
		for(i = 0; i <= 45; i++) {
			fwrite(&e[i], sizeof(e[i]), 1, f9);
		}
	fclose(f9);
	return 0;
	
}	
int writequeueac(int n) {
	int i = 0;
	FILE *f11 = fopen("qac.txt", "w");
	if(f11 == NULL) {
		perror("open failed:");
		return errno;
	}
	for(i = 0; i <= 45; i++) {
		fwrite(&l[i], sizeof(l[i]), 1, f11);
	}
	fclose(f11);
	return 0;
	
}	

int appendqueueac() {
	int i = 0;
	FILE *f12 = fopen("qac.txt", "a+");
	if(f12 == NULL) {
		perror("open failed:");
		return errno;
		refresh();
	} 
	fwrite(&l[0], sizeof(l[0]), 1, f12);
	fclose(f12);
	return 0;
}

int appendqueuesl() {
	int i = 0;
	FILE *f13 = fopen("qsl.txt", "a+");
	if(f13 == NULL) {
		perror("open failed:");
		refresh();
		return errno;
	} 
	fwrite(&g[cssl], sizeof(g[cssl]), 1, f13);
	fclose(f13);
	return 0;
}


int Enqueuesl(char login[], int no) {
	notesl = -1;	
	int n;
	n = readqueuesl();
	int i;
	for(i = 0; i < 50; i++) {
		if(e[i].train_no == no) {
			notesl = i;
			looksl++;
		}
	}
	//printw("value of note is:%d\t%d\n", notesl, i); 
	if(notesl != (-1)) {
		if(e[notesl].total > 4) {
			printw("Queue Full\n");
			refresh();
		}
		else if(e[notesl].total == 0) {	
			strcpy(e[notesl].uname[0], str_login);
			e[notesl].train_no = no;
			e[notesl].total++;
			
		}
		else if(e[notesl].total == 1) {	
			strcpy(e[notesl].uname[1], str_login);
			e[notesl].train_no = no;
			e[notesl].total++;
		}
		else if(e[notesl].total == 2) {	
			strcpy(e[notesl].uname[1], str_login);
			e[notesl].train_no = no;
			e[notesl].total++;
		}
		else if(e[notesl].total == 3) {	
			strcpy(e[notesl].uname[3], str_login);
			e[notesl].train_no = no;
			e[notesl].total++;
			
		}
		else if(e[notesl].total == 4) {	
			strcpy(e[notesl].uname[4], str_login);
			e[notesl].train_no = no;
			e[notesl].total++;
			
		}
		writequeuesl(n);
	}
	else if(notesl == -1) {
		//printw("Works\n");
		g[cssl].total = 0;
		if(g[cssl].total > 4) {
			printw("Queue Full\n");
		}
		else if(g[cssl].total == 0) {	
		//	printw("Hi\n");
			strcpy(g[cssl].uname[0], str_login);
			g[cssl].train_no = no;
			e[cssl].total++;
			g[cssl].total = e[cssl].total;
		}
		else if(g[cssl].total == 1) {	
			strcpy(g[cssl].uname[1], str_login);
			g[cssl].train_no = no;
			e[cssl].total++;
			g[cssl].total = e[cssl].total;
		}
		else if(g[cssl].total == 2) {	
			strcpy(g[cssl].uname[1], str_login);
			g[cssl].train_no = no;
			e[cssl].total++;
			g[cssl].total = e[cssl].total;
		}
		else if(g[cssl].total == 3) {	
			strcpy(g[cssl].uname[3], str_login);
			g[cssl].train_no = no;
			e[cssl].total++;
			g[cssl].total = e[cssl].total;
		}
		else if(g[cssl].total == 4) {	
			strcpy(g[cssl].uname[4], str_login);
			g[cssl].train_no = no;
			e[cssl].total++;
			g[cssl].total = e[cssl].total;
		}
		//printw("%d\t%s\t%d\n",g[cssl].train_no, g[cssl].uname[0], g[cssl].total);
		appendqueuesl();
		cssl++;
	}
		
	
	return 0;
}

int Enqueueac(char login[], int no) {
	noteac = -1;
	int n;
	n = readqueueac();
	int i;

	for(i = 0; i < SizeCust; i++) {
		if(h[i].train_no == no) {
			noteac = i;
			lookac++;
		}
	} 
	if(noteac != (-1)) {
		if(h[noteac].total > 4) {
			printw("Queue Full\n");
			refresh();
		}
		else if(h[noteac].total == 0) {	
			strcpy(l[noteac].uname[0], str_login);
			h[noteac].train_no = no;
			h[noteac].total++;
			
		}
		else if(h[noteac].total == 1) {	
			strcpy(l[noteac].uname[1], str_login);
			h[noteac].train_no = no;
			h[noteac].total++;
		}
		else if(h[noteac].total == 2) {	
			strcpy(l[noteac].uname[2], str_login);
			h[noteac].train_no = no;
			h[noteac].total++;
		}
		else if(h[noteac].total == 3) {	
			strcpy(l[noteac].uname[3], str_login);
			h[noteac].train_no = no;
			h[noteac].total++;
			
		}
		else if(h[noteac].total == 4) {	
			strcpy(l[noteac].uname[4], str_login);
			h[noteac].train_no = no;
			h[noteac].total++;
			
		}
		writequeueac(n);	
	}
	else if(noteac == -1) {
		l[csac].total = 0;
		if(l[csac].total > 4) {
			printw("Queue Full\n");
			refresh();
		}
		else if(l[csac].total == 0) {	
			strcpy(l[csac].uname[0], str_login);
			l[csac].train_no = no;
			h[csac].total++;
			l[csac].total = h[csac].total;
		}
		else if(l[csac].total == 1) {	
			strcpy(l[csac].uname[1], str_login);
			l[csac].train_no = no;
			h[csac].total++;
			l[csac].total = h[csac].total;
		}
		else if(l[csac].total == 2) {	
			strcpy(l[csac].uname[2], str_login);
			l[csac].train_no = no;
			h[csac].total++;
			l[csac].total = h[csac].total;
		}
		else if(l[csac].total == 3) {	
			strcpy(l[csac].uname[3], str_login);
			l[csac].train_no = no;
			h[csac].total++;
			l[csac].total = h[csac].total;
		}
		else if(l[csac].total == 4) {	
			strcpy(l[csac].uname[4], str_login);
			l[csac].train_no = no;
			h[csac].total++;
			l[csac].total = h[csac].total;
		}
		appendqueueac();
		csac++;	
	}
	return;
}
void Dequeuesl(int no) {
	int n;
	n = readqueuesl();
	int i;
	int look2 = 0;
	int fr = 0, ns;
	for(i = 0; i < SizeCust; i++) {
		if(e[i].train_no == no) {
			notesl = i;
			look2++;
		}
	}
	
	if(look2 == 0) { 
		printw("This train has no booking made\n");
		refresh();
		return;
	}
	for(i = 0; i < 5; i++) {
		if(strcmp(e[notesl].uname[i], str_login) == 0) {
			e[notesl].total--;
			strcpy(e[notesl].uname[i], "\0");
			fr++;
			ns = i;
		}
	}
	printw("ns%d%d", ns,notesl);
	for(ns; ns < 4; ns++) {
		strcpy(e[notesl].uname[ns], e[notesl].uname[ns + 1]);
	}
	strcpy(e[notesl].uname[4], "\0");
	e[notesl].train_no = no;
	  
	if(fr == 0) {
		printw("You have not booked a ticket in this train\n");
		refresh();		
		return;
	}
	writequeuesl(n);
	return;
}

void Dequeueac(int no) {
	int n;
	n = readqueueac();
	int i;
	int look2 = 0;
	int fr = 0, ns;
	for(i = 0; i < SizeCust; i++) {
		if(h[i].train_no == no) {
			noteac = i;
			look2++;
		}
	}
	if(look2 == 0) { 
		printw("This train has no booking made\n");
		refresh();
		return;
	}
	for(i = 0; i < 5; i++) {
		if(strcmp(e[noteac].uname[i], str_login) == 0) {
			g[noteac].total--;
			strcpy(g[noteac].uname[i], "\0");
			fr++;
			ns = i;
		}
	}
	for(ns; ns < 4; ns++) {
		strcpy(g[noteac].uname[ns], g[noteac].uname[ns  + 1]);
	}
	strcpy(g[noteac].uname[4], "\0");
	g[noteac].train_no = no;
	  
	if(fr == 0) {
		printw("You have not booked a ticket in this train\n");
		refresh();
		return;
	}
	writequeueac(n);
	return;
}
int DisplayQueuesl() {
	move(0,0);
	clrtobot();
	int i, n, num, nt = -1, j = 0;
	queue t[45];
	FILE *fp= fopen("qsl.txt", "r");
	if(fp == NULL) {
		perror("open failed:");
		refresh();
		return errno;
	}
	n = fread(&t, sizeof(t[0]), 45, fp);
	fclose(fp);
	printw("Enter train no\n");
	refresh();
	scanw("%d", &num);
	for(i = 0; i < 45; i++) {
		if(t[i].train_no == num) {
			nt = i;
		}
	}
	printw("%d\n", nt);
	if(t[nt].total < 3) {
		printw("No waiting\n");
		refresh();
	}
	else if(t[nt].total > 3) {
		printw("Waiting is:%d\n", (t[nt].total - 3));
		refresh();
	}
	while(j < 5) {
		printw("%s\t", t[nt].uname[j]);
		j++;
		refresh();
	} 
	printw("\n");
		
	return 0;
}

int DisplayQueueac() {
	move(0,0);
	clrtobot();
	int i, n, num, nt = -1, j = 0;
	queue t[45];
	FILE *fp= fopen("qac.txt", "r");
	if(fp == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&t, sizeof(t[0]), 45, fp);
	fclose(fp);
	printw("Enter train no\n");
	refresh();
	scanw("%d", &num);
	for(i = 0; i < 45; i++) {
		if(t[i].train_no == num) {
			nt = i;
		}
	}
	printw("%d\n", nt);
	if(t[nt].total < 3) {
		printw("No waiting\n");
	}
	else if(t[nt].total > 3) {
		printw("Waiting is:%d\n", (t[nt].total - 3));
	}	
	while(j < 5) {
		printw("%s\t", t[nt].uname[j]);
		j++;
	} 
	printw("\n");
	refresh();
		
	return 0;
}

void register_id() {
	move(0,0);
	clrtobot();
	echo();
	readCustomer();
	char uname[10], pass[10];
	int n = 1, max = 100000;
	time_t tt;
	int phone_no;	
	srandom(time(&tt));
	printw("Enter username\n");
	refresh();
	scanw("%s", uname);
		
	checkUserName(uname);
	printw("Enter your password\n");
	refresh();
	scanw("%s", pass);
	printw("Enter your phone no\n");
	refresh();
	scanw("%d", &phone_no);
	
	a[count].id =  (random() % max);	
	strcpy((a[count].uname), uname);
	strcpy((a[count].pass), pass);
	a[count].phone_no =  phone_no;
	count++;
	writeToCustomer();
}
int frd(int f) {
	if(f > 2) { 
		printw("No of wrong attempts exceeded. Please try again later\n");
		fraud = 0;
		refresh();
		exit(1);
	}
	return 0;
}
 
int login() {
	move(0,0);
	clrtobot();
	echo();
	readCustomer();
	int i, check = 0;
	char uname[10], pass[10];
	readCustomer();
	printw("Enter username:");
	refresh();
	scanw("%s", uname);
	printw("\nEnter password:");
	refresh();
	noecho();
	scanw("%s", pass);      /*makes the passowrd being typed invisible - for security*/	
	echo();	
	for(i = 0; i < SizeCust; i++) {
		if(strcmp(b[i].uname,uname) == 0 && strcmp(b[i].pass, pass) == 0) {
			so = i;
			check++;
		}
	}
	if(check == 0)  {
		printw("Invaild username or password. Try again\n");
		refresh();
		fraud++;
		frd(fraud);
		login();
	}
	lgn++;
	mvaddstr(0,20,"Welcome");
	printw("\n");
	move(1,20);
	printw("%s\n", b[so].uname);
	strcpy(str_login, b[so].uname);
	refresh();
	return 0;
}

int cancelticket() {
	move(0,0);
	clrtobot();
	int n, i, no, class;
	printw("Enter train no:");
	refresh();
	scanw("%d", &no);
	printw("\nPress 1 if class was ac, and 2 if class for sleeper\n");
	refresh();
	scanw("%d", &class);
	switch(class) {
		case 1: cancelac(no);
			   break;
		case 2: cancelsleeper(no);
			   break;
		default : printw("Incorrect selection. Try again\n");
			  refresh();	
			  cancelticket();
			  break;
	}
	
}		
					
int cancelac(int id) {
	int n, i;
	FILE *f7 = fopen("hav.txt", "r");
	if(f7 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&d, sizeof(d[0]), 45, f7);
	fclose(f7);

	for(i = 0; i < 45; i++) {
		if(d[i].no == id) {
			if(d[i].acseats < 3) {
				d[i].acseats = d[i].acseats + 1;
				printw("Ticket cancelled successfully\n");
				refresh();
				Dequeueac(id);
			}
			else {
				printw("You have not booked a ticket in this train or this category. Please verify\n");
				refresh();
			}
		}
	}
	updateseats();
	return 0;
}	

int cancelsleeper(int id) {
	int n, i;
	FILE *f7 = fopen("hav.txt", "r");
	if(f7 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&d, sizeof(d[0]), 45, f7);
		fclose(f7);

	for(i = 0; i < n; i++) {
		if(d[i].no == id) {
			if(d[i].sleeperseats < 3) {
				d[i].sleeperseats = d[i].sleeperseats + 1;
				printw("Ticket cancelled successfully\n");
				refresh();
				Dequeuesl(id);
			}
			else {
				printw("You have not booked a ticket in this train or this category. Please verify\n");
				refresh();
			}
		}
	}
	updateseats();
	return 0;
}	



