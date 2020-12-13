#include "stdafx.h"
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <signal.h> // for signal() function
#include <mysql.h> // for database functions
#include <time.h>
#include "server.h"
#include <iostream>

using namespace std;

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

void print_table(MYSQL_RES *res){
	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		//Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = (int)mysql_num_rows(res);
			for (int i = 0; field = mysql_fetch_field(res); i++) { 
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

// insert gourp info ��Ⱥ��������ʷ��¼
void insert_into_group(char user_name[], char creat_time[], char content[], int rid){
	char toInsertHistory[1000];
	
	sprintf_s(toInsertHistory,"Insert INTO group_history(user_name,create_time,content,room_name) VALUES('%s','%s','%s',%d);"
			,user_name
			,creat_time
			,content
			,rid);

	printf("%s \n",user_name);

	ret = mysql_query(&mysqlConnect, toInsertHistory); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// A hint that shows the insertion is complte
	// printf("History record succeed! \n");

	return;
}

// insert private info ��˽��������ʷ��¼
void insert_into_private(char sender[], char creat_time[], char content[], char receiver[]){
	char toInsertHistory[1000];

	// Concat strings
	sprintf_s(toInsertHistory,"Insert INTO private_history VALUES('%s','%s','%s','%s');"
			,sender
			,creat_time
			,content
			,receiver);

	ret = mysql_query(&mysqlConnect, toInsertHistory); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// A hint that shows the insertion is complte
	// printf("History record succeed! \n");

	return;
}

// Use name to search private ��������˽��
void search_private_by_name(char sender[], char receiver[]){
	char toSearchByName[250];
	
	sprintf_s(toSearchByName,"SELECT * FROM `private_history` WHERE user_name = '%s' AND recevie_name = '%s';", sender, receiver);

	ret = mysql_query(&mysqlConnect, toSearchByName); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Use name to search group ��������Ⱥ��
void search_group_by_name(char user_name[], int rid){
	char toSearchByName[250];
	
	sprintf_s(toSearchByName,"SELECT * FROM `group_history` WHERE user_name = '%s' AND rid = %d;", user_name, rid);

	ret = mysql_query(&mysqlConnect, toSearchByName); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Use content to search private �����ݲ�˽��
void search_private_by_content(char sender[], char receiver[], char content[]){
	char toSearchByContent[250]; // vague query

	sprintf_s(toSearchByContent,"SELECT * FROM `private_history` WHERE user_name = '%s' AND recevie_name = '%s' AND content LIKE '%s';"
			,sender
			,receiver
			,content);

	ret = mysql_query(&mysqlConnect, toSearchByContent); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Use content to search group �����ݲ�Ⱥ��
void search_group_by_content(char content[], int rid){
	char toSearchByContent[250]; // vague query

	sprintf_s(toSearchByContent,"SELECT * FROM `group_history` WHERE rid = %d AND content LIKE '%s';", rid, content);

	ret = mysql_query(&mysqlConnect, toSearchByContent); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Use date to search private ��ʱ���˽��
void search_private_by_date(char sender[], char receiver[], char date[]){
	char toSearchByDate[250];

	sprintf_s(toSearchByDate,"SELECT * FROM `private_history` WHERE user_name = '%s' AND recevie_name = '%s' AND DATE_FORMAT(create_time, '%Y%m%d') = '%s';"
			,sender
			,receiver
			,date);

	ret = mysql_query(&mysqlConnect, toSearchByDate); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Use date to search group ��ʱ���Ⱥ��
void search_group_by_date(char date[], int rid){
	char toSearchByDate[250];

	sprintf_s(toSearchByDate,"SELECT * FROM `group_history` WHERE rid = %d AND DATE_FORMAT(create_time, '%Y%m%d') = '%s';"
			,rid
			,date);

	ret = mysql_query(&mysqlConnect, toSearchByDate); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Get all private history ��ȫ��˽�ļ�¼
void search_private_history(){
	// The selection query
	ret = mysql_query(&mysqlConnect, "SELECT * FROM `private_history`;");

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// Get all group history ��ȫ��Ⱥ�ļ�¼
void search_gourp_history(){
	// The selection query
	ret = mysql_query(&mysqlConnect, "SELECT * FROM `group_history`;");

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// for user to sign up ע�ắ��
void user_sign_up(char user_name[], char pwd[]){
	char signUpInfo[250];

	// connect sql
	sprintf_s(signUpInfo,"INSERT INTO users VALUES('%s','%s');"
			,user_name
			,pwd);

	ret = mysql_query(&mysqlConnect, signUpInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("User sign up failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// create new chatroom
void add_room(char admin[], char room_name[]){
	char addRoomInfo[250];

	// connect sql
	sprintf_s(addRoomInfo,"INSERT INTO room_info(administrator,room_name) VALUES('%s','%s');"
			,admin
			,room_name);

	ret = mysql_query(&mysqlConnect, addRoomInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// add a member to a chat room ���ض�chatroom�������
void add_mem(int rid, char mem_name[]){
	char addMemberInfo[250] = "INSERT INTO room_mem VALUES(";

	sprintf_s(addMemberInfo,"INSERT INTO room_mem VALUES VALUES(%d,'%s');"
			,rid
			,mem_name);

	ret = mysql_query(&mysqlConnect, addMemberInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Add member failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// add a private chat ˽��
void add_private_chat(char sender[], char creat_time[], char content[], char recevier[]){
	char addPrivateInfo[250] = "INSERT INTO private_history VALUES('";

	sprintf_s(addPrivateInfo,"INSERT INTO private_history VALUES('%s','%s','%s','%s');"
			,sender
			,creat_time
			,content
			,recevier);

	ret = mysql_query(&mysqlConnect, addPrivateInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

// function to accept a connection
void accept_conn(void *dummy) {
		SOCKET sub_sock = (SOCKET) dummy;

		connecting++;
		printf("current number of clients: %d\n", connecting);

		usrData usrInfo;

		// init onlineList
		for (int i = 0; i < MAX_ALLOWED; i++) {
			usrInfo.onlineList[i].uid = -1;
		}

		while (1) {

			

			msg_len = recv(sub_sock, szBuff, sizeof(szBuff), 0);
			
			memcpy(&usrInfo, szBuff, sizeof szBuff);

			printf("usrInfo: name: %s content: %s type: %s\n", usrInfo.name, usrInfo.msg, usrInfo.type);

			time_t timep;
			struct tm p;
			time(&timep); // get how many seconds pass sence 1900
			localtime_s(&p, &timep); // use local time to transform from second to stucture tm
			sprintf_s(usrInfo.createTime,"%d/%d/%d %02d:%02d:%02d\n", 1900 + p.tm_year, 1+ p.tm_mon, p.tm_mday,p.tm_hour, p.tm_min, p.tm_sec);
			printf("create time: %s\n", usrInfo.createTime);

			printf("msg_len: %d\n", msg_len);

			// recv fails, destroy the socket
			if (msg_len == SOCKET_ERROR){
				fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
				for (int s = 0; s < MAX_ALLOWED; s++) {
					if (clients[s].client_socket == sub_sock) {
						clients[s].client_socket = INVALID_SOCKET;
						memset(clients[s].name, 0, sizeof(clients[s].name));
					}
				}
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
				break;
				//return -1;
			}

			// printf("Msg from search request --- name: %s\n", usrInfo.searchMsg->search_name);
			// printf("Msg from search request --- content: %s\n", usrInfo.searchMsg->search_content);
			// printf("Msg from search request --- time: %s\n", usrInfo.searchMsg->search_time);

			// decompose szBuff to seperate type and content
			// char *typeMsg = "type: ";
			// char *contentMsg = "content: ";
			// int type = szBuff[strlen(typeMsg)] - 48;
			// char *content = szBuff + strlen(typeMsg) + strlen(contentMsg) + 3;

			if (usrInfo.name) {
				printf("Bytes Received: %d, message: %s from name: %s\n", msg_len, usrInfo.msg, usrInfo.name);
			} else {
				printf("Bytes Received: %d, message: %s from IP: %s\n", msg_len, usrInfo.msg, inet_ntoa(client_addr.sin_addr));
			}
			

			/* Send message back to Client */

			if (strcmp(usrInfo.type, "ENTER") == 0) {
				
				/* send enter room msg */
				char enterMsgOther[100] = { 0 }; // the enter msg sent to others
				char enterMsgSelf[100] = "Welcome "; // the enter msg sent to the client himhelf
				for (int s = 0; s < MAX_ALLOWED; s++) {
					if (clients[s].client_socket == sub_sock) {
						if (!strlen(clients[s].name)) {
							// construct the msg towards different clients
							memcpy(clients[s].name, usrInfo.name, sizeof(clients[s].name));
							memcpy(enterMsgOther, clients[s].name, sizeof(clients[s].name));
							strcat_s(enterMsgSelf, sizeof(clients[s].name), clients[s].name);
							strcat_s(enterMsgOther, sizeof(enterMsgOther), " enters the chatroom!");
						}
						break;
					}
				}

				// online list logic
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						usrInfo.onlineList[i].uid = i;
						memcpy(usrInfo.onlineList[i].name, clients[i].name, sizeof(usrInfo.onlineList[i].name));
					}
				}

				for (int i = 0; i < MAX_ALLOWED; i++) {
					printf("---usrInfo.online.uid: %d\n", usrInfo.onlineList[i].uid);
				}

				// broadcast name msg depends on different clients
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						if (clients[i].client_socket == sub_sock) {
							strcpy_s(usrInfo.msg, sizeof usrInfo.msg, enterMsgSelf);
							msg_len = send(clients[i].client_socket, (char*)&usrInfo, BufferSize, 0);

							if (msg_len <= 0){
								printf("Client IP: %s closed connection\n", inet_ntoa(client_addr.sin_addr));
								closesocket(sub_sock);
								connecting--;
								printf("current number of clients: %d\n", connecting);
								_endthread();
								//return -1;
							}

						} else {
							strcpy_s(usrInfo.msg, sizeof usrInfo.msg, enterMsgOther);
							msg_len = send(clients[i].client_socket, (char*)&usrInfo, BufferSize, 0);

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
			if (strcmp(usrInfo.type, "CHAT") == 0) {
				insert_into_database(usrInfo.name, usrInfo.createTime, usrInfo.msg); // Insert the history into the database
				// printf("curr name: %s\n", usrInfo.name);
				// printf("curr time: %s\n", usrInfo.createTime);
				// printf("curr msg: %s\n", usrInfo.msg);
				// search_history();// Search history from database

				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket == sub_sock) {
						strcpy_s(usrInfo.name, sizeof usrInfo.name, clients[i].name);
					}
				}
				
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						msg_len = send(clients[i].client_socket, (char*)&usrInfo, BufferSize, 0);

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

			if (strcmp(usrInfo.type, "SEARCH") == 0) {
				// Because time is always returned from client, there is only four cases to judge.

				printf("Msg from search request --- name: %s\n", usrInfo.searchMsg.search_name);
				printf("Msg from search request --- content: %s\n", usrInfo.searchMsg.search_content);
				printf("Msg from search request --- time: %s\n", usrInfo.searchMsg.search_time);

				if (strcmp(usrInfo.searchMsg.search_name,"") == 0){
					// The case that only search by date
					if(strcmp(usrInfo.searchMsg.search_content,"") == 0){
						search_by_date(usrInfo.searchMsg.search_time,sub_sock,usrInfo);
					}
					// The case that only search by date and content
					else{
						
					}
				}else{
					if(strcmp(usrInfo.searchMsg.search_content,"") == 0){
	
					}
				}
				// printf("Msg from search request --- name: %s\n", usrInfo.searchMsg.search_name);
				// printf("Msg from search request --- content: %s\n", usrInfo.searchMsg.search_content);
				// printf("Msg from search request --- time: %s\n", usrInfo.searchMsg.search_time);
			}



		}

		for (int i = 0; i < MAX_ALLOWED; i++) {
			printf("---usrInfo.online.uid: %d\n", usrInfo.onlineList[i].uid);
		}

		// if client closes socket, clear its information in client array
		for (unsigned i = 0; i < MAX_ALLOWED; i++) {
			if (clients[i].client_socket == sub_sock) {
				clients[i].fd = 0;
				clients[i].client_socket = INVALID_SOCKET;
				memset(clients[i].name, 0, sizeof clients[i].name);
			}
		}
		
		for (unsigned i = 0; i < MAX_ALLOWED; i++) {
			if (clients[i].client_socket == INVALID_SOCKET) {
				usrInfo.onlineList[i].uid = -1;
				memset(usrInfo.onlineList[i].name, 0, sizeof usrInfo.onlineList[i].name);
			}
		}

		memcpy(usrInfo.type, "QUIT", sizeof usrInfo.type);
		for (int i = 0; i < MAX_ALLOWED; i++) {
			printf("usrInfo.online.uid: %d\n", usrInfo.onlineList[i].uid);
		}

		for (unsigned i = 0; i < MAX_ALLOWED; i++) {
			if (clients[i].client_socket != INVALID_SOCKET) {
				msg_len = send(clients[i].client_socket, (char*)&usrInfo, BufferSize, 0);
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
		// init clients
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