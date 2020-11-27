
#define DEFAULT_PORT	5019

WSADATA wsaData;

SOCKET connect_sock = INVALID_SOCKET;

struct sockaddr_in server_addr; // server end address

char serverIP[32] = { 0 };
char server_port[10] = { 0 };
char			*server_name = "localhost";
unsigned short	port = DEFAULT_PORT;
unsigned int	addr;

char szBuff[1000];
int msg_len;
struct hostent *hp;


// call a thread to receive message from server
// void *THRE_RECV(SOCKET ClientSocket);

