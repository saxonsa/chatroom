#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")

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


char name[255] = {};


// call a thread to receive message from server
void recv_msg(void *client_socket);
