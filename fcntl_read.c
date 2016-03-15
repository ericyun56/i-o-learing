#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd;
	fd = open("hello",O_RDWR | O_CREAT,066);
	if(fd < 0){
		perror("open:");
		exit(1);
	}
	lock_set(fd,F_RDLCK);
	getchar();
	lock_set(fd,F_UNLCK);
	getchar();
	close(fd);
	exit(0);
}