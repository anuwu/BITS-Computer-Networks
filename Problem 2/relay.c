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

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE ;

	if (fork())
	{
		struct sockaddr_in relayEvenAddr, otherAddr ; 
		int relayEvenSock, new_socket, clientSock, slen = sizeof(otherAddr) ;
		int i, max_sd, sd, valread, disconnect = 0, activity ;

		
		data *datBuf = (data *) malloc (sizeof(data));

		if( (relayEvenSock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP)) == 0) 
		{ 
			perror("Even relay server socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		memset((char *) &relayEvenAddr, 0, sizeof(relayEvenAddr));

		relayEvenAddr.sin_family = AF_INET; 
		relayEvenAddr.sin_addr.s_addr = htonl (INADDR_ANY) ; 
		relayEvenAddr.sin_port = htons(RELAY_EVEN_PORT) ; 

		if (bind(relayEvenSock, (struct sockaddr *)&relayEvenAddr, sizeof(relayEvenAddr))<0) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 
			
		printf("RELAY_EVEN : Waiting for connection\n"); 

		while (TRUE)
		{
			// recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)
			if ((valread = recvfrom(relayEvenSock , datBuf, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)) == -1) 
			{ 						
				printf ("RELAY_EVEN : Closed client side\n") ;
				disconnect++ ;
			} 	
			else
				printf ("%d : %d\n", 0, datBuf->offset) ;

			if (disconnect == 1)
				break ;
		}

		close (relayEvenSock) ;
	}
	else
	{
		struct sockaddr_in relayOddAddr, otherAddr ; 
		int relayOddSock, new_socket, clientSock, slen = sizeof(otherAddr) ;
		int i, max_sd, sd, valread, disconnect = 0, activity ;

		
		data *datBuf = (data *) malloc (sizeof(data));

		if( (relayOddSock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP)) == 0) 
		{ 
			perror("RELAY_ODD : server socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		memset((char *) &relayOddAddr, 0, sizeof(relayOddAddr));

		relayOddAddr.sin_family = AF_INET; 
		relayOddAddr.sin_addr.s_addr = htonl (INADDR_ANY) ; 
		relayOddAddr.sin_port = htons(RELAY_ODD_PORT) ; 

		if (bind(relayOddSock, (struct sockaddr *)&relayOddAddr, sizeof(relayOddAddr))<0) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 
			
		printf("RELAY_ODD : Waiting for connection\n"); 

		while (TRUE)
		{
			// recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)
			if ((valread = recvfrom(relayOddSock , datBuf, sizeof(data), 0, (struct sockaddr *) &otherAddr, &slen)) == -1) 
			{ 						
				printf ("RELAY_ODD : Closed client side\n") ;
				disconnect++ ;
			} 	
			else
				printf ("\t%d : %d\n", 1, datBuf->offset) ;

			if (disconnect == 1)
				break ;
		}

		close (relayOddSock) ;
	}
}
