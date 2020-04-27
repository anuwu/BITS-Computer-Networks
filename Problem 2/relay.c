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
#include "packet.h"
	
#define TRUE 1 
#define FALSE 0 

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE ;

	if (fork())
	{
		int relayEvenSock, new_socket, addrlen, client_socket[2] ;
		int i, max_sd, sd, valread, disconnect = 0, max_clients = 2, activity ;

		data *datBuf = (data *) malloc (sizeof(data));
		struct sockaddr_in address; 
		fd_set readfds; 

		client_socket[0] = 0 ;
		client_socket[1] = 0 ;

		if( (relayEvenSock = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
		{ 
			perror("Even relay server socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		if(setsockopt(relayEvenSock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) 
		{ 
			perror("setsockopt failed"); 
			exit(EXIT_FAILURE); 
		} 

		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons(RELAY_EVEN_PORT) ; 

		if (bind(relayEvenSock, (struct sockaddr *)&address, sizeof(address))<0) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 

		if (listen(relayEvenSock, 2) < 0) 
		{ 
			perror("listen failed"); 
			exit(EXIT_FAILURE); 
		} 
			
		addrlen = sizeof(address); 
		printf("EVEN : Waiting for connection\n"); 

		while (TRUE)
		{
			FD_ZERO(&readfds); 
			FD_SET(relayEvenSock, &readfds); 
			max_sd = relayEvenSock ; 
				
			for (i = 0 ; i < max_clients ; i++) 
			{ 
				sd = client_socket[i]; 
				if(sd > 0) 
					FD_SET( sd , &readfds); 
					
				if(sd > max_sd) 
					max_sd = sd; 
			}

			//printf ("RELAY_EVEN : Waiting for activity\n") ;
			activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
			//printf ("RELAY_EVEN : Activity detected\n") ;

			if (FD_ISSET(relayEvenSock, &readfds)) 
			{ 
				if ((new_socket = accept(relayEvenSock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
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
			else if (FD_ISSET(client_socket[0], &readfds))
			{
				if ((valread = read(client_socket[0] , datBuf, sizeof(data))) == 0) 
				{ 						
					close (client_socket[0]);
					printf ("RELAY_EVEN : Closed client side\n") ;
					client_socket[0] = 0; 
					disconnect++ ;
				} 	
				else
					printf ("%d : %d\n", 0, datBuf->offset) ;
	
			}
			else if (FD_ISSET(client_socket[1], &readfds))
				printf ("RELAY_EVEN : Real server side activity\n") ;
			else
				printf ("RELAY_EVEN : Invalid activity\n") ;


			if (disconnect == 1)
				break ;
		}

		close (relayEvenSock) ;
	}
	else
	{
		int relayOddSock, new_socket, addrlen, client_socket[2] ;
		int i, max_sd, sd, valread, disconnect = 0 , max_clients = 2, activity ;

		data *datBuf = (data *) malloc (sizeof(data));
		struct sockaddr_in address; 
		fd_set readfds; 

		client_socket[0] = 0 ;
		client_socket[1] = 0 ;

		if((relayOddSock = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
		{ 
			perror("Even relay server socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		if(setsockopt(relayOddSock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) 
		{ 
			perror("setsockopt failed"); 
			exit(EXIT_FAILURE); 
		} 

		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons(RELAY_ODD_PORT) ; 

		if (bind(relayOddSock, (struct sockaddr *)&address, sizeof(address))<0) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 

		if (listen(relayOddSock, 2) < 0) 
		{ 
			perror("listen failed"); 
			exit(EXIT_FAILURE); 
		} 
			
		addrlen = sizeof(address); 
		printf("ODD : Waiting for connection\n"); 

		while (TRUE)
		{
			FD_ZERO(&readfds); 
			FD_SET(relayOddSock, &readfds); 
			max_sd = relayOddSock; 
				
			for (i = 0 ; i < max_clients ; i++) 
			{ 
				sd = client_socket[i]; 
				if(sd > 0) 
					FD_SET( sd , &readfds); 
					
				if(sd > max_sd) 
					max_sd = sd; 
			}

			//printf ("RELAY_-ODD : Waiting for activity\n") ;
			activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
			//printf ("RELAY_ODD : Activity detected\n") ;

			if (FD_ISSET(relayOddSock, &readfds)) 
			{ 
				if ((new_socket = accept(relayOddSock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
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
			else if (FD_ISSET(client_socket[0], &readfds))
			{
				if ((valread = read(client_socket[0] , datBuf, sizeof(data))) == 0) 
				{ 						
					close (client_socket[0]); 
					printf ("RELAY_ODD : Closed client side\n") ;
					client_socket[0] = 0; 
					disconnect++ ;
				} 	
				else
					printf ("\t%d : %d\n", 1, datBuf->offset) ;
	
			}
			else if (FD_ISSET(client_socket[1], &readfds))
				printf ("RELAY_ODD : Real server side activity\n") ;
			else
				printf ("RELAY_ODD : Invalid activity\n") ;


			if (disconnect == 1)
				break ;
		}

		close (relayOddSock) ;
	}
}
