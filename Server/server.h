#define		DEFAULT_PORT		5019	// server Listening Port
#define BufferSize 2820


/*************************************************************/
/************* structure to store socket *********************/

WSADATA	wsaData = { 0 };	
SOCKET sock = INVALID_SOCKET;		// master server socket
SOCKET msg_sock = INVALID_SOCKET;	// client socket

/*************************************************************/


struct sockaddr_in local = { 0 };		// server end address
struct sockaddr_in client_addr = { 0 };	// client end address


char szBuff[BufferSize];
int msg_len;
int addr_len;

int connecting = 0; // cuurent number of clients connected with server
const int MAX_ALLOWED = 3; // the max number of clients supported by server 

//Database global variables

MYSQL mysqlConnect; 
MYSQL_RES *res;
MYSQL_FIELD *field; 
MYSQL_ROW nextRow;
int ret = 0;


// client structure
// an array to store all the clients connecting with server
typedef struct _Client {
	int fd;
	char name[100]; // Client name
	SOCKET client_socket; // Client socket
}Client;

Client clients[MAX_ALLOWED] = { 0 };

typedef struct _nameList {
	int uid;
	char name[100];
}nameList;

int onlineList_msg = 0;

typedef struct {
	char search_name[100];
	char search_time[100];
	char search_content[1000];
}search;

typedef struct {
	char name[100];
	char type[100];
	char msg[1000];
	char createTime[100];
	int room;
	nameList onlineList[MAX_ALLOWED];
	search searchMsg;
}usrData;

/*typedef struct {
	char name[100];
	char time[32];
	char content[1000];
}historyRes;*/


char normalMsg[1000] = { 0 }; // the normal msg sent to all... e.x: Saxon: msg

