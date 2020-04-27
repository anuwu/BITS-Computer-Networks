#include<stdlib.h> //exit(0);
#include<stdio.h> //printf
#include<string.h> //memset
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "packet.h"

char* isLastToString (isLast i)
{
	switch (i)
	{
		case YES :
			return "YES" ;
		case NO :
			return "NO" ;
		default :
			return "islast_ERROR" ;
	}
}

char* packetTypeToString (packetType pkt)
{
	switch (pkt)
	{
		case DATA :
			return "DATA" ;
		case ACK :
			return "ACK" ;
		default :
			return "packetType_ERROR" ;	
	}
}

char* channelIDToString (channelID cid)
{
	switch (cid)
	{
		case EVEN :
			return "EVEN" ;
		case ODD :
			return "ODD" ;
		default :
			return "channelID_ERROR" ;
	}
}

int setSockAddr (struct sockaddr_in *server_addr, int relay_port)
{
	int sockfd ;

	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf ("Socket creation error\n") ;
        exit (0) ;
	}
 
    memset((char *) server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(relay_port);
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
     
	return sockfd ;
}

int setSockAddrBind (struct sockaddr_in *server_addr, int relay_port)
{
	int sockfd ;

	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf ("Socket creation error\n") ;
        exit (0) ;
	}
 
    memset((char *) server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(relay_port);
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in))<0) 
	{ 
		perror("Bind failed"); 
		exit(EXIT_FAILURE); 
	}
     
	return sockfd ;
}

void setSock (FILE *fpout, data *datBuf)
{
	fseek (fpout, datBuf->offset, SEEK_SET) ;
	fwrite (datBuf->stuff, sizeof(char), datBuf->payload, fpout) ;
}