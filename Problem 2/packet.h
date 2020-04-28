#define PACKET_SIZE 100
#define WINDOW_SIZE 4
#define DROP 0.5
#define TIMEOUT 2

#define SERVER_PORT 8888
#define RELAY_EVEN_PORT 9000
#define RELAY_ODD_PORT 9001

typedef struct _dat data ;

typedef enum _isLast
{
	YES, NO
} isLast ;

typedef enum _packetType
{
	DATA, ACK
} packetType ;

typedef enum _channelID
{
	EVEN, ODD
} channelID ;

struct _dat
{
	int payload ;
	int offset ;
	isLast last ;
	packetType pktType ;
	char stuff [PACKET_SIZE+1] ;
} ;

char* isLastToString (isLast i) ;
char* packetTypeToString (packetType pkt) ;
char* channelIDToString (channelID cid) ;
int setSockAddr (struct sockaddr_in *server_addr, int relay_port) ;
int setSockAddrBind (struct sockaddr_in *server_addr, int relay_port) ;