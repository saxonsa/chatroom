#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <signal.h> // for signal() function
#include <mysql.h> // for database functions
#include "server.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmysql.lib")

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

void insert_into_database(char user_name[], char content[]){
	char toInsertHistory[250] = "Insert INTO history(user_name,content) VALUES(";
	
	char userName[1000] = "'";
	strcat_s(userName,sizeof userName,user_name);
	strcat_s(userName,sizeof userName,"','");

	printf("%s \n",userName);

	// char userName[] = "'David','";

	char finalString[300];
	
	// Concat strings
	strcat_s(toInsertHistory,sizeof toInsertHistory,userName);
	strcat_s(toInsertHistory,sizeof toInsertHistory,content);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"');");

	strcpy(finalString,toInsertHistory);

	printf("%s \n",userName);

	ret = mysql_query(&mysqlConnect, finalString); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// A hint that shows the insertion is complte
	// printf("History record succeed! \n");

	return;
}

// 
void search_by_keyword(char keyword[]){
	char toSearchByKeyword[300] = "SELECT * FROM `history` WHERE user_name LIKE '%";

	// Concat strings
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, keyword);
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, "%' OR DATE_FORMAT(create_time, '%Y%m%d') LIKE '%");
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, keyword);
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, "%' OR content LIKE '%");
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, keyword);
	strcat_s(toSearchByKeyword,sizeof toSearchByKeyword, "%';");

	// char userName[] = "'David'";

	char finalString[300];

	strcpy(finalString,toSearchByKeyword);

	ret = mysql_query(&mysqlConnect, finalString); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	res = mysql_store_result(&mysqlConnect);// Check the res value

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		//Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = mysql_num_rows(res);
			for (unsigned int i = 0; field = mysql_fetch_field(res); i++) { 
				printf("%25s", field->name);
				printf(" |");
			}
			printf("\n");
			while (nextRow = mysql_fetch_row(res)) {
				for (int j = 0; j < column; j++) {
					printf("%25s", nextRow[j]);
					printf(" |");
				}
				printf("\n");
			}
		}
		else {
			printf("No resullt. This is the result of a character splitting query... \n");
		}
	}
	else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return;
}

void search_history(){
	// The selection query
	ret = mysql_query(&mysqlConnect, "SELECT * FROM `history`");

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		return;
	}

	res = mysql_store_result(&mysqlConnect);// Check the res value

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		//Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = mysql_num_rows(res);
			for (unsigned int i = 0; field = mysql_fetch_field(res); i++) { 
				printf("%25s", field->name);
				printf(" |");
			}
			printf("\n");
			while (nextRow = mysql_fetch_row(res)) {
				for (int j = 0; j < column; j++) {
					printf("%25s", nextRow[j]);
					printf(" |");
				}
				printf("\n");
			}
		}
		else {
			printf("No resullt. This is the result of a character splitting query... \n");
		}
	}
	else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return;
}

// function to accept a connection
void accept_conn(void *dummy) {
		SOCKET sub_sock = (SOCKET) dummy;

		connecting++;
		printf("current number of clients: %d\n", connecting);

		char current_user_name[300];
		usrData usrInfo;

		while (1) {

			msg_len = recv(sub_sock, szBuff, sizeof(szBuff), 0);

			memcpy(&usrInfo, szBuff, sizeof szBuff);
			printf("usrInfo: name: %s content: %s type: %s\n", usrInfo.name, usrInfo.msg, usrInfo.type);
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

			// decompose szBuff to seperate type and content
			// char *typeMsg = "type: ";
			// char *contentMsg = "content: ";
			// int type = szBuff[strlen(typeMsg)] - 48;
			// char *content = szBuff + strlen(typeMsg) + strlen(contentMsg) + 3;


			for (int index = 0; index < MAX_ALLOWED; index++) {
				if (clients[index].client_socket == sub_sock) {
					// IP: inet_ntoa(client_addr.sin_addr)
					if (strlen(clients[index].name)) {
						printf("Bytes Received: %d, message: %s from name: %s\n", msg_len, usrInfo.msg, clients[index].name);
					} else {
						printf("Bytes Received: %d, message: %s from IP: %s\n", msg_len, usrInfo.msg, inet_ntoa(client_addr.sin_addr));
					}
				}
			}
			

			/* Send message back to Client */

			// check if it's the first time received from clinet
			// if first -- it should be username
			// if not -- it should be the normal msg and should be broadcast

			if (strcmp(usrInfo.type, "ENTER") == 0) {
				char enterMsgOther[100] = { 0 }; // the enter msg sent to others
				char enterMsgSelf[100] = "Welcome "; // the enter msg sent to the client himhelf
				for (int s = 0; s < MAX_ALLOWED; s++) {
					if (clients[s].client_socket == sub_sock) {
						if (!strlen(clients[s].name)) {
							// construct the msg towards different clients
							memcpy(clients[s].name, usrInfo.msg, sizeof(clients[s].name));
							memcpy(enterMsgOther, clients[s].name, sizeof(clients[s].name));
							strcat_s(enterMsgSelf, sizeof(clients[s].name), clients[s].name);
							strcat_s(enterMsgOther, sizeof(enterMsgOther), " enters the chatroom!");
						}
						break;
					}
				}

				// broadcast name msg depends on different clients
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
							strcpy(current_user_name,usrInfo.msg);

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
			if (strcmp(usrInfo.type, "CHAT") == 0) { // not the first time
				insert_into_database(current_user_name, usrInfo.msg); // Insert the history into the database

				search_history();// Search history from database
				
				for (int c = 0; c < MAX_ALLOWED; c++) {
					if (clients[c].client_socket == sub_sock) {
						// save the client name in normalMsg
						memcpy(normalMsg, clients[c].name, sizeof(normalMsg));
						strcat_s(normalMsg, sizeof(normalMsg), ": ");
						strcat_s(normalMsg, sizeof(normalMsg), usrInfo.msg);
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
		closesocket(sub_sock);
		_endthread();
}

int main(int argc, char **argv){

	
	mysql_init(&mysqlConnect);
	if (!(mysql_real_connect(&mysqlConnect, "localhost", "root", "", "chatroom", 3306, NULL, 0))) {
		printf("Failed to access to the database...Error: %s\n", mysql_error(&mysqlConnect));
	}else{
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