#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include "server.h"
#pragma comment(lib,"ws2_32.lib")

// function to accept a connection
void accept_conn(void *dummy) {
	// doing something here
}

int main(int argc, char **argv){

	/*  Initilize Socket

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

	/* waiting for the connections from Client */
	if (listen(sock, 5) == SOCKET_ERROR){
		fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	
	while (1) {

		printf("Waiting for the connections ........\n");

		addr_len = sizeof(client_addr);



		msg_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
		if (msg_sock == INVALID_SOCKET){
			fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		printf("accepted connection from %s, port %d\n",
			inet_ntoa(client_addr.sin_addr),
			htons(client_addr.sin_port));

		msg_len = recv(msg_sock, szBuff, sizeof(szBuff), 0);

		if (msg_len == SOCKET_ERROR){
			fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}




		if (msg_len == 0){
			printf("Client closed connection\n");
			closesocket(msg_sock);
			return -1;
		}

		printf("Bytes Received: %d, message: %s from %s\n", msg_len, szBuff, inet_ntoa(client_addr.sin_addr));

		/* Send message back to Client */
		msg_len = send(msg_sock, szBuff, sizeof(szBuff), 0);
		if (msg_len == 0){
			printf("Client closed connection\n");
			closesocket(msg_sock);
			return -1;
		}

		closesocket(msg_sock);
	}
	WSACleanup();
}