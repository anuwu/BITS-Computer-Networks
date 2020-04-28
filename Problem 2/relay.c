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
#include <time.h> 
#include <fcntl.h>
#include "packet.h"
	
#define TRUE 1 
#define FALSE 0 

double getDelay ()
{
	return 2 * (double)rand()/(double)(RAND_MAX) ;
}

int main(int argc , char *argv[]) 
{ 
	struct sockaddr_in serverAddr ;
	int serverSock ;
	serverSock = setSockAddr (&serverAddr, SERVER_PORT) ;

	struct sockaddr_in relayEvenAddr, relayOddAddr, clientAddr ; 
	int relayEvenSock, relayOddSock, new_socket, clientSock, slen = sizeof(struct sockaddr_in ) ;
	int i, max_sd, sd, disconnect = 0, activity, flags ;
	double delay ;

	data *pkt = (data *) malloc (sizeof (data)) ;
	data *datPkt ;
	data *ackPkt = (data *) malloc (sizeof (data)) ;
	ackPkt->pktType = ACK ;

	if (fork())
	{
		relayEvenSock = setSockAddrBind (&relayEvenAddr, RELAY_EVEN_PORT) ;

		fd_set evenfds ;
		FD_ZERO (&evenfds) ;
		FD_SET (relayEvenSock, &evenfds) ;
		FD_SET (serverSock, &evenfds) ;
		max_sd = (relayEvenSock > serverSock) ? relayEvenSock : serverSock ;

		printf("RELAY_EVEN : Waiting for connection\n"); 
		while (TRUE)
		{
			select (max_sd + 1 , &evenfds, NULL, NULL, NULL) ;
			if (FD_ISSET (relayEvenSock, &evenfds))
			{
				while (recvfrom(relayEvenSock , pkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen) != -1)
				{
					datPkt = pkt ;
					delay = getDelay () ;
					if (!fork())
					{
						sleep (delay/1000) ;
						printf ("%d : RECV\n", datPkt->offset) ;
						sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;
						exit (0) ;
					}
				}
			}

			if (FD_ISSET (serverSock, &evenfds))
			{
				while (recvfrom(serverSock , pkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, &slen) != -1)
				{
					ackPkt = pkt ;
					printf ("%d : ACK\n", ackPkt->offset) ;
					sendto (relayEvenSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&clientAddr, slen) ;
				}
			}

			FD_ZERO (&evenfds) ;
			FD_SET (relayEvenSock, &evenfds) ;
			FD_SET (serverSock, &evenfds) ;
		}

		close (relayEvenSock) ;
	}
	else
	{
		relayOddSock = setSockAddrBind (&relayOddAddr, RELAY_ODD_PORT) ;

		fd_set oddfds ;
		FD_ZERO (&oddfds) ;
		FD_SET (relayOddSock, &oddfds) ;
		FD_SET (serverSock, &oddfds) ;

		max_sd = (relayOddSock > serverSock) ? relayOddSock : serverSock ;
		
		printf("RELAY_ODD : Waiting for connection\n"); 
		while (TRUE)
		{
			select (max_sd + 1 , &oddfds, NULL, NULL, NULL) ;
			if (FD_ISSET (relayOddSock, &oddfds))
			{
				while (recvfrom(relayOddSock , pkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen) != -1)
				{
					datPkt = pkt ;
					delay = getDelay () ;
					if (!fork())
					{
						sleep (delay/1000) ;
						printf ("%d : RECV\n", datPkt->offset) ;
						sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;
						exit (0) ;
					}
				}
			}

			if (FD_ISSET (serverSock, &oddfds))
			{
				while (recvfrom(serverSock , pkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, &slen) != -1)
				{
					// ACK packet
					ackPkt = pkt ;
					printf ("%d : ACK\n", ackPkt->offset) ;
					sendto (relayOddSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&clientAddr, slen) ;
				}
			}
		
			FD_ZERO (&oddfds) ;
			FD_SET (relayOddSock, &oddfds) ;
			FD_SET (serverSock, &oddfds) ;
		}
		close (relayOddSock) ;
	}
}