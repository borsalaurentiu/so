#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define RESP_NAME "RESP_PIPE_29258"
#define REQ_NAME "REQ_PIPE_29258"

int main(void)
{
	int fd1 = -1;
	int fd2 = -1;
	int no = 29258;
	char *x = "CONNECT";
	char *pong = "PONG";
	int x_len = strlen(x);
	int pong_len = strlen(pong);
	char* str = (char*)malloc(sizeof(char));

	if(mkfifo(RESP_NAME, 0600) != 0){
		perror("ERROR\ncannot create the response pipe");
		exit(1);
	}

	fd1 = open(REQ_NAME, O_RDONLY);
	if(fd1 == -1){
		perror("ERROR\ncannot open the request pipe");
		exit(1);
	}

	fd2 = open(RESP_NAME, O_WRONLY);
	if(fd2 == -1){
		perror("ERROR\ncannot open the request pipe");
		exit(1);
	}

	write(fd2, &x_len, sizeof(char));
	write(fd2, x, x_len);

	read(fd1, &x_len, sizeof(char));
	read(fd1, str, x_len);

	int str_len = strlen(str);

	write(fd2, &str_len, sizeof(char));
	write(fd2, str, str_len);
	write(fd2, &pong_len, sizeof(char));
	write(fd2, pong, pong_len);
	write(fd2, &no, sizeof(int));
		
	close(fd2);
	unlink(RESP_NAME);
	close(fd1);
	unlink(REQ_NAME);

	return 0;
}
