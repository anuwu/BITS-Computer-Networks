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
#include<time.h> 
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
	int serverSock, valRead, disconnected = 0, i ;
	int slen = sizeof (struct sockaddr_in) ;

	data *datPkt = (data *) malloc (sizeof (data)) ;
	data *ackPkt = (data *) malloc (sizeof (data)) ;
	ackPkt->pktType = ACK ;
	serverSock = setSockAddrBind (&serverAddr, SERVER_PORT) ;

	printf ("Waiting for connection!\n") ;
	while (TRUE)
	{
		if (recvfrom(serverSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen))
		{
			printf ("%d : RCVD\n", datPkt->offset) ;
			ackPkt->offset = datPkt->offset ;
			sendto (serverSock, ackPkt, sizeof (data), 0, (struct sockaddr *) &otherAddr, slen) ;
		} 	
	}

	return 0; 
}
