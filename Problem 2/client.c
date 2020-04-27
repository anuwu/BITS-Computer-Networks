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

void sendInWindow (int *windowPktStat, int *windowPktOffset, data **datCache, int windowStart, int *latestPkt, int noPkts, int relayEvenSock, int relayOddSock, struct sockaddr_in* relayEvenAddr, struct sockaddr_in* relayOddAddr, FILE *fp)
{
    int i , bytesRead ;
    data *datPkt ;

    for (i = 0 ; i < WINDOW_SIZE ; i++)
    {
        if (!windowPktStat[i])
        {
            windowPktOffset[i] = (windowStart + i) * PACKET_SIZE ;
            datPkt = (data *) malloc (sizeof (data)) ;
            datPkt->offset = (windowStart + i)*PACKET_SIZE ;
            datPkt->last = (windowStart + i == noPkts-1)?YES:NO ;
            datPkt->pktType = DATA ;

            fseek (fp, (windowStart + i)*PACKET_SIZE, SEEK_SET) ;
            bytesRead = (int)fread (datPkt->stuff , sizeof(char), PACKET_SIZE, fp) ;
            datPkt->stuff[bytesRead] = '\0' ;
            datPkt->payload = bytesRead ;
            datCache[i] = datPkt ;

            sendto ((windowStart + i) % 2 ? relayOddSock : relayEvenSock, datPkt, sizeof(data), 0, (struct sockaddr *)((windowStart + i) % 2 ? relayOddAddr : relayEvenAddr), sizeof(struct sockaddr_in)) ;
             printf ("%d : %d\n", (windowStart + i), datPkt->offset) ;

            windowPktStat[i] = 1 ;
            if (windowStart + i > *latestPkt)
                *latestPkt = windowStart + i ;
        }
    }

}

int unsentInWindow (int *windowPktStat)
{
    int i ;
    for (i = 0 ; i < WINDOW_SIZE ; i++)
        if (!windowPktStat[i])
            return 1 ;

    return 0 ;
}

void initWindow (int *windowPktOffset, int *windowPktStat)
{
    for (int i = 0 ; i < WINDOW_SIZE ; i++)
    {
        windowPktOffset[i] = -1 ;
        windowPktStat[i] = 0 ;
    }
}

int main(void)
{
	struct sockaddr_in relayEvenAddr, relayOddAddr, otherAddr ;
    int relayEvenSock, relayOddSock, i, slen = sizeof(struct sockaddr_in) ;
    int sndCount, fileSize, noPkts, bytesRead ;
    struct timeval timeout = {TIMEOUT,0} ;
    data *datPkt, *ackPkt ;
	data *datCache [WINDOW_SIZE] ;

    /* ------------------------------------------------------ */

	FILE *fp = fopen ("input.txt", "r") ;
	fseek (fp, 0, SEEK_END) ;
	fileSize = ftell (fp)  ;
	noPkts = fileSize/PACKET_SIZE + ((fileSize % PACKET_SIZE)?1:0) ;
	fseek (fp, 0, SEEK_SET) ;
	printf ("Size of file = %d\n", fileSize) ;
	printf ("No of packets = %d\n", noPkts) ;

    /* ------------------------------------------------------ */

    int maxfd ;
	relayEvenSock = setSockAddr (&relayEvenAddr, RELAY_EVEN_PORT) ;
	relayOddSock = setSockAddr (&relayOddAddr, RELAY_ODD_PORT) ;
    fd_set relayfds ;
    FD_ZERO (&relayfds) ;
    FD_SET (relayEvenSock, &relayfds) ;
    FD_SET (relayOddSock, &relayfds) ;

    maxfd = relayEvenSock > relayOddSock ? relayEvenSock : relayOddSock ;

    /* ------------------------------------------------------ */

    int windowStart = 0, latestPkt = -1 , windowSize = WINDOW_SIZE ;
    int windowPktOffset[WINDOW_SIZE] ;
    int windowPktStat[WINDOW_SIZE] ;
    initWindow (windowPktOffset, windowPktStat) ;

	
    while (1)
    {
        if (unsentInWindow(windowPktStat))
            sendInWindow (windowPktStat, windowPktOffset, datCache, windowStart, &latestPkt, noPkts, relayEvenSock, relayOddSock, &relayEvenAddr, &relayOddAddr, fp) ;

        select (maxfd + 1 , &relayfds, NULL, NULL, NULL) ;
        if (FD_ISSET (relayEvenSock))
        {
            ackPkt = recvfrom ()
        }

        break ;
    }

    printf ("ALL STATUSES - \n") ;
    for (i = 0 ; i < WINDOW_SIZE ; i++)
        printf ("%d\n" ,windowPktStat[i]) ;

    	

    fclose (fp) ;
    close(relayEvenSock) ;
    close(relayOddSock) ;

    exit (0) ;
}
