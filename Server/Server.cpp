#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <process.h>
#include <signal.h> // for signal() function
//#include <mysql.h> // for database functions
#include <iostream>
#include <time.h>
//#include "server.h"
#include "sqlfuc.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

WSADATA wsaData = {0};
SOCKET sock = INVALID_SOCKET;     // master server socket
SOCKET msg_sock = INVALID_SOCKET; // client socket

/*************************************************************/

struct sockaddr_in local = {0};       // server end address
struct sockaddr_in client_addr = {0}; // client end address

char szBuff[BUFFERSIZE];
int msg_len;
int addr_len;

int connecting = 0; // cuurent number of clients connected with server

//Database global variables

MYSQL mysqlConnect;
MYSQL_RES *res;
MYSQL_FIELD *field;
MYSQL_ROW nextRow;
int ret = 0;

Client clients[MAX_ALLOWED] = {0};

int onlineList_msg = 0;

char normalMsg[1000];

nameList personOnlineList[MAX_ALLOWED];

/*
exit_clean: (safe exit function)
description: Signal callback function, when unknown exit Ctrl-c, will clear the process

1. clean up WSA
2. free all the allocated memory
*/
void exit_clean(int arg)
{
	WSACleanup();
	exit(0);
}

// function to accept a connection
void accept_conn(void *dummy)
{
	SOCKET sub_sock = (SOCKET)dummy;

	connecting++;
	printf("current number of clients: %d\n", connecting);

	usrData usrInfo;

	char current_name[100] = { 0 };

	nameList groupList[MAX_ROOM] = { 0 };

	// init onlineList
	for (int i = 0; i < MAX_ALLOWED; i++)
	{
		usrInfo.onlineList[i].uid = -1;
		memset(usrInfo.onlineList[i].name, 0, sizeof usrInfo.onlineList[i].name);
	}

	while (1)
	{

		msg_len = recv(sub_sock, szBuff, sizeof(szBuff), 0);

		memcpy(&usrInfo, szBuff, sizeof szBuff);

		printf("usrInfo: name: %s content: %s type: %s\n", usrInfo.name, usrInfo.msg, usrInfo.type);

		time_t timep;
		struct tm p;
		time(&timep);            // get how many seconds pass sence 1900
		localtime_s(&p, &timep); // use local time to transform from second to stucture tm
		sprintf_s(usrInfo.createTime, "%d/%d/%d %02d:%02d:%02d\n", 1900 + p.tm_year, 1 + p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
		printf("create time: %s\n", usrInfo.createTime);

		printf("msg_len: %d\n", msg_len);

		// recv fails, destroy the socket
		if (msg_len == SOCKET_ERROR)
		{
			fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
			memcpy(usrInfo.type, "QUIT", sizeof usrInfo.type);

			// clear client info in clients array
			for (int s = 0; s < MAX_ALLOWED; s++)
			{
				if (clients[s].client_socket == sub_sock)
				{
					for (int i = 0; i < MAX_ALLOWED; i++) {
						if (strcmp(personOnlineList[i].name, clients[s].name) == 0) {
							personOnlineList[i].uid = -1;
							memcpy(personOnlineList[i].name, "", sizeof personOnlineList[i].name);
						}
					}
					clients[s].fd = -1;
					clients[s].client_socket = INVALID_SOCKET;
					memset(clients[s].name, 0, sizeof(clients[s].name));
				}
			}

			for (int i = 0; i < MAX_ALLOWED; i++) {
				memcpy(usrInfo.onlineList[i].name, personOnlineList[i].name, sizeof usrInfo.onlineList[i].name);
				usrInfo.onlineList[i].uid = personOnlineList[i].uid;
			}
			

			for (unsigned i = 0; i < MAX_ALLOWED; i++)
			{
				if (clients[i].client_socket != INVALID_SOCKET)
				{
					msg_len = send(clients[i].client_socket, (char *)&usrInfo, BUFFERSIZE, 0);
					if (msg_len <= 0)
					{
						printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
						closesocket(sub_sock);
						connecting--;
						printf("current number of clients: %d\n", connecting);
						_endthread();
					}
				}
			}

			// set status to 0 when usr quit the room
			char* resMsg;
			// set status to 0
			resMsg = set_user_status(current_name, 0);
			printf("set status to 0 msg: %s\n", resMsg);

			break;
			//return -1;
		}

		if (msg_len == 0)
		{
			printf("Client closed connection\n");
			char* resMsg;
			// set status to 0
			resMsg = set_user_status(current_name, 0);
			printf("set status to 0 msg: %s\n", resMsg);
			for (int s = 0; s < MAX_ALLOWED; s++)
			{
				if (clients[s].client_socket == sub_sock)
				{
					clients[s].client_socket = INVALID_SOCKET;
					memset(clients[s].name, 0, sizeof(clients[s].name));
				}
			}
			break;
			//return -1;
		}

		if (usrInfo.name)
		{
			printf("Bytes Received: %d, message: %s from name: %s\n", msg_len, usrInfo.msg, usrInfo.name);
		}
		else
		{
			printf("Bytes Received: %d, message: %s from IP: %s\n", msg_len, usrInfo.msg, inet_ntoa(client_addr.sin_addr));
		}

		/* Send message back to Client */

		if (strcmp(usrInfo.type, "LOGIN") == 0)
		{
			char *resStr = check_login(usrInfo.name, usrInfo.pwd);

			if (strcmp(resStr, "Success") == 0 || strcmp(resStr, "new") == 0)
			{
				strcpy_s(usrInfo.type, sizeof usrInfo.type, "Login");
				send(sub_sock, (char *)&usrInfo, BUFFERSIZE, 0);			
			}
			else
			{
				strcpy_s(usrInfo.type, sizeof usrInfo.type, "LoginF");
				strcpy_s(usrInfo.msg, sizeof usrInfo.msg, resStr);
				send(sub_sock, (char *)&usrInfo, BUFFERSIZE, 0);
				printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
				closesocket(sub_sock);
				connecting--;
				printf("current number of clients: %d\n", connecting);
				_endthread();
			}
		}

		if (strcmp(usrInfo.type, "ENTER") == 0)
		{

			/* send enter room msg */
			char enterMsgOther[100] = {0};       // the enter msg sent to others
			char enterMsgSelf[100] = "Welcome "; // the enter msg sent to the client himhelf

			// initialize online list on server end

			for (int i = 0; i < MAX_ALLOWED; i++) {
				if (personOnlineList[i].uid == -1) {
					strcpy_s(personOnlineList[i].name, sizeof personOnlineList[i].name, usrInfo.name);
					personOnlineList[i].uid = i;
					break;
				}
			}

			for (int i = 0; i < MAX_ALLOWED; i++) {
				printf("onlineList[i]: %s\n", personOnlineList[i].name);
			}
			

			for (int s = 0; s < MAX_ALLOWED; s++)
			{
				if (clients[s].client_socket == sub_sock)
				{
					if (!strlen(clients[s].name))
					{
						// construct the msg towards different clients
						memcpy(clients[s].name, usrInfo.name, sizeof(clients[s].name));
						clients[s].fd = s;
						memcpy(enterMsgOther, clients[s].name, sizeof(clients[s].name));
						if (current_name) {
							// initialize group list on server end when the first enter the room
							char **group_name = NULL;
							memcpy(current_name, usrInfo.name, sizeof(current_name));
							group_name = get_room_name(current_name);
							for (int i = 0; i < sizeof(group_name)/sizeof(char*); i++) {
								if (group_name[i] && strcmp(group_name[i], "") != 0) {
									memcpy(groupList[i].name, group_name[i], sizeof groupList[i].name);
									groupList[i].uid = i;
								}
							}

							for (int i = 0; i < MAX_ROOM; i++) {
								printf("%d: group member: %s\n", i, groupList[i].name);
							}
							
						}
						strcat_s(enterMsgSelf, sizeof(clients[s].name), clients[s].name);
						strcat_s(enterMsgOther, sizeof(enterMsgOther), " enters the chatroom!");
					}
					break;
				}
			}

			// *usrInfo.onlineList = *personOnlineList;
			for (int i = 0; i < MAX_ALLOWED; i++) {
				memcpy(usrInfo.onlineList[i].name, personOnlineList[i].name, sizeof usrInfo.onlineList[i].name);
				usrInfo.onlineList[i].uid = personOnlineList[i].uid;
			}

			// *usrInfo.groupList = *groupList;
			for (int i = 0; i < MAX_ROOM; i++) {
				memcpy(usrInfo.groupList[i].name, groupList[i].name, sizeof usrInfo.groupList[i].name);
				usrInfo.groupList[i].uid = groupList[i].uid;
			}
			

			// broadcast name msg depends on different clients
			for (int i = 0; i < MAX_ALLOWED; i++)
			{
				if (clients[i].client_socket != INVALID_SOCKET)
				{
					if (clients[i].client_socket == sub_sock)
						strcpy_s(usrInfo.msg, sizeof usrInfo.msg, enterMsgSelf);
					else
						strcpy_s(usrInfo.msg, sizeof usrInfo.msg, enterMsgOther);

					msg_len = send(clients[i].client_socket, (char *)&usrInfo, BUFFERSIZE, 0);

					if (msg_len <= 0)
					{
						printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
						closesocket(sub_sock);
						connecting--;
						printf("current number of clients: %d\n", connecting);
						_endthread();
					}
				}
			}
		}

		// broadcast normal chat msg to all clients in the chatroom
		if (strcmp(usrInfo.type, "CHAT") == 0)
		{
			if (usrInfo.room == 0) { // private CHAT
				insert_into_private(current_name, usrInfo.createTime, usrInfo.msg, usrInfo.recv_name);
				// find recv socket
				SOCKET recv_socket = INVALID_SOCKET;
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (strcmp(clients[i].name, usrInfo.recv_name) == 0) {
						recv_socket = clients[i].client_socket;
						break;
					}
				}

				// send msg to sender
				msg_len = send(sub_sock, (char*)&usrInfo, BUFFERSIZE, 0);
				if (msg_len <= 0)
				{
					printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
					closesocket(sub_sock);
					connecting--;
					printf("current number of clients: %d\n", connecting);
					_endthread();
				}
				
				msg_len = send(recv_socket, (char*)&usrInfo, BUFFERSIZE, 0);
				if (msg_len <= 0)
				{
					printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
					closesocket(sub_sock);
					connecting--;
					printf("current number of clients: %d\n", connecting);
					_endthread();
				}
				

			} else { // group CHAT
				insert_into_group(usrInfo.name, usrInfo.createTime, usrInfo.msg, usrInfo.room); // Insert the history into the database
				// printf("curr name: %s\n", usrInfo.name);
				// printf("curr time: %s\n", usrInfo.createTime);
				// printf("curr msg: %s\n", usrInfo.msg);
				// search_history();// Search history from database

				for (int i = 0; i < MAX_ALLOWED; i++)
				{
					if (clients[i].client_socket == sub_sock)
					{
						strcpy_s(usrInfo.name, sizeof usrInfo.name, clients[i].name);
					}
				}

				for (int i = 0; i < MAX_ALLOWED; i++)
				{
					if (clients[i].client_socket != INVALID_SOCKET)
					{
						msg_len = send(clients[i].client_socket, (char *)&usrInfo, BUFFERSIZE, 0);

						if (msg_len <= 0)
						{
							printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
							closesocket(sub_sock);
							connecting--;
							printf("current number of clients: %d\n", connecting);
							_endthread();
						}
					}
				}
			}
		}

		if (strcmp(usrInfo.type, "SEARCH") == 0)
		{
			// Because time is always returned from client, there is only four cases to judge.

			printf("Msg from search request --- name: %s\n", usrInfo.searchMsg.search_name);
			printf("Msg from search request --- content: %s\n", usrInfo.searchMsg.search_content);
			printf("Msg from search request --- time: %s\n", usrInfo.searchMsg.search_time);

			if (strcmp(usrInfo.searchMsg.search_name, "") == 0)
			{
				// The case that only search by date
				if (strcmp(usrInfo.searchMsg.search_content, "") == 0)
				{
					search_group_by_date(usrInfo.searchMsg.search_time, usrInfo.room, sub_sock, usrInfo);
				}
				// The case that only search by date and content
				else
				{
				}
			}
			else
			{
				if (strcmp(usrInfo.searchMsg.search_content, "") == 0)
				{
				}
			}
		}
		if (strcmp(usrInfo.type, "SWITCH_PRIVATE_CHAT") == 0) {
			printf("usr: %s, usrInfo.recv: %s\n", usrInfo.name, usrInfo.recv_name);
			// load chat history chat to usrInfo.recv_name


		}
	}

	connecting--;
	printf("current number of clients: %d\n", connecting);
	closesocket(sub_sock);
	_endthread();
}

int main(int argc, char **argv)
{
	mysql_init(&mysqlConnect);
	if (!(mysql_real_connect(&mysqlConnect, "localhost", "root", "", "chatroom", 3306, NULL, 0)))
	{
		printf("Failed to access to the database...Error: %s\n", mysql_error(&mysqlConnect));
	}
	else
	{
		printf("Success! \n");
	}

	// Register signal handlers to prevent accidental exits and release ports
	signal(SIGTERM, exit_clean);
	signal(SIGINT, exit_clean);

	/*  Initilize WSA

	WSAStartup:
	@param1: request Socket version
	@param2: variable to save version information
	*/

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
	{
		// stderr: standard error are printed to the screen.
		fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
		//WSACleanup function terminates use of the Windows Sockets DLL.
		WSACleanup();
		return -1;
	}

	for (int i = 0; i < MAX_ALLOWED; i++)
	{
		// init clients
		clients[i].fd = -1;
		clients[i].client_socket = INVALID_SOCKET;
		memset(clients[i].name, 0, sizeof(clients[i].name));
		personOnlineList[i].uid = -1;
		memset(personOnlineList[i].name, 0, sizeof personOnlineList[i].name);
	}

	/* Set the attributes of server address */
	local.sin_family = AF_INET;           // The way of connection
	local.sin_addr.s_addr = INADDR_ANY;   // Any client can connect to this server
	local.sin_port = htons(DEFAULT_PORT); // Server Listening port

	/* Create a socket */
	sock = socket(AF_INET, SOCK_STREAM, 0); //TCp socket
	if (sock == INVALID_SOCKET)
	{
		fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	/* Bind server */
	if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
	{
		fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	/*
	waiting for the connections from Client
	max number of clients waiting to connect: MAX_ALLOWED
	*/
	if (listen(sock, MAX_ALLOWED) == SOCKET_ERROR)
	{
		fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Waiting for the connections ........\n");

	while (1)
	{
		addr_len = sizeof(client_addr);
		msg_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_len);
		if (msg_sock == INVALID_SOCKET)
		{
			fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		if (connecting >= MAX_ALLOWED)
		{
			closesocket(msg_sock);
			printf("max client number reached!\n");
			continue;
		}

		printf("accepted connection from %s, port %d\n",
			inet_ntoa(client_addr.sin_addr),
			htons(client_addr.sin_port));

		for (int num = 0; num < MAX_ALLOWED; num++)
		{
			if (clients[num].client_socket == INVALID_SOCKET)
			{
				clients[num].client_socket = msg_sock;
				break;
			}
		}

		_beginthread(accept_conn, 0, (void *)msg_sock);
	}

	exit_clean(0);
}