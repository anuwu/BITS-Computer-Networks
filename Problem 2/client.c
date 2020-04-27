#include<stdlib.h> //exit(0);
#include<stdio.h> //printf
#include<string.h> //memset
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/wait.h>
#include "packet.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

void resetTimeout (int serverSock, fd_set *serverfd, struct timeval *timeout)
{
	FD_ZERO (serverfd) ;
	FD_SET (serverSock, serverfd) ;
	timeout->tv_sec = TIMEOUT ;
	timeout->tv_usec = 0 ;
}



int main(void)
{
	struct sockaddr_in relayEvenAddr, relayOddAddr, otherAddr ;
    int relayEvenSock, relayOddSock, i, slen = sizeof(struct sockaddr_in) ;
    int sndCount, fileSize, noPkts, bytesRead, maxfd ;
    struct timeval timeout = {TIMEOUT,0} ;

    data *datBuf, *ackPkt ;
	data *datCache [10] ;
	ackPkt = (data *) malloc (sizeof(data)) ;
	ackPkt->pktType = DATA ;

	FILE *fp = fopen ("input.txt", "r") ;

	fseek (fp, 0, SEEK_END) ;
	fileSize = ftell (fp)  ;
	noPkts = fileSize/PACKET_SIZE + ((fileSize % PACKET_SIZE)?1:0) ;
	fseek (fp, 0, SEEK_SET) ;

	printf ("Size of file = %d\n", fileSize) ;
	printf ("No of packets = %d\n", noPkts) ;

	FILE *fpout = fopen ("output.txt", "w") ;

	relayEvenSock = setSockAddr (&relayEvenAddr, RELAY_EVEN_PORT) ;
	relayOddSock = setSockAddr (&relayOddAddr, RELAY_ODD_PORT) ;

	fd_set evenfd, oddfd ;
	FD_ZERO (&evenfd) ;
	FD_SET (relayEvenSock, &evenfd) ;
	FD_ZERO (&oddfd) ;
	FD_SET (relayOddSock, &oddfd) ;

	maxfd = relayEvenSock > relayOddSock ? relayEvenSock : relayOddSock ;

	
    for (int i = 0 ; i < noPkts ; i++)
    {
    	datBuf = (data *) malloc (sizeof(data)) ;
    	datBuf->offset = i*PACKET_SIZE ;
    	datBuf->last = (i == noPkts-1)?YES:NO ;
    	datBuf->pktType = DATA ;

    	fseek (fp, i*PACKET_SIZE, SEEK_SET) ;
    	bytesRead = (int)fread (datBuf->stuff , sizeof(char), PACKET_SIZE, fp) ;
    	datBuf->stuff[bytesRead] = '\0' ;
    	datBuf->payload = bytesRead ;
    	datCache[i] = datBuf ;

    	sendto (i % 2 ? relayOddSock : relayEvenSock, datBuf, sizeof(data), 0, (struct sockaddr *)(i % 2 ? &relayOddAddr : &relayEvenAddr), slen) ;
    	printf ("CLIENT SENDING PACKET %d : SEQ NO %d\n", i, datBuf->offset) ;

    	setSock (fpout, datBuf) ;

    	free (datBuf) ;

    	/*
    	if (i%2)
    	{
	    	if (!fork())
	    	{
	    		while (1)
	    		{
		    		select (relayOddSock + 1 , &oddfd, NULL, NULL, &timeout) ;
		    		if (FD_ISSET (relayOddSock, &oddfd))
		    		{
		    			recvfrom(relayOddSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &relayOddAddr, &slen) ;
						printf ("\t%d : %d ACK\n", 0, ackPkt->offset) ;
						break ;
		    		}
		    		else
		    		{
		    			printf ("\t%d : %d NACK\n", 0, datBuf->offset) ;
		    			resetTimeout (relayOddSock, &oddfd, &timeout) ;
		    			sendto (relayOddSock, datBuf, sizeof(data), 0, (struct sockaddr *)&relayOddAddr, slen) ;
		    		}
	    		}
	    		exit (0) ;
	    	}
	    }
	    else
	    {
	    	if (!fork())
	    	{
	    		while (1)
	    		{
		    		select (relayEvenSock + 1 , &evenfd, NULL, NULL, &timeout) ;
		    		if (FD_ISSET (relayEvenSock, &evenfd))
		    		{
		    			recvfrom(relayEvenSock , ackPkt, sizeof(data), 0, (struct sockaddr *) &relayEvenAddr, &slen) ;
						printf ("%d : %d ACK\n", 0, ackPkt->offset) ;
						break ;
		    		}
		    		else
		    		{
		    			printf ("%d : %d NACK\n", 0, datBuf->offset) ;
		    			resetTimeout (relayEvenSock, &evenfd, &timeout) ;
		    			sendto (relayEvenSock, datBuf, sizeof(data), 0, (struct sockaddr *)&relayEvenAddr, slen) ;
		    		}
	    		}
	    		exit (0) ;
	    	}
	    }
	    */
    }

    fclose (fp) ;
    fclose (fpout) ;
    close(relayEvenSock) ;
    close(relayOddSock) ;

    exit (0) ;
}
