#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	int i;
	struct tdata {
	int no;
	int sleeper;
	int ac;
	}s;

	FILE *fs = fopen("hav.txt", "w");
	if(fs == NULL) {
		perror("open failed:");
		return errno;
	}
	
	while(scanf("%d%d%d", &s.no, &s.sleeper, &s.ac) != EOF)
		fwrite(&s, sizeof(s), 1, fs);
	close(fs);
	return 0;
}
