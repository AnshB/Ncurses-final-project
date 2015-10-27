
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc, char *argv[]) {
	int i, x, n, j;
	struct queue {
	int train_no;
	char uname[5][10];	
	int total;
}t[45];
	FILE *fp= fopen("qsl.txt", "r");
	if(fp == NULL) {
		perror("open failed:");
		return errno;
	}
	n = fread(&t, sizeof(t[0]), 20, fp);
	for(i = 0; i < n; i++) {
		for(j = 0; j < 5; j++) {
			printf("%d\t%s\t%d\n", t[i].train_no, t[i].uname[j], t[i].total);
			}
		
	}
	fclose(fp);
	return 0;
}
