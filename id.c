#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include<ncurses.h>
#define size 5      /*normal seating + those alloted for waitlist*/
#define SizeCust 50

int ct = 0;
typedef struct queue {
	int train_no;
	int id;
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
	char time[8];
}tdata;

typedef struct tseats {
	int no;
	int sleeperseats;
	int acseats;
}tseats;

store a[SizeCust];
storer b[SizeCust];
tdata c[SizeCust];
tseats d[SizeCust];
queue e[SizeCust];
int front = -1, rear = -1, so;

int checkqueue() {
	FILE *f1 = fopen("yo", "r");
	if(f1 == NULL) {
		perror("open failed:");
		return errno;
	}
	fread(&b, sizeof(b[0]), SizeCust, f1);
	return 0;
	fclose(f1);
}

void checkUserName(char unm[]) {
	int i = 0;
	char um[10];
	while (i < SizeCust) {
			if(ct == 0) {
				return;
			}
			else if(strcmp(unm, b[i].uname) == 0) {
				printw("Username already in use. Please enter a new username\n");
				scanw("%s", um);
				refresh();
				checkUserName(um);
			}	
			else { 
				break;
			}	
	}
}	

int writeToCustomer() {
	int i = 0;
	FILE *f2 = fopen("yo", "w+");
	if(f2 == NULL) {
		perror("open failed:");
		return errno;
	}
	fwrite(&a[i], sizeof(a[i]), 1, f2);
	return 0;
	fclose(f2);
}
	
int readCustomer() {
	FILE *f1 = fopen("yo", "r");
	if(f1 == NULL) {
		perror("open failed:");
		return errno;
	}
	fread(&b, sizeof(b[0]), SizeCust, f1);
	return 0;
	fclose(f1);
}

int CheckAvailibility(int id) {
	int i;
	move(0,0);
	clrtobot();
	FILE *f4 = fopen("seatstrains", "r");
	if(f4 == NULL) {
		perror("open failed:");
		return errno;
	}
	fread(&d, sizeof(d[0]), SizeCust, f4);

	for(i = 0; i < 45; i++) {
		if(d[i].no == id) {
			printw("Sleeper:%dAC:%d\n", d[i].sleeperseats, d[i].acseats);
			refresh();
		}
	}
	fclose(f4);
	return 0;	
}	
int findtrain() {
	int i, choice, book;
	char initial[20], destination[20];
	move(0,0);
	clrtobot();
	printw("Enter from location and destination\n");
	scanw("%s%s", initial, destination);
	refresh();
	FILE *f3 = fopen("ltrains", "r");
	if(f3 == NULL) {
		perror("open failed:");
		return errno;
	}
	fread(&c, sizeof(c[0]), 45, f3);
	for(i = 0; i < 45; i++) {
		if((c[i].from == initial) && (c[i].to == destination)) {
			printw("Available trains is/are:\t Train no:%d\tTrain name:%s\n", c[i].no, c[i].name);
			printw("Available seats:\n");
			refresh();
			CheckAvailibility(c[i].no);
			printw("Would you like to book?\n Press 1 to book in sleeper and 2 to book in ac\n");
			scanw("%d", &book);
			refresh();			
			switch(book) {
				case 1 : booktrainsleeper(c[i].no);
					 break;
				case 2 : booktrainac(c[i].no);	
					 break;				
				default : break;
			}
		}
		else {
			printw("No train available. Enter 1 to find another train or 2 to exit");
			scanw("%d", &choice);
			refresh();	
		}
	}
	switch(choice) {
		case 1 : findtrain();
			 break;
		case 2 : exit(1);
	
		default :
			  break;
	}
	fclose(f3);
}
		
int booktrainsleeper(int id) {
	int i, choice; 	 
	for(i = 0; i < SizeCust; i++) {
		if(d[i].no == id) {
			if(d[i].sleeperseats > 0) {
				d[i].sleeperseats = d[i].sleeperseats - 1;
				printw("Ticket booked. Here are your ticket details\n");
				refresh();
				 						 
			}

			else { 
				printf("Capacity full. Press 1 to put in waitlist, or 2 to book in ac, or 3 to exit\n");	
				scanf("%d", &choice);
				refresh();
				switch(choice) {
					case 1 : d[i].sleeperseats = d[i].sleeperseats - 1;
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
				printf("Ticket booked. Here are your ticket details\n");
				refresh();
									 
			}

			else { 
				printf("Capacity full. Press 1 to put in waitlist, or 2 to book in sleeper, or 3 to exit\n");	
				scanf("%d", &choice);
				refresh();
				switch(choice) {
					case 1 : d[i].acseats = d[i].acseats - 1;
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

int updateseats() {
	int i;	
	FILE *f4 = fopen("filename", "w");
	if(f4 == NULL) {
		perror("open failed:");
		return errno;
	}
	for(i = 0; i < SizeCust; i++) {
		fwrite(&a[i], sizeof(a[i]), 1, f4);
	}
	fclose(f4);
}	

 
	

void Enqueue(int login_id, int wait) {
	if(rear == 1) {
		printw("Waiting is full full\n"); 

	}
	else if(front == -1 && rear == -1) {	
		front = 0;
		rear = 0;
		e[rear].id = login_id;
	}
	else {
		rear = rear + 1;
		e[rear].id = login_id;
	}
	
	return;
}

void Dequeue() {
	if(front == -1 && rear == -1) {
		printw("Queue is empty\n");
	}
	else if(front == rear) {		
		front = -1;
		rear = -1;	}
	else {		
		front = front + 1;
	} 
	return;
}

void DisplayQueue(int train_no) {
	int i = front;
	move(0,0);
	clrtobot();
	if(front == -1 && rear == -1) {
		printw("No waiting\n");
		refresh();	
	}
	else {
		while(i <= rear) {
			printw("%d\n", e[i].id);
			i++;
			refresh();
		}
	} 
	printw("Total waiting:%d", i);
	refresh();	
	return;
}
int writequeue() {
	int i;	
	FILE *f5 = fopen("filename", "w+");
	if(f5 == NULL) {
		perror("open failed:");
		return errno;
	}
	for(i = 0; i < SizeCust; i++) {
		fwrite(&e[i], sizeof(e[i]), 1, f5);
	}
}	
void register_id() {
	readCustomer();
	char uname[10], pass[10];
	int n = 1, max = 100;
	time_t tt;
	int phone_no;	
	srandom(time(&tt));
	printw("Enter username\n");
	scanw("%s", uname);
	refresh();	
	checkUserName(uname);
	printw("Enter your password\n");
	scanw("%s", pass);
	printf("Enter your phone no\n");
	scanw("%d", &phone_no);
	refresh();
	a[ct].id =  (random() % max);	
	strcpy((a[ct].uname), uname);
	strcpy((a[ct].pass), pass);
	a[ct].phone_no =  phone_no;
	ct++;
	writeToCustomer();
}

void login() {
	move(0,0);
	clrtobot();
	int i;
	char uname[10], pass[10];
	readCustomer();
	printf("Enter username and password");
	scanw("%s", uname);
	scanw("%s", pass);      /*makes the passowrd being typed invisible - for security*/	
	refresh();	
	for(i = 0; i < SizeCust; i++) {
		if(b[i].uname == uname && b[i].pass == pass) {
			printw("Welcome\t%s", b[i].uname);
			so = i;
		}
	 
	}
	printf("Invaild username or password. Try again\n");
	login();
}



