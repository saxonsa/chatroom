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

// insert gourp info 给群聊添加历史记录
void insert_into_group(char user_name[], char creat_time[], char content[], int rid){
	char toInsertHistory[1000] = "Insert INTO group_history(user_name,create_time,content,room_name) VALUES('";
	
	char str_rid[10] = { 0 };
	itoa(rid, str_rid, 10);

	// Concat strings
	strcat_s(toInsertHistory,sizeof toInsertHistory,user_name);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,creat_time);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,content);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"',");
	strcat_s(toInsertHistory,sizeof toInsertHistory,str_rid);
	strcat_s(toInsertHistory,sizeof toInsertHistory,");");

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

// insert private info 给私聊添加历史记录
void insert_into_private(char sender[], char creat_time[], char content[], char receiver[]){
	char toInsertHistory[1000] = "Insert INTO private_history VALUES('";

	// Concat strings
	strcat_s(toInsertHistory,sizeof toInsertHistory,sender);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,creat_time);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,content);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,receiver);
	strcat_s(toInsertHistory,sizeof toInsertHistory,"');");

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

// Use keyword to search
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

	strcpy_s(finalString, sizeof finalString, toSearchByKeyword);

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
			int column = (int)mysql_num_fields(res);
			int row = (int)mysql_num_rows(res);
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

// Use name to search
void search_by_name(char user_name[]){
	char toSearchByName[250] = "SELECT * FROM `history` WHERE user_name = ";
	char userName[1000] = "'";
	strcat_s(userName,sizeof userName,user_name);

	// char userName[] = "'David'";

	char finalString[300];
	
	// Concat strings
	strcat_s(toSearchByName,sizeof toSearchByName,userName);
	strcat_s(toSearchByName,sizeof toSearchByName,"';");

	strcpy_s(finalString, sizeof finalString, toSearchByName);

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

// Use content to search
void search_by_content(char content[]){
	char toSearchByContent[250] = "SELECT * FROM `history` WHERE content LIKE "; // vague query
	char searchContent[1000] = "'%";
	strcat_s(searchContent,sizeof searchContent,content);

	char finalString[300];
	
	// Concat strings
	strcat_s(toSearchByContent,sizeof toSearchByContent,searchContent);
	strcat_s(toSearchByContent,sizeof toSearchByContent,"%';");

	strcpy_s(finalString, sizeof finalString, toSearchByContent);

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

// Use date to search
void search_by_date(char date[]){
	char toSearchByDate[250] = "SELECT * FROM `history` WHERE DATE_FORMAT(create_time, '%Y%m%d') = '";

	char finalString[300];
	
	// Concat strings
	strcat_s(toSearchByDate,sizeof toSearchByDate,date);
	strcat_s(toSearchByDate,sizeof toSearchByDate,"';");

	strcpy_s(finalString, sizeof finalString, toSearchByDate);
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

// 这个函数有问题，找的这个history是群聊的还是私聊的？？？？
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
			int row = (int)mysql_num_rows(res);
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

// for user to sign up 注册函数
void user_sign_up(char user_name[], char pwd[]){
	char signUpInfo[250] = "INSERT INTO users VALUES('";

	// connect sql
	strcat_s(signUpInfo,sizeof signUpInfo,user_name);
	strcat_s(signUpInfo,sizeof signUpInfo,"','");
	strcat_s(signUpInfo,sizeof signUpInfo,pwd);
	strcat_s(signUpInfo,sizeof signUpInfo,"');");

	ret = mysql_query(&mysqlConnect, signUpInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("User sign up failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	res = mysql_store_result(&mysqlConnect);// Check the res value

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

// create new chatroom
void add_room(char admin[], char room_name[]){
	char addRoomInfo[250] = "INSERT INTO room_info(administrator,room_name) VALUES('";

	// connect sql
	strcat_s(addRoomInfo,sizeof addRoomInfo,admin);
	strcat_s(addRoomInfo,sizeof addRoomInfo,"','");
	strcat_s(addRoomInfo,sizeof addRoomInfo,room_name);
	strcat_s(addRoomInfo,sizeof addRoomInfo,"');");

	ret = mysql_query(&mysqlConnect, addRoomInfo); // Pass the query to database

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

// add a member to a chat room 往特定chatroom里面加人
void add_mem(int rid, char mem_name[]){
	char addMemberInfo[250] = "INSERT INTO room_mem VALUES(";

	char str_rid[10] = { 0 };
	itoa(rid, str_rid, 10);

	// connect sql
	strcat_s(addMemberInfo,sizeof addMemberInfo,str_rid);
	strcat_s(addMemberInfo,sizeof addMemberInfo,",'");
	strcat_s(addMemberInfo,sizeof addMemberInfo,mem_name);
	strcat_s(addMemberInfo,sizeof addMemberInfo,"');");

	ret = mysql_query(&mysqlConnect, addMemberInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Add member failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	res = mysql_store_result(&mysqlConnect);// Check the res value
	
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

// add a private chat 私聊
void add_private_chat(char sender[], char creat_time[], char content[], char recevier[]){
	char addPrivateInfo[250] = "INSERT INTO private_history VALUES('";

	// connect sql
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,sender);
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,"','");
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,creat_time);
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,"','");
	strcat_s(toInsertHistory,sizeof toInsertHistory,content);
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,"','");
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,recevier);
	strcat_s(addPrivateInfo,sizeof addPrivateInfo,"');");

	ret = mysql_query(&mysqlConnect, addPrivateInfo); // Pass the query to database

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

// function to accept a connection
void accept_conn(void *dummy) {
		SOCKET sub_sock = (SOCKET) dummy;

		connecting++;
		printf("current number of clients: %d\n", connecting);

		usrData usrInfo;

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

			if (usrInfo.name) {
				printf("Bytes Received: %d, message: %s from name: %s\n", msg_len, usrInfo.msg, usrInfo.name);
			} else {
				printf("Bytes Received: %d, message: %s from IP: %s\n", msg_len, usrInfo.msg, inet_ntoa(client_addr.sin_addr));
			}
			

			/* Send message back to Client */

			if (strcmp(usrInfo.type, "ENTER") == 0) {
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

				// broadcast name msg depends on different clients
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						if (clients[i].client_socket == sub_sock) {
							strcpy_s(usrInfo.msg, sizeof usrInfo.msg, enterMsgSelf);
							msg_len = send(clients[i].client_socket, (char*)&usrInfo, 1500, 0);

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
							msg_len = send(clients[i].client_socket, (char*)&usrInfo, 1500, 0);

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




				// 还需要传入chatroom的名字
				//insert_into_database(usrInfo.name, usrInfo.createTime, usrInfo.msg); // Insert the history into the database
				
				
				
				
				printf("curr name: %s\n", usrInfo.name);
				printf("curr time: %s\n", usrInfo.createTime);
				printf("curr msg: %s\n", usrInfo.msg);
				search_history();// Search history from database

				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket == sub_sock) {
						strcpy_s(usrInfo.name, sizeof usrInfo.name, clients[i].name);
					}
				}
				
				for (int i = 0; i < MAX_ALLOWED; i++) {
					if (clients[i].client_socket != INVALID_SOCKET) {
						msg_len = send(clients[i].client_socket, (char*)&usrInfo, 1500, 0);

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
		// if client closes socket, clear its information in client array
		for (unsigned i = 0; i < MAX_ALLOWED; i++) {
			if (clients[i].client_socket == sub_sock) {
				clients[i].fd = 0;
				clients[i].client_socket = INVALID_SOCKET;
				memset(clients[i].name, 0, sizeof clients[i].name);
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