#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // close() function

#define BUFSIZE 32

int main()
{
	/* CREATE A TCP SOCKET*/
	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if (sock < 0) 
	{ 
		printf ("Error in opening a socket"); 
		exit (0);
	}

	printf ("Client Socket Created with %d\n", sock);

	/*CONSTRUCT SERVER ADDRESS STRUCTURE*/
	struct sockaddr_in serverAddr;

	/*memset() is used to fill a block of memory with a particular value*/
	memset (&serverAddr,0,sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345); 	// This is the port number that the client connects to
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.106"); //Specify server's

	// 192.168.1.106

	printf ("Address assigned\n");

	/*ESTABLISH CONNECTION*/
	int c = connect (sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
	printf ("%d\n",c);

	if (c < 0)
	{ 
		printf ("Error while establishing connection");
		exit (0);
	}

	printf ("Connection Established\n");

	printf ("ENTER MESSAGE FOR SERVER with max 32 characters\n");
	char msg[BUFSIZE];
	gets(msg);
	int bytesSent = send (sock, msg, strlen(msg), 0);

	// Send message
	if (bytesSent != strlen(msg))
	{ 
		printf("Error while sending the message");
		exit(0);
	}
	printf ("Data Sent\n");

	/*RECEIVE BYTES*/
	char recvBuffer[BUFSIZE];
	int bytesRecvd = recv (sock, recvBuffer, BUFSIZE-1, 0);
	if (bytesRecvd < 0)
	{ 
		printf ("Error while receiving data from server");
		exit (0);
	}

	recvBuffer[bytesRecvd] = '\0';
	printf ("%s\n",recvBuffer);

	close(sock);
}
