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

	data *datPkt = (data *) malloc (sizeof (data)) ;
	data *ackPkt = (data *) malloc (sizeof (data)) ;

	if (fork())
	{
		int evenCloseFlag = 0 ;
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
				while (recvfrom(relayEvenSock , datPkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen) != -1)
				{
					if (datPkt->pktType == CLOSE)
					{
						evenCloseFlag = 1 ;
						break ;
					}

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

			if (evenCloseFlag)
				break ;


			if (FD_ISSET (serverSock, &evenfds))
			{
				while (recvfrom(serverSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, &slen) != -1)
				{
					printf ("%d : ACK\n", ackPkt->offset) ;
					sendto (relayEvenSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&clientAddr, slen) ;
				}
			}

			FD_ZERO (&evenfds) ;
			FD_SET (relayEvenSock, &evenfds) ;
			FD_SET (serverSock, &evenfds) ;
		}

		printf ("Even relay closing\n") ;
		close (relayEvenSock) ;
	}
	else
	{
		int oddCloseFlag = 0 ;
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
				while (recvfrom(relayOddSock , datPkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen) != -1)
				{
					if (datPkt->pktType == CLOSE)
					{
						oddCloseFlag = 1 ;
						break ;
					}

					delay = getDelay () ;
					if (!fork())
					{
						// 
						sleep (delay/1000) ;
						printf ("%d : RECV\n", datPkt->offset) ;
						sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;
						exit (0) ;
					}
				}
			}

			if (oddCloseFlag)
				break ;

			if (FD_ISSET (serverSock, &oddfds))
			{
				while (recvfrom(serverSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, &slen) != -1)
				{
					printf ("%d : ACK\n", ackPkt->offset) ;
					sendto (relayOddSock, ackPkt, sizeof(data), 0, (struct sockaddr *)&clientAddr, slen) ;
				}
			}
		
			FD_ZERO (&oddfds) ;
			FD_SET (relayOddSock, &oddfds) ;
			FD_SET (serverSock, &oddfds) ;
		}

		printf ("Odd relay closing\n") ;
		close (relayOddSock) ;
	}
}