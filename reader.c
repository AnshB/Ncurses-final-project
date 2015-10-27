/* This programs reads a set of structures in a file. 
   We know that the structures are stored in a sequence in the file. So we can read it back one by one, or also as an array.
   This code tries to read it back as an array and then prints the elements of the array.

   The program read.c reads the records using "open,read,close" set of functions.
   This program reads the same file using "foopen,fclose,fread" set of functions.

   Compile the code as: 
   >cc fread.c -o fread
   Run the code as:  
   >./fread data
*/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc, char *argv[]) {
	int i, x, n;
	struct tdata {
	int no;
	char name[50];
	char from[50];
	char to[50];
	char day[9];
	}t[45];
	FILE *fp= fopen(argv[1], "r");
	if(fp == NULL) {
		perror("open failed:");
		return errno;
	}
	/* Askign to read 16 records, each of size sizeof(t[0]) */
	n = fread(&t, sizeof(t[0]), 45, fp);
	printf("%d\n", n);
	printf("%s\n", t[1].from);
	/* fread may read less number of RECORDS (not bytes), than requested. That is given in n */
	for(i = 0; i < n; i++)
		printf("%d\t%s\t%s\t%s\t%s\n", t[i].no, t[i].name, t[i].from, t[i].to, t[i].day);

	/* scanf returns -1 when user presses ctrl-d. EOF is #defined to be -1 in stdio.h */
	fclose(fp);
	return 0;
}
