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
				break;
				//return -1;
			}

			if (msg_len == 0){
				printf("Client closed connection\n");
				closesocket(sub_sock);
				break;
				//return -1;
			}

			printf("Bytes Received: %d, message: %s from %s\n", msg_len, szBuff, inet_ntoa(client_addr.sin_addr));

			/* Send message back to Client */
			// msg_len = send(sub_sock, szBuff, sizeof(szBuff), 0);

			for (int i = 0; i < MAX_ALLOWED; i++) {
				if (clients[i].client_socket != INVALID_SOCKET) {
					msg_len = send(clients[i].client_socket, szBuff, sizeof(szBuff), 0);

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
		clients[i].client_socket = INVALID_SOCKET;
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