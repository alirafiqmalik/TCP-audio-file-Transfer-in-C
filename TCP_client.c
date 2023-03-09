#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 1024
char *ip = "127.0.0.1";
#define PORT 8080
#define SA struct sockaddr


FILE *destination;

void write_to_file(unsigned char buffer[],int n,int filename){

	// char path[MAX];
	// sprintf(path, "./tmpout/%d", filename);
	// printf("%s \n\n",path);


// fwrite(buffer, 1, n, destination);
fprintf(destination, "%s", buffer);
// bzero(buffer, 1024);
}


void func(int sockfd)
{
	char buff[MAX];
	// unsigned char buffer[MAX-2];
	int count=0;
	while(1){
	
	read(sockfd, buff, sizeof(buff));
	// strcpy(buffer,buff);
	// printf("From Server : %s \n\n", buff);
	
	printf("From Server : %d  \n\n", count);
	if(strcmp(buff,"endtrans")==0){
		break;
	}

if(count==1081)
fwrite(buff, 1, 436, destination);
else
	fwrite(buff, 1, 1024, destination);

	// fprintf(destination, "%s", buffer);
	// count=buff[MAX-1]*10+buff[MAX-2];
	
	// write_to_file(buff,1024,count);
	count+=1;
	}
}


// 

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	destination = fopen("tmpout/tmpout.m4a", "wb");
	func(sockfd);


fclose(destination);
	// close the socket
	close(sockfd);
}
