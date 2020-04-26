#include<stdlib.h> //exit(0);
#include<stdio.h> //printf
#include<string.h> //memset
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 2  //Max length of buffer
#define SERVER_PORT 8888  // The port on which to send data


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
	if (!fork())
	{
	    struct sockaddr_in server_addr;
	    int sockfd, i, slen=sizeof(server_addr);
	    char numDat[BUFLEN] ;
	    char ackBuf [4] ;
	    char closeChar ;
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");
	 
	    memset((char *) &server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    server_addr.sin_port = htons(SERVER_PORT);
	    server_addr.sin_addr.s_addr = inet_addr("192.168.1.106");
	     
	    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	    	die ("Connection even failed!\n") ;
	    else
	    	printf ("Connection even success\n") ;

	    for (int i = 0 ; i < 10 ; i += 2)
	    {
	        sprintf (numDat, "%d", i) ;
	        write (sockfd, numDat, BUFLEN) ;

	        ackBuf[read (sockfd, ackBuf, 4)] = '\0' ;
	        printf ("%d : %s\n", i, ackBuf) ;
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
	 
	    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        die("socket\n");
	 
	    memset((char *) &server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    server_addr.sin_port = htons(SERVER_PORT);
	    server_addr.sin_addr.s_addr = inet_addr("192.168.1.106");
	     
	    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	    	die ("Connection odd failed!\n") ;
	    else
	    	printf ("Connection odd success\n") ;

	    for (int i = 1 ; i < 10 ; i += 2)
	    {
	        sprintf (numDat, "%d", i) ;
	        write (sockfd, numDat, BUFLEN) ;

	        ackBuf[read (sockfd, ackBuf, 4)] = '\0' ;
	        printf ("\t%d : %s\n", i, ackBuf) ;
	    }

	    close(sockfd);

	}
    return 0;
}