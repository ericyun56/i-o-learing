#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/select.h>

int main()
{
	int fds[2];
	char buf[7];
	int i,rc,maxfd;
	fd_set inset1,inset2;
	struct timeval tv;
	if((fds[0] = open("hello1",O_RDWR | O_CREAT,0666)) < 0)
		perror("open hello1");
	if((fds[1] = open("hello2",O_RDWR | O_CREAT,0666)) < 0)
		perror("open hello2");
	if((rc = write(fds[0],"Hello!\n",7)))
		printf("rc=%d\n",rc);
	lseek(fds[0],0,SEEK_SET);
	maxfd = fds[0] >fds[1] ? fds[0] : fds[1];
	FD_ZERO(&inset1);
	FD_SET(fds[0],&inset1);

	FD_ZERO(&inset2);
	FD_SET(fds[1],&inset2);

	while(FD_ISSET(fds[0],&inset1) || FD_ISSET(fds[1],&inset2)){
		if(select(maxfd+1,&inset1,&inset2,NULL,&tv))
			perror("select");
		else{
			if(FD_SET(fds[0],&inset1)){
				rc = read(fds[0],buf,7);
				if(rc > 0){
					buf[rc] = '\0';
					printf("read:%s\n",buf);
				}
				else
					perror("read:");
			}

			if(FD_SET(fds[1],&inset2)){
				rc = read(fds[0],buf,7);
				if(rc > 0){
					buf[rc] = '\0';
					printf("rc=%d,write:%s\n",rc,buf);
				}
				else
					perror("write:");
				sleep(10);
			}

		}
	}
	exit(0);
}