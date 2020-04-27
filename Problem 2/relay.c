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

void resetTimeout (int serverSock, fd_set *serverfd, struct timeval *timeout)
{
	FD_ZERO (serverfd) ;
	FD_SET (serverSock, serverfd) ;
	timeout->tv_sec = TIMEOUT ;
	timeout->tv_usec = 0 ;
}

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE ;
	struct sockaddr_in serverAddr ;
	int serverSock ;

	data *ackPkt = (data *) malloc (sizeof (data)) ;
	serverSock = setSockAddr (&serverAddr, SERVER_PORT) ;
	fd_set serverfd ;
	FD_ZERO (&serverfd) ;
	FD_SET (serverSock, &serverfd) ;

	if (fork())
	{
		struct sockaddr_in relayEvenAddr, serverAckAddr, clientAddr ; 
		int relayEvenSock, new_socket, clientSock, slen = sizeof(struct sockaddr_in ) ;
		int i, max_sd, sd, valread, disconnect = 0 ;
		double delay ;

		struct timeval timeout = {TIMEOUT,0} ;

		data *datBuf = (data *) malloc (sizeof (data));
		relayEvenSock = setSockAddrBind (&relayEvenAddr, RELAY_EVEN_PORT) ;

		printf("RELAY_EVEN : Waiting for connection\n"); 
		while (TRUE)
		{
			// recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &serverAddr, &slen)
			if ((valread = recvfrom(relayEvenSock , datBuf, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen)) == -1) 
			{ 						
				printf ("RELAY_EVEN : Closed client side\n") ;
				disconnect++ ;
			} 	
			else
			{
				delay = getDelay () ;
				if (!fork())
				{
					sleep (delay/1000) ;

					printf ("RELAY %d : RECEIVED FROM CLIENT SEQ NO %d\n", 0, datBuf->offset) ;
					sendto (serverSock, datBuf, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;

					select(serverSock + 1 , &serverfd , NULL , NULL , &timeout); 
					if (FD_ISSET(serverSock, &serverfd))
					{
						recvfrom(serverSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &serverAckAddr, &slen) ;
						printf ("RELAY %d : RECEIVED ACK FROM SERVER WITH SEQ NO %d\n", 0, ackPkt->offset) ;

						resetTimeout (serverSock, &serverfd, &timeout) ;

						sendto (serverSock, ackPkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, slen) ;
					}
					else
						printf ("RELAY %d : TIMEOUT FOR PACKET WITH SEQ NO %d\n",0, datBuf->offset) ;

					exit (0) ;
				}

				//printf ("%d : %d \n", 0 , datBuf->offset) ;
			}

			if (disconnect == 1)
				break ;
		}

		close (relayEvenSock) ;
	}
	else
	{
		struct sockaddr_in relayOddAddr, serverAckAddr, clientAddr ; 
		int relayOddSock, new_socket, clientSock, slen = sizeof(struct sockaddr_in) ;
		int i, max_sd, sd, valread, disconnect = 0, activity ;
		double delay ;

		struct timeval timeout = {TIMEOUT,0} ;

		data *datBuf = (data *) malloc (sizeof(data));
		relayOddSock = setSockAddrBind (&relayOddAddr, RELAY_ODD_PORT) ;
		
		printf("RELAY_ODD : Waiting for connection\n"); 

		while (TRUE)
		{
			// recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &serverAddr, &slen)
			if ((valread = recvfrom(relayOddSock , datBuf, sizeof(data), 0, (struct sockaddr *) &clientAddr, &slen)) == -1) 
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

					printf ("RELAY %d : RECEIVED FROM CLIENT SEQ NO %d\n", 1, datBuf->offset) ;
					sendto (serverSock, datBuf, sizeof(data), 0, (struct sockaddr *) &serverAddr, slen) ;

					select(serverSock + 1 , &serverfd , NULL , NULL , &timeout); 

					if (FD_ISSET(serverSock, &serverfd))
					{
						recvfrom(serverSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &serverAckAddr, &slen) ;
						printf ("RELAY %d : RECEIVED ACK FROM SERVER WITH SEQ NO %d\n", 1, ackPkt->offset) ;

						resetTimeout (serverSock, &serverfd, &timeout) ;

						sendto (serverSock, ackPkt, sizeof(data), 0, (struct sockaddr *) &clientAddr, slen) ;
					}
					else
						printf ("RELAY %d : TIMEOUT FOR PACKET WITH SEQ NO %d\n",0, datBuf->offset) ;

					exit (0) ;
				}
			}

			if (disconnect == 1)
				break ;
		}

		close (relayOddSock) ;
	}
}
