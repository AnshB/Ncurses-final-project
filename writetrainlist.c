#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc, char *argv[]) {
	int i;
	struct tdata {
	int no;
	char name[50];
	char from[50];
	char to[50];
	char day[9];
	}t;
	FILE *fd = fopen("usk.txt", "w");
	if(fd == NULL) {
		perror("open failed:");
		return errno;
	}
	
	while(scanf("%d%s%s%s%s", &t.no, t.name, t.from, t.to, t.day) != EOF)
		fwrite(&t, sizeof(t), 1, fd);
	close(fd);
	return 0;
}
