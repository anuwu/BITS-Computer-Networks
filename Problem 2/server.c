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
	int serverSock, valRead, disconnected = 0, valread, i ;
	int slen = sizeof (struct sockaddr_in) ;

	int lastRcvd = 0, lastOffset, maxOffset = -1 ;

	
	data *ackPkt = (data *) malloc (sizeof (data)) ;
	ackPkt->pktType = ACK ;
	serverSock = setSockAddrBind (&serverAddr, SERVER_PORT) ;

	printf ("Waiting for connection!\n") ;
	while (TRUE)
	{
		if ((valread = recvfrom(serverSock , datBuf, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)) == -1) 
		{
			printf ("Disconnected\n") ;
			disconnected++ ;
		}	
		else
		{
			printf ("%d\n", i, datBuf->offset) ;
			ackPkt->offset = datBuf->offset ;
			sendto (serverSock, ackPkt, valread, 0, (struct sockaddr *) &otherAddr, slen) ;

			if (datBuf->isLast == YES)
			{
				lastOffset = datBuf->offset ;
				lastRcvd = 1 ;
			}
			else if (maxOffset > datBuf->offset)
				maxOffset = datBuf->offset ;

		}	
	}

	return 0; 
}
