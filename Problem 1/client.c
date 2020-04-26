/* Simple udp client */
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFLEN 512  //Max length of buffer

#define PORT 8888  // The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in server_addr;
    int sockfd, i, slen=sizeof(server_addr);
    char buf[BUFLEN];
    char message[BUFLEN];
    char closeChar ;
 
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        die("socket\n");
 
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.106");
    // 192.168.1.106
     
    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    	die ("connection failed!\n") ;

    buf[read (sockfd, buf, BUFLEN)] = '\0' ;
    printf ("%d : %s", strlen(buf), buf) ;

    printf ("Write your message : ") ;
    scanf ("%s", message) ;
    write (sockfd, message, BUFLEN) ;

    buf[read (sockfd, buf, BUFLEN)] = '\0' ;
    printf ("Echoed message : %s\n", buf) ;

    scanf ("%c", &closeChar) ;
    printf ("Do you want to close? Press any key : ") ;
    scanf ("%c", &closeChar) ;

    close(sockfd);
    return 0;
}

