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
	    int sockfd, i, slen=sizeof(server_addr), activity ;
	    struct timeval timeout = {2,0} ;

	    data *datBuf ;
	    data *ackPkt = (data *) malloc (sizeof(data)) ;
		ackPkt->pktType = DATA ;
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");

	    fd_set readfds; 
	    FD_ZERO(&readfds); 
	    FD_SET (sockfd, &readfds) ;
	 
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
	        while (1)
	        {
	        	send (sockfd, (char *)datBuf, sizeof(data), 0) ;
	        	activity = select (sockfd + 1, &readfds, NULL, NULL, &timeout) ;

	        	if (FD_ISSET (sockfd, &readfds))
	        	{
	        		read (sockfd, (char *)ackPkt, sizeof(data)) ;	        		

	        		if (ackPkt->pktType != ACK)
	        			printf ("%d : TIMEOUT %d\n", i, timeout.tv_sec) ;
	        		else
	        		{
	        			printf ("%d : ACK\n", i, timeout.tv_sec) ;
	        			ackPkt->pktType = DATA ;
	        			break ;
	        		}		
	        	}

	        	timeout.tv_sec = 2 ;
	        	timeout.tv_usec = 0 ;
	        }
	        ackPkt->pktType = DATA ;
	    }

	    close(sockfd);
	}
	else
	{
		struct sockaddr_in server_addr;
	    int sockfd, i, slen=sizeof(server_addr), activity;
	    struct timeval timeout = {2,0} ;

	    data *datBuf ;
	   	data *ackPkt = (data *) malloc (sizeof(data)) ;
		ackPkt->pktType = DATA ;
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");

	    fd_set readfds; 
	    FD_ZERO(&readfds); 
	    FD_SET (sockfd, &readfds) ;
	 
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
	        while (1)
	        {
	        	send (sockfd, (char *)datBuf, sizeof(data), 0) ;
	        	activity = select (sockfd + 1 , &readfds, NULL, NULL, &timeout) ;

	        	if (FD_ISSET (sockfd, &readfds))
	        	{
	        		read (sockfd, (char *)ackPkt, sizeof(data)) ;

	        		if (ackPkt->pktType != ACK)
	        			printf ("\t%d : TIMEOUT\n", i) ;
	        		else
	        		{
	        			printf ("\t%d : ACK\n", i) ;
	        			ackPkt->pktType = DATA ;
	        			break ;
	        		}		
	        	}

				timeout.tv_sec = 2 ;
	        	timeout.tv_usec = 0 ;
	        }
	        ackPkt->pktType = DATA ;
	    }

	    close(sockfd);

	}
    return 0;
}