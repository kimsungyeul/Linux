#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT	5100

int main(int argc, char **argv)
{
	int ssock;
	struct sockaddr_in servaddr;
	char mesg[BUFSIZ];
	fd_set readfd;						//

	if(argc < 2) {
		printf("Usage : %s IP_ADRESS\n", argv[0]);
		return -1;
	}
	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &(servaddr.sin_addr.s_addr)); 
	servaddr.sin_port = htons(TCP_PORT);

	if(connect(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect()");
		return -1;
	}

	while(1) {
		FD_ZERO(&readfd);									//
		FD_SET(0, &readfd);								//
		FD_SET(ssock, &readfd);						//

		select(ssock+1, &readfd, NULL,NULL , NULL);		//
		memset(mesg, 0, BUFSIZ);

		if(FD_ISSET(ssock, &readfd) != 0) {
			int n = read(ssock, mesg , BUFSIZ);
			if(n <= 0) {
				break;
			}
			printf("FROM SERVER : %s\n", mesg);
		}

		if(FD_ISSET(0, &readfd) != 0) {
			read(0, mesg, BUFSIZ);
			int n = strlen(mesg);
			write(ssock, mesg, BUFSIZ);
		}

	} while(strcmp(mesg, "q"));

	close(ssock);

	return 0;
}
