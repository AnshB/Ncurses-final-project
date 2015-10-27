#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


int main() {
	int select;
	int id;	
	printf("Welcome to the rail booking system\n");
	
	
	while(1) {
		printf("Select from the menu below what you wish to do\n");
	scanf("%d", &select);
		switch(select) {
			case 1: printf("Log in/Register\n");
				login();
				break;
			case 2: printf("Book ticket\n");
				findtrain();
				break;
			case 3: printf("Enter train number\n");
				scanf("%d", &id);
				CheckAvailibility(id);
				break;
			case 4: printf("Check ticket status\n");
				register_id();				
				break;
			case 5: printf("Cancel ticket\n");
				cancelticket();
				break;			
			case 6: exit(1);
			case 7: DisplayQueueac();
				break;
			case 8: DisplayQueuesl();
				break;
				
				
			default: printf("Incorrect option. Please select again\n"); 
				break;
		}
	}
	return 0;
}
