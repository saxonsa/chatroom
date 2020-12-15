#ifndef SERVER_H_
#define SERVER_H_

#define DEFAULT_PORT 5019	// server Listening Port
#define BUFFERSIZE 8952
#define MAX_ALLOWED 10 // the max number of clients supported by server 
#define MAX_ROOM 50 // the max room number can be created



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
extern int row_num;


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

extern nameList personOnlineList[MAX_ALLOWED];

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
	search searchMsg;
	nameList groupList[MAX_ROOM];
	nameList onlineList[MAX_ALLOWED];
}usrData;



extern char normalMsg[1000]; // the normal msg sent to all... e.x: Saxon: msg

#endif