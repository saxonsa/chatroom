
#define		DEFAULT_PORT		5019	// Server Listening Port


/*************************************************************/
/************* structure to store socket *********************/

WSADATA	wsaData = { 0 };	
SOCKET sock = INVALID_SOCKET;		// master server socket
SOCKET msg_sock = INVALID_SOCKET;	// slave server socket

/*************************************************************/


struct sockaddr_in local = { 0 };		// server end address
struct sockaddr_in client_addr = { 0 };	// client end address


char szBuff[1000];
int msg_len;
int addr_len;

