#include<stdio.h>
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
				printf("Username already in use. Please enter a new username\n");
				scanf("%s", um);
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
			printf("Sleeper:%d\tAC:%d\n", d[i].sleeperseats, d[i].acseats);
			ct++;
			break;
		}
		if(ct > 0) 
			return 0;
	}
		
}	
int findtrain() {
	if(lgn == 0) {
		printf("Please login and continue\n");
		login();
	}	
	int i, n, choice, book, ct = 0;
	char initial[20], destination[20];
	printf("Enter from location and destination\n");
	scanf("%s%s", initial, destination);
	
	FILE *f3 = fopen("usk.txt", "r");
	if(f3 == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&c, sizeof(c[0]), 45, f3);
	fclose(f3);

	for(i = 0; i < n; i++) {
		if(strcmp(c[i].from, initial) == 0 && strcmp(c[i].to, destination) == 0) {
			printf("Available trains is/are:\t Train no:%d\tTrain name:%s\n", c[i].no, c[i].name);
			printf("Available seats:\n");
			ct++;
			CheckAvailibility(c[i].no);
			printf("Would you like to book?\n Press 1 to book in sleeper and 2 to book in ac\n");
			scanf("%d", &book);
						
			switch(book) {
				case 1 : booktrainsleeper(c[i].no);
					 break;
				case 2 : booktrainac(c[i].no);	
					 break;				
				default : break;
			}
			
		}
		
	}
	if(ct == 0) {
			printf("No train available.Incorrect train no. Enter 1 to find another train or 2 to exit");
			scanf("%d", &choice);
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
				printf("Ticket booked. Here are your ticket details\n");
				Enqueuesl(str_login, id);	
				 						 
			}

			else { 
				printf("Capacity full. Press 1 to put in waitlist, or 2 to book in ac, or 3 to exit\n");	
				scanf("%d", &choice);
				
				switch(choice) {
					case 1 : if(d[i].sleeperseats > (-2)) {
							 d[i].sleeperseats = d[i].sleeperseats - 1;
							 Enqueuesl(str_login, id);
						 }
						 else {
							printf("Waitlist list is full. Regret\n");
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
				printf("Ticket booked. Here are your ticket details\n");
				Enqueueac(str_login, id);
									 
			}

			else { 
				printf("Capacity full. Press 1 to put in waitlist, or 2 to book in sleeper, or 3 to exit\n");	
				scanf("%d", &choice);
				
				switch(choice) {
					case 1 : if(d[i].acseats > (-2)) {
  						 	d[i].acseats = d[i].acseats - 1;
							Enqueueac(str_login, id);
						 }	
						 else {
							printf("Waitlist list is full. Regret\n");
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
	printf("value of note is:%d\t%d\n", notesl, i); 
	if(notesl != (-1)) {
		if(e[notesl].total > 4) {
			printf("Queue Full\n");
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
		printf("Works\n");
		g[cssl].total = 0;
		if(g[cssl].total > 4) {
			printf("Queue Full\n");
		}
		else if(g[cssl].total == 0) {	
			printf("Hi\n");
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
		printf("%d\t%s\t%d\n",g[cssl].train_no, g[cssl].uname[0], g[cssl].total);
		appendqueuesl();
		cssl++;
	}
		
	
	return;
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
			printf("Queue Full\n");
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
			printf("Queue Full\n");
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
		printf("This train has no booking made\n");
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
	printf("ns%d%d", ns,notesl);
	for(ns; ns < 4; ns++) {
		strcpy(e[notesl].uname[ns], e[notesl].uname[ns + 1]);
	}
	strcpy(e[notesl].uname[4], "\0");
	e[notesl].train_no = no;
	  
	if(fr == 0) {
		printf("You have not booked a ticket in this train\n");
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
		printf("This train has no booking made\n");
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
		printf("You have not booked a ticket in this train\n");
		return;
	}
	writequeueac(n);
	return;
}
int DisplayQueuesl() {
	int i, n, num, nt = -1, j = 0;
	queue t[45];
	FILE *fp= fopen("qsl.txt", "r");
	if(fp == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&t, sizeof(t[0]), 45, fp);
	fclose(fp);
	printf("Enter train no\n");
	scanf("%d", &num);
	for(i = 0; i < 45; i++) {
		if(t[i].train_no == num) {
			nt = i;
		}
	}
	printf("%d\n", nt);
	if(t[nt].total < 3) {
		printf("No waiting\n");
	}
	else if(t[nt].total > 3) {
		printf("Waiting is:%d\n", (t[nt].total - 3));
	}	
	while(j < 5) {
		printf("%s\t", t[nt].uname[j]);
		j++;
	} 
	printf("\n");
		
	return 0;
}

int DisplayQueueac() {
	int i, n, num, nt = -1, j = 0;
	queue t[45];
	FILE *fp= fopen("qac.txt", "r");
	if(fp == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&t, sizeof(t[0]), 45, fp);
	fclose(fp);
	printf("Enter train no\n");
	scanf("%d", &num);
	for(i = 0; i < 45; i++) {
		if(t[i].train_no == num) {
			nt = i;
		}
	}
	printf("%d\n", nt);
	if(t[nt].total < 3) {
		printf("No waiting\n");
	}
	else if(t[nt].total > 3) {
		printf("Waiting is:%d\n", (t[nt].total - 3));
	}	
	while(j < 5) {
		printf("%s\t", t[nt].uname[j]);
		j++;
	} 
	printf("\n");
		
	return 0;
}

void register_id() {
	readCustomer();
	char uname[10], pass[10];
	int n = 1, max = 100000;
	time_t tt;
	int phone_no;	
	srandom(time(&tt));
	printf("Enter username\n");
	scanf("%s", uname);
		
	checkUserName(uname);
	printf("Enter your password\n");
	scanf("%s", pass);
	printf("Enter your phone no\n");
	scanf("%d", &phone_no);
	
	a[count].id =  (random() % max);	
	strcpy((a[count].uname), uname);
	strcpy((a[count].pass), pass);
	a[count].phone_no =  phone_no;
	count++;
	writeToCustomer();
}
int frd(int f) {
	if(f > 2) { 
		printf("No of wrong attempts exceeded. Please try again later\n");
		fraud = 0;
		exit(1);
	}
	return 0;
}
 
int login() {
	readCustomer();
	int i, check = 0;
	char uname[10], pass[10];
	readCustomer();
	printf("Enter username:");
	scanf("%s", uname);
	printf("\nEnter password:");
	scanf("%s", pass);      /*makes the passowrd being typed invisible - for security*/	
		
	for(i = 0; i < SizeCust; i++) {
		if(strcmp(b[i].uname,uname) == 0 && strcmp(b[i].pass, pass) == 0) {
			printf("Welcome\t%s\n", b[i].uname);
			so = i;
			check++;
			strcpy(str_login, b[i].uname);
		}
	}
	if(check == 0)  {
		printf("Invaild username or password. Try again\n");
		fraud++;
		frd(fraud);
		login();
	}
	lgn++;
	return 0;
}

int cancelticket() {
	int n, i, no, class;
	printf("Enter train no:");
	scanf("%d", &no);
	printf("\nPress 1 if class was ac, and 2 if class for sleeper\n");
	scanf("%d", &class);
	switch(class) {
		case 1: cancelac(no);
			   break;
		case 2: cancelsleeper(no);
			   break;
		default : printf("Incorrect selection. Try again\n");
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

	for(i = 0; i < n; i++) {
		if(d[i].no == id) {
			if(d[i].acseats < 3) {
				d[i].acseats = d[i].acseats + 1;
				printf("Ticket cancelled successfully\n");
				Dequeueac(id);
			}
			else {
				printf("You have not booked a ticket in this train or this category. Please verify\n");
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
				printf("Ticket cancelled successfully\n");
				Dequeuesl(id);
			}
			else {
				printf("You have not booked a ticket in this train or this category. Please verify\n");
			}
		}
	}
	updateseats();
	return 0;
}	



