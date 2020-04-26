//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
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
#include<time.h> 
#include "dataDef.h"
	
#define TRUE 1 
#define FALSE 0 
#define PORT 8888
	
double getRand ()
{
	return rand() / (double)(RAND_MAX) ;
}

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30] , 
		max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
	data *datBuf = (data *) malloc (sizeof(data));

	data *ackPkt = (data *) malloc (sizeof(data)) ;
	ackPkt->pktType = ACK ;
		
	//char buffer[1025]; //data buffer of 1K 
	srand (time(0)) ;
		
	//set of socket descriptors 
	fd_set readfds; 
		
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
		client_socket[i] = 0; 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("Server socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT) ; 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Waiting for connection\n"); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 2) < 0) 
	{ 
		perror("listen failed"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	int disconnect = 0 ;
		
	while(TRUE) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
			perror("select error"); 

		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
			{ 
				perror("accept error"); 
				exit(EXIT_FAILURE); 
			} 

			for (i = 0; i < max_clients; i++) 
			{ 
				if(!client_socket[i]) 
				{ 
					client_socket[i] = new_socket; 
					break; 
				} 
			} 
		} 
		else //else its some IO operation on some other socket 
		{
			for (i = 0; i < max_clients; i++) 
			{ 
				sd = client_socket[i]; 
					
				if (FD_ISSET( sd , &readfds)) 
				{ 
					if ((valread = read(sd , datBuf, sizeof(data))) == 0) 
					{ 						
						close (sd); 
						client_socket[i] = 0; 
						disconnect++ ;
					} 	
					else
					{ 
						if (getRand () > DROP)
						{
							ackPkt->pktType = ACK ;
							printf ("%s : (%s, %s) , (%d, %d) --> %s \n", channelIDToString (datBuf->channel), packetTypeToString (datBuf->pktType), isLastToString (datBuf->last), datBuf->payload, datBuf->offset, datBuf->stuff);
							send(sd , ackPkt , sizeof(data) , 0); 
						}
						else
						{
							printf ("%d : DROP\n", datBuf->offset/4) ;
							ackPkt->pktType = DATA ;
							send (sd, ackPkt, sizeof(data), 0) ;
						}
					} 
				} 
			} 
		}
		if (disconnect == 2)
			break ;
	} 
		
	close (master_socket) ;
	return 0; 
}