#ifndef CLIENT_H
#define CLIENT_H

//#include <stdafx.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#define DEFAULT_PORT 5019
#define BufferSize 9252

extern WSADATA wsaData;

extern SOCKET connect_sock; // = INVALID_SOCKET;

extern struct sockaddr_in server_addr; // server end address

extern char serverIP[32];    // = { 0 };
extern char server_port[10]; // = { 0 };
extern char *server_name;    // = (char*)"localhost";
extern unsigned short port;  // = DEFAULT_PORT;
extern unsigned int addr;

extern char szBuff[BufferSize];
extern int msg_len;
extern struct hostent *hp;

const int MAX_ALLOWED = 10; // the max number of clients supported by server
const int MAX_ROOM = 50;    // the max room number can be created

extern char name[255]; // = {};
extern char g_recv_name[100];

typedef struct _nameList
{
  int uid;
  char name[100];
} nameList;

typedef struct
{
  char search_name[100];
  char search_time[100];
  char search_content[1000];
} search;

typedef struct
{
  char name[100];
  char pwd[100];
  char type[100];
  char enter[100];
  char msg[1000];
  char createTime[100];
  int room;
  char recv_name[100];
  char room_name[100];
  char invite_name[100];
  search searchMsg;
  nameList groupList[MAX_ROOM];
  nameList onlineList[MAX_ALLOWED];
} usrData;

extern usrData usr;
extern nameList groupList[MAX_ROOM];

extern int curSelectRmNum;

// call a thread to receive message from server
//void recv_msg(void *client_socket);

int client_connect(char serverIP[], char server_port[], char *name);

char* str_handle(char raw[]);

//char* recv_msg_dialog();

#endif // CLIENT_H
