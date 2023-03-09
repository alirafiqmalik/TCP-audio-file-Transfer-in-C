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

FILE *source;
int n;
int count = 0;
int written = 0;

void func(int sockfd)
{
	char buff[MAX];

	source = fopen("test_audio/audio.m4a", "rb");

	if (source)
	{
		int counti = 0;
		while (!feof(source))
		{
			n = fread(buff, 1, MAX, source);
			count += n;
			printf("n = %d  %d\n", counti, n);

			write(sockfd, buff, sizeof(buff));

			counti += 1;
			// if(counti==4){break;}
		}
		printf("%d bytes read from library and sent.\n", count);
		fclose(source);
	}
	else
	{
		printf("File Access Failed\n");
	}
	write(sockfd, "endtrans", sizeof(buff));


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

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
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
		printf("Socket successfully created..\n");
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
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA *)&cli, &len);
	if (connfd < 0)
	{
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// // Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
