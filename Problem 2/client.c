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

int setRelay (struct sockaddr_in *server_addr, int relay_port)
{
	int sockfd ;

	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket\n");
 
    memset((char *) server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(relay_port);
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
     
    if (connect (sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0)
    	die ("Connection failed!\n") ;
    else
    	printf ("Connecton successful!\n") ;

	return sockfd ;
}

int main(void)
{
	struct sockaddr_in otherAddrEven, otherAddrOdd ;
    int relayEvenSock, relayOddSock, i, slen = sizeof(struct sockaddr_in) ;
    int sndCount, fileSize, noPkts, bytesRead ;
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

	relayEvenSock = setRelay (&otherAddrEven, RELAY_EVEN_PORT) ;
	relayOddSock = setRelay (&otherAddrOdd, RELAY_ODD_PORT) ;

	
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

    	sendto (i % 2 ? relayOddSock : relayEvenSock, datBuf, sizeof(data), 0, (struct sockaddr *)(i % 2 ? &otherAddrOdd : &otherAddrEven), slen) ;
    	printf ("%d : %d %d\n", i, datBuf->offset, (int)strlen(datBuf->stuff)) ;
    	free (datBuf) ;
    }

    	

    fclose (fp) ;
    close(relayEvenSock) ;
    close(relayOddSock) ;

    printf ("Closed both\n") ;
    exit (0) ;
}
