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

int setRelay (struct sockaddr_in *server_addr, fd_set* readfds, int receiver_port)
{
	int sockfd ;

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        die("socket\n");

    FD_ZERO(readfds); 
    FD_SET (sockfd, readfds) ;
 
    memset((char *) server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(receiver_port);
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
     
    if (connect (sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0)
    	die ("Connection failed!\n") ;
    else
    	printf ("Connecton successful!\n") ;

	return sockfd ;
}


int resendPrep (fd_set *readfds, int relayEvenSock, int relayOddSock, struct timeval *timeout, int *sndCount)
{
    (*sndCount)++ ;
	FD_ZERO(readfds); 
	FD_SET (relayEvenSock, readfds) ;
	FD_SET (relayOddSock, readfds) ;
	timeout->tv_sec = TIMEOUT ;
	timeout->tv_usec = 0 ;
}

int main(void)
{
	struct sockaddr_in relayEvenAddr, relayOddAddr ;
    int relayEvenSock, relayOddSock, i, slen = sizeof(struct sockaddr_in) ;
    int sndCount, fileSize, noPkts, bytesRead ;
    struct timeval timeout = {TIMEOUT,0} ;
    fd_set readfds ;

    data *datBuf, *ackPkt ;
	data *datCache [10] ;
	ackPkt = (data *) malloc (sizeof(data)) ;
	ackPkt->pktType = DATA ;

	FILE *fp = fopen ("input.txt", "r") ;

	fseek (fp, 0, SEEK_END) ;
	fileSize = ftell (fp) - 1 ;
	noPkts = fileSize/PACKET_SIZE + ((fileSize % PACKET_SIZE)?1:0) ;
	fseek (fp, 0, SEEK_SET) ;

	printf ("Size of file = %d\n", fileSize) ;
	printf ("No of packets = %d\n", noPkts) ;

	relayEvenSock = setRelay (&relayEvenAddr, &readfds, RELAY_EVEN_PORT) ;
	relayOddSock = setRelay (&relayOddAddr, &readfds, RELAY_ODD_PORT) ;

	
    for (int i = 0 ; i < noPkts ; i++)
    {
    	datBuf = (data *) malloc (sizeof(data)) ;
    	datBuf->offset = i*PACKET_SIZE ;
    	datBuf->last = (i == noPkts-1)?YES:NO ;
    	datBuf->pktType = DATA ;
    	datBuf->channel = EVEN ;

    	fseek (fp, i*PACKET_SIZE, SEEK_SET) ;
    	bytesRead = (int)fread (datBuf->stuff , sizeof(char), PACKET_SIZE, fp) ;
    	datBuf->stuff[bytesRead] = '\0' ;
    	datBuf->payload = bytesRead ;
    	datCache[i] = datBuf ;

    	send (i % 2 ? relayOddSock : relayEvenSock, datBuf, sizeof(data), 0) ;
    	printf ("%d : %d %s\n", i, datBuf->offset, datBuf->stuff) ;
    }
    

    fclose (fp) ;
    close(relayEvenSock) ;
    close(relayOddSock) ;
	
	return 0 ;
}
