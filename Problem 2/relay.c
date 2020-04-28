//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
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

double getDelay ()
{
	return 2 * (double)rand()/(double)(RAND_MAX) ;
}

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE ;

	struct sockaddr_in serverAddr ;
	int serverSock ;

	//serverSock = setSockAddr (&serverAddr, SERVER_PORT) ;

	if (fork())
	{
		struct sockaddr_in relayEvenAddr, otherAddr ; 
		int relayEvenSock, new_socket, clientSock, slen = sizeof(struct sockaddr_in ) ;
		int i, max_sd, sd, valread, disconnect = 0, activity ;
		double delay ;

		data *datPkt = (data *) malloc (sizeof(data));
		data *ackPkt = (data *) malloc (sizeof (data)) ;
		ackPkt->pktType = ACK ;
		relayEvenSock = setSockAddrBind (&relayEvenAddr, RELAY_EVEN_PORT) ;

		fd_set evenfd ;
		FD_ZERO (&evenfd) ;
		FD_SET (relayEvenSock, &evenfd) ;

		printf("RELAY_EVEN : Waiting for connection\n"); 
		while (TRUE)
		{
			select (relayEvenSock + 1 , &evenfd, NULL, NULL, NULL) ;
			if (FD_ISSET (relayEvenSock, &evenfd) && (valread = recvfrom(relayEvenSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen))) 
			{
				delay = getDelay () ;
				if (!fork())
				{
					sleep (delay/1000) ;
					printf ("%d : %d\n", 0, datPkt->offset) ;
					//sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;

					ackPkt->offset = datPkt->offset ;
					printf ("EVEN %d : ACK\n", ackPkt->offset) ;
					sendto (relayEvenSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&otherAddr, slen) ;

					exit (0) ;
				}
			}

			if (disconnect == 1)
				break ;
		}

		close (relayEvenSock) ;
	}
	else
	{
		struct sockaddr_in relayOddAddr, otherAddr ; 
		int relayOddSock, new_socket, clientSock, slen = sizeof(struct sockaddr_in) ;
		int i, max_sd, sd, valread, disconnect = 0, activity ;
		double delay ;

		data *datPkt = (data *) malloc (sizeof(data));
		data *ackPkt = (data *) malloc (sizeof (data)) ;
		ackPkt->pktType = ACK ;
		relayOddSock = setSockAddrBind (&relayOddAddr, RELAY_ODD_PORT) ;

		fd_set oddfd ;
		FD_ZERO (&oddfd) ;
		FD_SET (relayOddSock, &oddfd) ;
		
		printf("RELAY_ODD : Waiting for connection\n"); 

		while (TRUE)
		{
			select (relayOddSock + 1 , &oddfd, NULL, NULL, NULL) ;
			if (FD_ISSET (relayOddSock, &oddfd) && (valread = recvfrom(relayOddSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)))
			{
				delay = getDelay () ;
				if (!fork())
				{
					sleep (delay/1000) ;
					printf ("\t%d : %d\n", 1, datPkt->offset) ;
					//sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;

					ackPkt->offset = datPkt->offset ;
					printf ("\tODD %d : ACK\n", ackPkt->offset) ;
					sendto (relayOddSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&otherAddr, slen) ;
					exit (0) ;
				}
			}

			if (disconnect == 1)
				break ;
		}

		close (relayOddSock) ;
	}
}
