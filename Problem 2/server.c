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
	
void initRecvOffsets (int *recvOffsets)
{
	int i ;

	for (i = 0 ; i < WINDOW_SIZE ; i++)
		recvOffsets[i] = -1 ;
} 

void updateRecvOffsets (int *recvOffsets, int offset)
{
	int min = recvOffsets[0], minPos = 0, max = recvOffsets[0], i ;

	for (i = 1 ; i < WINDOW_SIZE ; i++)
	{
		if (recvOffsets[i] < min)
		{
			min = recvOffsets[i] ;
			minPos = i ;
		}

		if (recvOffsets[i] > max)
			max = recvOffsets[i] ;
	}

	if (offset > max || offset > min)
		recvOffsets[minPos] = offset ; 
}

int lastWindowAllRecv (int *recvOffsets, int lastOffset)
{
	int i, j, srch = lastOffset, srchFlag = 0 ;

	for (i = 0 ; i < WINDOW_SIZE ; i++)
	{
		srchFlag = 0 ;
		for (j = 0 ; j < WINDOW_SIZE ; j++)
		{
			if (recvOffsets[j] == srch)
			{
				srchFlag = 1 ;
				break ;
			}
		}

		if (!srchFlag)
			return 0 ;

		srch = srch - PACKET_SIZE ;
	}

	return 1 ;
}

int main(int argc , char *argv[]) 
{ 
	struct sockaddr_in serverAddr, otherAddr ;
	int serverSock, valRead, disconnected = 0, i, lastFlag = 0, lastOffset ;
	int slen = sizeof (struct sockaddr_in) ;
	int recvOffsets[WINDOW_SIZE] ;

	initRecvOffsets (recvOffsets) ;

	data *datPkt = (data *) malloc (sizeof (data)) ;
	data *ackPkt = (data *) malloc (sizeof (data)) ;
	ackPkt->pktType = ACK ;
	serverSock = setSockAddrBind (&serverAddr, SERVER_PORT) ;

	FILE *fp ;
	fp = fopen ("output.txt", "w") ;

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
				printf ("%d : RCVD\n", datPkt->offset) ;
				updateRecvOffsets (recvOffsets, datPkt->offset) ;

				fseek (fp, datPkt->offset, SEEK_SET) ;
				fwrite (datPkt->stuff, sizeof(char), datPkt->payload, fp) ;

				if (datPkt->last == YES)
				{
					lastFlag = 1 ;
					lastOffset = datPkt->offset ;
				}

				ackPkt->offset = datPkt->offset ;
				sendto (serverSock, ackPkt, sizeof (data), 0, (struct sockaddr *) &otherAddr, slen) ;
			} 
		}

		if (lastFlag && lastWindowAllRecv (recvOffsets, lastOffset))
			break ;

		FD_ZERO (&serverfd) ;
		FD_SET (serverSock, &serverfd) ;
	}

	fclose (fp) ;
	printf ("\nFile received successfully at server\n") ;
	return 0; 
}
