#ifndef SERVER_H_
#define SERVER_H_

#define DEFAULT_PORT 5019	// server Listening Port
#define BUFFERSIZE 2920
#define MAX_ALLOWED 3 // the max number of clients supported by server 



/*************************************************************/
/************* structure to store socket *********************/

extern WSADATA wsaData;	
extern SOCKET sock;		// master server socket
extern SOCKET msg_sock;	// client socket

/*************************************************************/


extern struct sockaddr_in local;		// server end address
extern struct sockaddr_in client_addr;	// client end address


extern char szBuff[BUFFERSIZE];
extern int msg_len;
extern int addr_len;

extern int connecting; // cuurent number of clients connected with server

//Database global variables

extern MYSQL mysqlConnect; 
extern MYSQL_RES *res;
extern MYSQL_FIELD *field; 
extern MYSQL_ROW nextRow;
extern int ret;


// client structure
// an array to store all the clients connecting with server
typedef struct _Client {
	int fd;
	char name[100]; // Client name
	SOCKET client_socket; // Client socket
}Client;

extern Client clients[MAX_ALLOWED];                                

typedef struct _nameList {
	int uid;
	char name[100];
}nameList;

extern int onlineList_msg;

typedef struct {
	char search_name[100];
	char search_time[100];
	char search_content[1000];
}search;

typedef struct {
	char name[100];
	char pwd[100];
	char type[100];
	char msg[1000];
	char createTime[100];
	int room;
	char recv_name[100];
	nameList onlineList[MAX_ALLOWED];
	search searchMsg;
}usrData;

/*typedef struct {
	char name[100];
	char time[32];
	char content[1000];
}historyRes;*/


extern char normalMsg[1000]; // the normal msg sent to all... e.x: Saxon: msg

#endif