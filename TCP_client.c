#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h> // read(), write(), close()

#define MAX 1024
#define IP "127.0.0.1"
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

void read_file(int sockfd, FILE *fname)
{
	char buff[MAX];
	int count = 0;
	while (1)
	{
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %d  \n", count);

		if (strcmp(buff, "endtrans") == 0)
		{
			break;
		}

		fwrite(buff, 1, MAX, fname);
		count += 1;
	}
}

void send_msg(int sockfd, const char *msg)
{
	char buff[MAX];
	send(sockfd, msg, strlen(msg) + 1, 0);
	printf("Message (%s) sent\n", msg);

	strcpy(buff, "endtrans");
	send(sockfd, buff, sizeof(buff), 0);
}

char *user_input() {
	char buff[MAX];
	printf("Enter a filename (or 'q' to exit): ");

	char *ret = fgets(buff, MAX, stdin);

	return ret;
}

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
	{
		printf("connected to the server..\n");
	}

	char *fname;
	struct stat sb;
	FILE *dest_fp = NULL;
	while (1)
	{
		fname = user_input();
		if (fname[0] == 'q') {
			send_msg(sockfd, "q");
			printf("Exiting...\n");
			break;
		}

		printf("%s", fname);
		if (stat(fname, &sb) == -1)
		{
			printf("File does not exist\n");
			continue;
		}
		send_msg(sockfd, fname);
		if (strcmp(recv_msg(sockfd), "exists") != 0)
		{
			printf("File does not exist on the server\n");
			continue;
		}

		char dest_addr[MAX];
		snprintf(dest_addr, MAX, "tmpout/%s", fname);
		dest_fp = fopen(dest_addr, "rb");

		read_file(sockfd, dest_fp);
		fclose(dest_fp);
	}

	// close the socket
	close(sockfd);
}
