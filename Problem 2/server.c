#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <time.h> 
#include <fcntl.h>
#include "packet.h"
	
#define TRUE 1 
#define FALSE 0 
	
double getRand ()
{
	return rand() / (double)(RAND_MAX) ;
}

int main(int argc , char *argv[]) 
{ 
	struct sockaddr_in serverAddr, otherAddr ;
	int serverSock, valRead, disconnected = 0, i, flags ;
	int slen = sizeof (struct sockaddr_in) ;

	data *datPkt = (data *) malloc (sizeof (data)) ;
	data *ackPkt = (data *) malloc (sizeof (data)) ;
	ackPkt->pktType = ACK ;
	serverSock = setSockAddrBind (&serverAddr, SERVER_PORT) ;

	/*
	flags = fcntl(serverSock, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(serverSock, F_SETFL, flags);
	*/

	fd_set serverfd ;
	FD_ZERO (&serverfd) ;
	FD_SET (serverSock, &serverfd) ;

	printf ("Waiting for connection!\n") ;
	while (TRUE)
	{
		select (serverSock + 1 , &serverfd, NULL, NULL, NULL) ;
		if (FD_ISSET(serverSock, &serverfd))
		{
			while (recvfrom(serverSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen) != -1)
			{
				if (1/*getRand() > DROP*/)
				{
					printf ("%d : RCVD\n", datPkt->offset) ;
					ackPkt->offset = datPkt->offset ;
					sendto (serverSock, ackPkt, sizeof (data), 0, (struct sockaddr *) &otherAddr, slen) ;
				}
				else
					printf ("%d : DROP\n", datPkt->offset) ;	
			} 
		}

		FD_ZERO (&serverfd) ;
		FD_SET (serverSock, &serverfd) ;
	}

	return 0; 
}
