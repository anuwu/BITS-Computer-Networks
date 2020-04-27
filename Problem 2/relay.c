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

		printf("RELAY_EVEN : Waiting for connection\n"); 
		while (TRUE)
		{
			if (valread = recvfrom(relayEvenSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)) 
			{
				delay = getDelay () ;
				if (!fork())
				{
					sleep (delay/1000) ;
					printf ("%d : %d\n", 0, datPkt->offset) ;
					//sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;

					ackPkt->offset = datPkt->offset ;
					//sendto (serverSock, ackPkt, sizeof(data), 0, (struct sockaddr *) &)
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
		relayOddSock = setSockAddrBind (&relayOddAddr, RELAY_ODD_PORT) ;
		
		printf("RELAY_ODD : Waiting for connection\n"); 

		while (TRUE)
		{
			// recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &serverAddr, &slen)
			if ((valread = recvfrom(relayOddSock , datPkt, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)) == -1) 
			{ 						
				printf ("RELAY_ODD : Closed client side\n") ;
				disconnect++ ;
			} 	
			else
			{
				delay = getDelay () ;
				if (!fork())
				{
					sleep (delay/1000) ;
					printf ("\t%d : %d\n", 1, datPkt->offset) ;
					sendto (serverSock, datPkt, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;
					exit (0) ;
				}
			}

			if (disconnect == 1)
				break ;
		}

		close (relayOddSock) ;
	}
}
