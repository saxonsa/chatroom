#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <signal.h> // for signal() function
#include "server.h"
#pragma comment(lib,"ws2_32.lib")

/*
	exit_clean: (safe exit function)
		description: Signal callback function, when unknown exit Ctrl-c, will clear the process

	1. clean up WSA
	2. free all the allocated memory
*/
void exit_clean(int arg) {
	WSACleanup();
	exit(0);
}

// function to accept a connection
void accept_conn(void *dummy) {
		SOCKET sub_sock = (SOCKET) dummy;

		connecting++;
		printf("current number of clients: %d\n", connecting);
		
		while (1) {

			msg_len = recv(sub_sock, szBuff, sizeof(szBuff), 0);

			printf("msg_len: %d\n", msg_len);

			if (msg_len == SOCKET_ERROR){
				fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
				for (int s = 0; s < MAX_ALLOWED; s++) {
					if (clients[s].client_socket == sub_sock) {
						clients[s].client_socket = INVALID_SOCKET;
						memset(clients[s].name, 0, sizeof(clients[s].name));
					}
				}
				closesocket(sub_sock);
				break;
				//return -1;
			}
			
			if (msg_len == 0){
				printf("Client closed connection\n");
				for (int s = 0; s < MAX_ALLOWED; s++) {
					if (clients[s].client_socket == sub_sock) {
						clients[s].client_socket = INVALID_SOCKET;
						memset(clients[s].name, 0, sizeof(clients[s].name));
					}
				}
				closesocket(sub_sock);
				break;
				//return -1;
			}

			for (int index = 0; index < MAX_ALLOWED; index++) {
				if (clients[index].client_socket == sub_sock) {
					// IP: inet_ntoa(client_addr.sin_addr)
					if (strlen(clients[index].name)) {
						printf("Bytes Received: %d, message: %s from name: %s\n", msg_len, szBuff, clients[index].name);
					} else {
						printf("Bytes Received: %d, message: %s from IP: %s\n", msg_len, szBuff, inet_ntoa(client_addr.sin_addr));
					}
				}
			}
			

			/* Send message back to Client */

			// check if it's the first time received from clinet
			// if first -- it should be username
			// if not -- it should be the normal msg and should be broadcast
			int first = 0;
			char enterMsgOther[100] = { 0 }; // the enter msg sent to others
			char enterMsgSelf[100] = "Welcome "; // the enter msg sent to the client himhelf
			for (int s = 0; s < MAX_ALLOWED; s++) {
				if (clients[s].client_socket == sub_sock) {
					if (!strlen(clients[s].name)) {
						// construct the msg towards different clients
						memcpy(clients[s].name, szBuff, sizeof(clients[s].name));
						memcpy(enterMsgOther, clients[s].name, sizeof(clients[s].name));
						strcat_s(enterMsgSelf, sizeof(clients[s].name), clients[s].name);
						strcat_s(enterMsgOther, sizeof(enterMsgOther), " enters the chatroom!");
						first = 1;
						break;
					} else {
						break;
					}
				}
			}

			// broadcast name msg depends on different clients
			if (first == 1) { // not the first time
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						if (clients[i].client_socket == sub_sock) {
							msg_len = send(clients[i].client_socket, enterMsgSelf, sizeof(enterMsgSelf), 0);

							if (msg_len <= 0){
								printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
								closesocket(sub_sock);
								connecting--;
								printf("current number of clients: %d\n", connecting);
								_endthread();
								//return -1;
							}
						} else {
							msg_len = send(clients[i].client_socket, enterMsgOther, sizeof(enterMsgOther), 0);

							if (msg_len <= 0){
								printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
								closesocket(sub_sock);
								connecting--;
								printf("current number of clients: %d\n", connecting);
								_endthread();
								//return -1;
							}
						}
					}
				}
			}

			// broadcast normal chat msg to all clients in the chatroom
			if (first == 0) { // not the first time
				for (int c = 0; c < MAX_ALLOWED; c++) {
					if (clients[c].client_socket == sub_sock) {
						// save the client name in normalMsg
						memcpy(normalMsg, clients[c].name, sizeof(normalMsg));
						strcat_s(normalMsg, sizeof(normalMsg), ": ");
						strcat_s(normalMsg, sizeof(normalMsg), szBuff);
					}
				}
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						msg_len = send(clients[i].client_socket, normalMsg, sizeof(normalMsg), 0);

						if (msg_len <= 0){
							printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
							closesocket(sub_sock);
							connecting--;
							printf("current number of clients: %d\n", connecting);
							_endthread();
							//return -1;
						}
					}
				}
			}
		}
		connecting--;
		printf("current number of clients: %d\n", connecting);
		_endthread();
}

int main(int argc, char **argv){

	// Register signal handlers to prevent accidental exits and release ports
    signal(SIGTERM, exit_clean);
    signal(SIGINT, exit_clean);

	/*  Initilize WSA

		WSAStartup: 
			@param1: request Socket version
			@param2: variable to save version information
	*/

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR){
		// stderr: standard error are printed to the screen.
		fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
		//WSACleanup function terminates use of the Windows Sockets DLL. 
		WSACleanup();
		return -1;
	}

	for (int i = 0; i < MAX_ALLOWED; i++) {
		clients[i].fd = i;
		clients[i].client_socket = INVALID_SOCKET;
		memset(clients[i].name, 0, sizeof(clients[i].name));
	}


	/* Set the attributes of server address */
	local.sin_family		= AF_INET; // The way of connection
	local.sin_addr.s_addr	= INADDR_ANY; // Any client can connect to this server
	local.sin_port		= htons(DEFAULT_PORT); // Server Listening port

	/* Create a socket */
	sock = socket(AF_INET,SOCK_STREAM, 0);	//TCp socket
	if (sock == INVALID_SOCKET){
		fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	/* Bind server */
	if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR){
		fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	/*
		waiting for the connections from Client
		max number of clients waiting to connect: MAX_ALLOWED
	*/
	if (listen(sock, MAX_ALLOWED) == SOCKET_ERROR){
		fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Waiting for the connections ........\n");


	while(1){
		addr_len = sizeof(client_addr);
		msg_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
		if (msg_sock == INVALID_SOCKET){
			fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		if (connecting >= MAX_ALLOWED) {
			closesocket(msg_sock);
			printf("max client number reached!\n");
			continue;
		}

		printf("accepted connection from %s, port %d\n",
			inet_ntoa(client_addr.sin_addr),
			htons(client_addr.sin_port));

		for (int num = 0; num < MAX_ALLOWED; num++) {
			if (clients[num].client_socket == INVALID_SOCKET) {
				clients[num].client_socket = msg_sock;
				break;
			}
		}

		_beginthread(accept_conn,0,(void*)msg_sock);
	}


	exit_clean(0);
}