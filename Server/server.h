
#define		DEFAULT_PORT		5019	// server Listening Port


/*************************************************************/
/************* structure to store socket *********************/

WSADATA	wsaData = { 0 };	
SOCKET sock = INVALID_SOCKET;		// master server socket
SOCKET msg_sock = INVALID_SOCKET;	// client socket

/*************************************************************/


struct sockaddr_in local = { 0 };		// server end address
struct sockaddr_in client_addr = { 0 };	// client end address


char szBuff[1000];
int msg_len;
int addr_len;

int connecting = 0; // cuurent number of clients connected with server
const int MAX_ALLOWED = 3; // the max number of clients supported by server 


// client structure
// an array to store all the clients connecting with server
typedef struct _Client {
	int fd;
	char buff[1000]; // Client buffer
	char name[32]; // Client name
	char IP[20]; // Client IP
	struct sockaddr_in client_addr; // Client IP address
	SOCKET client_socket; // Client socket
}Client;

Client clients[MAX_ALLOWED] = { 0 };

