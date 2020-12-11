#ifndef CLIENT_H
#define CLIENT_H

//#include <stdafx.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#define DEFAULT_PORT	5019

extern WSADATA wsaData;

extern SOCKET connect_sock; // = INVALID_SOCKET;

extern struct sockaddr_in server_addr; // server end address

extern char serverIP[32];// = { 0 };
extern char server_port[10];// = { 0 };
extern char *server_name;// = (char*)"localhost";
extern unsigned short port;// = DEFAULT_PORT;
extern unsigned int addr;

extern char szBuff[1000];
extern int msg_len;
extern struct hostent *hp;


extern char name[255];// = {};

typedef struct {
   char name[100];
   char type[100];
   char msg[1000];
   char createTime[100];
   int room;
}usrData;
extern usrData usr;

// call a thread to receive message from server
//void recv_msg(void *client_socket);

int client_connect(char serverIP[],char server_port[], char* name);

//char* recv_msg_dialog();


#endif // CLIENT_H
