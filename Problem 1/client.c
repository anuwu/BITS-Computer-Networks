#include<stdlib.h> //exit(0);
#include<stdio.h> //printf
#include<string.h> //memset
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "dataDef.h"

#define BUFLEN 2  //Max length of buffer
#define SERVER_PORT 8888  // The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
	data *datCache [10] ;

	if (!fork())
	{
	    struct sockaddr_in server_addr;
	    int sockfd, i, slen=sizeof(server_addr);
	    //char *serialDat ;
	    char ackBuf [4] ;
	    char closeChar ;
	    data *datBuf ;
	    data *ackPkt = (data *) malloc (sizeof(data)) ;
		ackPkt->pktType = DATA ;
	    
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");
	 
	    memset((char *) &server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    server_addr.sin_port = htons(SERVER_PORT);
	    server_addr.sin_addr.s_addr = inet_addr("192.168.1.106");
	     
	    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	    	die ("Connection even failed!\n") ;

	    for (int i = 0 ; i < 10 ; i += 2)
	    {
	    	datBuf = (data *) malloc (sizeof(data)) ;
	    	datBuf->payload = PACKET_SIZE ;
	    	datBuf->offset = i*PACKET_SIZE ;
	    	datBuf->last = NO ;
	    	datBuf->pktType = DATA ;
	    	datBuf->channel = EVEN ;
	    	strcpy (datBuf->stuff , "abcd") ;
	    	datCache[i] = datBuf ;

	    	printf ("%d : SENDING\n", i) ;
	    	ackPkt->pktType = DATA ;
	        while (ackPkt->pktType != ACK)
	        {
	        	send (sockfd, (char *)datBuf, sizeof(data), 0) ;
	        	recv (sockfd, ackPkt, sizeof(data), 0) ;

	        	if (ackPkt->pktType == ACK)
	        		printf ("%d : TIMEOUT\n", i) ;
	        }

	        printf ("%d : ACK\n", i) ;
	        ackPkt->pktType = DATA ;
	    }

	    close(sockfd);
	}
	else
	{
		struct sockaddr_in server_addr;
	    int sockfd, i, slen=sizeof(server_addr);
	    char numDat[BUFLEN] ;
	    char ackBuf [4] ;
	    char closeChar ;
	    data *datBuf ;
	   	data *ackPkt = (data *) malloc (sizeof(data)) ;
		ackPkt->pktType = DATA ;
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");
	 
	    memset((char *) &server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    server_addr.sin_port = htons(SERVER_PORT);
	    server_addr.sin_addr.s_addr = inet_addr("192.168.1.106");
	     
	    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	    	die ("Connection odd failed!\n") ;

	    for (int i = 1 ; i < 10 ; i += 2)
	    {
	    	datBuf = (data *) malloc (sizeof(data)) ;
	    	datBuf->payload = PACKET_SIZE ;
	    	datBuf->offset = i*PACKET_SIZE ;
	    	datBuf->last = (i == 9)?YES:NO ;
	    	datBuf->pktType = DATA ;
	    	datBuf->channel = ODD ;
	    	datCache[i] = datBuf ;
	    	strcpy (datBuf->stuff , "efgh") ;

	    	printf ("\t%d : SENDING\n", i) ;
	    	ackPkt->pktType = DATA ;
	        while (ackPkt->pktType != ACK)
	        {
	        	send (sockfd, (char *)datBuf, sizeof(data), 0) ;
	        	recv (sockfd, ackPkt, sizeof(data), 0) ;

	        	if (ackPkt->pktType != ACK)
	        		printf ("\t%d : TIMEOUT\n", i) ;
	        }

	        printf ("\t%d : ACK\n", i) ;
	        ackPkt->pktType = DATA ;
	    }

	    close(sockfd);

	}
    return 0;
}