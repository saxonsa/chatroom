#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "client.h"
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char **argv){

	// *************************************************************************************** //
	/* Read the server's IP address and port number which client wants to establish connection */
	printf("Please input the server's IP: ");
	gets_s(serverIP);
	printf("Please input the server's port: ");
	gets_s(server_port);

	server_name = serverIP;
	port = atoi(server_port);
	// *************************************************************************************** //

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

	if (isalpha(server_name[0]))
		hp = gethostbyname(server_name);
	else{
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (hp==NULL)
	{
		fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//copy the resolved information into the sockaddr_in structure
	memset(&server_addr, 0, sizeof(server_addr));
	memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = hp->h_addrtype;
	server_addr.sin_port = htons(port);

	
	connect_sock = socket(AF_INET,SOCK_STREAM, 0);	//TCp socket


	if (connect_sock == INVALID_SOCKET){
		fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Client connecting to: %s\n", hp->h_name);

	if (connect(connect_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) 
		== SOCKET_ERROR){
		fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// print connected server information
	printf("Successful connect to server IP: %s; Port: %d\n", inet_ntoa(server_addr.sin_addr), htons(server_addr.sin_port));

	
	_beginthread(recv_msg, 0, (void *) connect_sock);

	while (1) {

		// get input
		int empty = 0;
		while (1) {
			printf("input character string:\n");
			gets_s(szBuff);

			// empty input is not allowed
			if (strlen(szBuff) == 0) { // empty string
				empty = 1;
			} else {
				for (unsigned int c = 0; c < strlen(szBuff); c++) { // a string of spaces
					if (szBuff[c] != ' ') {
						empty = 0;
						break;
					} else
						empty = 1;
				}
			}
			if (empty == 1) {
				printf("Empty message cannot be sent!\n");
				empty = 0;
				continue;
			} else 
				break;
		}

		// when user type end in, the client will close the socket
		if (strcmp(szBuff, "end") == 0) {
			printf("exit chatroom\n");
			break;
		}
	
		msg_len = send(connect_sock, szBuff, sizeof(szBuff), 0);
	
		if (msg_len == SOCKET_ERROR){
			fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		if (msg_len == 0){
			printf("server closed connection\n");
			closesocket(connect_sock);
			WSACleanup();
			return -1;
		}
	}

	closesocket(connect_sock);
	WSACleanup();

	return 0;
}

void recv_msg(void *client_socket) {
	while (1) {
		msg_len = recv(connect_sock, szBuff, sizeof(szBuff), 0);
	
		if (msg_len == SOCKET_ERROR){
			fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
			break;
		}

		if (msg_len == 0){
			printf("server closed connection\n");
			break;
		}

		printf("Echo from the server %s.\n", szBuff);
	}
	closesocket(connect_sock);
	WSACleanup();
	_endthread();
}