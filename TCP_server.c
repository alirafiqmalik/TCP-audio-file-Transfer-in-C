#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

char *recv_msg(int sockfd) {
	char buff[MAX];
	char *tmp = NULL;
	int nread;

	if ((nread = recv(sockfd, buff, sizeof(buff), 0)) > 0)
	{
		tmp = malloc(sizeof(char) * nread);
		memcpy(tmp, buff, nread);
		return tmp;
	}

	return NULL;
}

void send_msg(int sockfd, const char *msg)
{
	char buff[MAX];
	send(sockfd, msg, strlen(msg) + 1, 0);
	printf("Message (%s) sent\n", msg);

	strcpy(buff, "endtrans");
	send(sockfd, buff, sizeof(buff), 0);
}

void send_file (int sockfd, const char *fname)
{
	int n;
	int count = 0;
	char buff[MAX];

	FILE *source = fopen(fname, "rb");

	if (source == NULL)
	{
		printf("File Access Failed\n");
	} else {
		int counti = 0;
		while (!feof(source))
		{
			n = fread(buff, 1, MAX, source);
			count += n;
			printf("n = %d  %d\n", counti, n);

			send(sockfd, buff, sizeof(buff), 0);

			counti += 1;
			// if(counti==4){break;}
		}
		printf("%d bytes read from library and sent.\n", count);
		fclose(source);
	}

	
	strcpy(buff, "endtrans");
	send(sockfd, buff, sizeof(buff), 0);
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int yes = 1;
	// char yes='1'; // use this under Solaris
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_LINGER, &yes, sizeof(yes)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
	{
		printf("Socket successfully created..\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("socket bind failed...\n");
		exit(0);
	}
	else
	{
		printf("Socket successfully binded..\n");
	}

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
	{
		printf("Server listening..\n");
	}
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA *)&cli, (socklen_t *)&len);
	if (connfd < 0)
	{
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");
	// Function for chatting between client and server
	char *fname = NULL;
	while ((fname = recv_msg(connfd))[0] != 'q' && strlen(fname) != 1)
	{
		send_file(connfd, fname);
		free(fname);
	}
	printf("Closing connection...\n");

	// After chatting close the socket
	close(sockfd);
}
