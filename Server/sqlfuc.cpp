#include "sqlfuc.h"
#include <iostream>

using namespace std;

void print_table(MYSQL_RES *res) {
	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		// Print the result table
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
		} else {
			printf(
				"No resullt. This is the result of a character splitting query... "
				"\n");
		}
	} else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return;
}

void insert_into_group(char user_name[], char creat_time[], char content[], int rid) {
	char toInsertHistory[1000];

	sprintf_s(toInsertHistory,
		"Insert INTO group_history(user_name,create_time,content,rid) "
		"VALUES('%s','%s','%s',%d);",
		user_name, creat_time, content, rid);

	printf("%s \n", user_name);

	ret = mysql_query(&mysqlConnect, toInsertHistory);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("insert_into_group failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// A hint that shows the insertion is complte
	// printf("History record succeed! \n");

	return;
}

void insert_into_private(char sender[], char creat_time[], char content[], char receiver[]) {
	char toInsertHistory[1000];

	// Concat strings
	sprintf_s(toInsertHistory,
		"Insert INTO private_history VALUES('%s','%s','%s','%s');", sender,
		creat_time, content, receiver);

	ret = mysql_query(&mysqlConnect, toInsertHistory);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("insert_into_private failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// A hint that shows the insertion is complte
	// printf("History record succeed! \n");

	return;
}

void search_private_by_name(char sender[], char receiver[]) {
	char toSearchByName[250];

	sprintf_s(toSearchByName,
		"SELECT * FROM `private_history` WHERE user_name = '%s' AND "
		"recevie_name = '%s';",
		sender, receiver);

	ret = mysql_query(&mysqlConnect, toSearchByName);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_private_by_name failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

void search_group_by_name(char user_name[], int rid) {
	char toSearchByName[250];

	sprintf_s(
		toSearchByName,
		"SELECT * FROM `group_history` WHERE user_name = '%s' AND rid = %d;",
		user_name, rid);

	ret =
		mysql_query(&mysqlConnect, toSearchByName);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_group_by_name failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

void search_private_by_content(char* sender, SOCKET socks, usrData usrInfo) {
	char toSearchByContent[250];  // vague query

	sprintf_s(toSearchByContent,"SELECT * FROM `private_history` WHERE (user_name = '%s' AND "
		"recevie_name = '%s' OR user_name = '%s' AND recevie_name = '%s') AND "
		"DATE_FORMAT(create_time, '%%Y-%%m-%%d') = '%s' AND content LIKE '%%%s%%' ORDER BY create_time;",
		sender, usrInfo.searchMsg.search_name, usrInfo.searchMsg.search_name,
		sender, usrInfo.searchMsg.search_time,usrInfo.searchMsg.search_content);

	ret = mysql_query(&mysqlConnect,toSearchByContent);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_private_by_content failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	res = mysql_store_result(&mysqlConnect);  // Check the res value

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		// Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = (int)mysql_num_rows(res);

			printf("\n");
			while (nextRow = mysql_fetch_row(res)) {
				for (int j = 0; j < column; j++) {
					printf("%25s", nextRow[j]);
					printf(" |");
					switch (j) {
					case 0:
						memcpy(usrInfo.searchMsg.search_name, nextRow[0],
							sizeof usrInfo.searchMsg.search_name);
						break;
					case 1:
						memcpy(usrInfo.searchMsg.search_time, nextRow[1],
							sizeof usrInfo.searchMsg.search_time);
						break;
					case 2:
						memcpy(usrInfo.searchMsg.search_content, nextRow[2],
							sizeof usrInfo.searchMsg.search_content);
						break;
					}
				}
				send(socks, (char*)&usrInfo, sizeof usrInfo, 0);
				printf("\n");
			}
		} else {
			printf(
				"No result. This is the result of a character splitting query... \n");
		}
	} else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}  

	return;
}

void search_group_by_content(char content[], char date[], char room_name[], SOCKET socks, usrData usrInfo) {
	char toSearchByContent[250];  // vague query

	int rid = get_room_id(room_name);

	sprintf_s( toSearchByContent, "SELECT * FROM `group_history` WHERE rid = %d AND (content LIKE '%s' OR user_name = '%s');", rid, content, content);

	ret = mysql_query(&mysqlConnect, toSearchByContent);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_group_by_content failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	// print_table(mysql_store_result(&mysqlConnect));

	res = mysql_store_result(&mysqlConnect);  // Check the res value

    if (res) {
          int fieldCount = mysql_field_count(&mysqlConnect);
          // Print the result table
          if (fieldCount > 0) {
            int column = mysql_num_fields(res);
            int row = (int)mysql_num_rows(res);
            // usrData hisRes;
            for (int i = 0; field = mysql_fetch_field(res); i++) {
              printf("%25s", field->name);
              printf(" |");
            }
            printf("\n");
            while (nextRow = mysql_fetch_row(res)) {
              for (int j = 0; j < column; j++) {
                printf("%25s", nextRow[j]);
                printf(" |");
                switch (j) {
                  case 1:
                    memcpy(usrInfo.searchMsg.search_name, nextRow[1],
                           sizeof usrInfo.searchMsg.search_name);
                    break;
                  case 2:
                    memcpy(usrInfo.searchMsg.search_time, nextRow[2],
                           sizeof usrInfo.searchMsg.search_time);
                    break;
                  case 3:
                    memcpy(usrInfo.searchMsg.search_content, nextRow[3],
                           sizeof usrInfo.searchMsg.search_content);
                    break;
                }
              }
              send(socks, (char *)&usrInfo, sizeof usrInfo, 0);
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

// void search_private_by_date(char sender[], char receiver[], char date[]) {
void search_private_by_date(char* sender, SOCKET socks, usrData usrInfo) {
	char toSearchByDate[250];

	sprintf_s(toSearchByDate, "SELECT * FROM `private_history` WHERE (user_name = '%s' AND recevie_name = '%s' "
		"OR user_name = '%s' AND recevie_name = '%s') AND DATE_FORMAT(create_time, '%%Y-%%m-%%d') = '%s' ORDER BY create_time;",
		sender, usrInfo.searchMsg.search_name, usrInfo.searchMsg.search_name, sender, usrInfo.searchMsg.search_time);

	ret = mysql_query(&mysqlConnect, toSearchByDate);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_private_by_date failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}


	res = mysql_store_result(&mysqlConnect);  // Check the res value

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		// Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = (int)mysql_num_rows(res);

			printf("\n");
			while (nextRow = mysql_fetch_row(res)) {
				for (int j = 0; j < column; j++) {
					printf("%25s", nextRow[j]);
					printf(" |");
					switch (j) {
					case 0:
						memcpy(usrInfo.searchMsg.search_name, nextRow[0],
							sizeof usrInfo.searchMsg.search_name);
						break;
					case 1:
						memcpy(usrInfo.searchMsg.search_time, nextRow[1],
							sizeof usrInfo.searchMsg.search_time);
						break;
					case 2:
						memcpy(usrInfo.searchMsg.search_content, nextRow[2],
							sizeof usrInfo.searchMsg.search_content);
						break;
					}
				}
				send(socks, (char*)&usrInfo, sizeof usrInfo, 0);
				printf("\n");
			}
		}
		else {
			printf("No result. This is the result of a character splitting query... \n");
		}
	}
	else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return;

}

void search_group_by_date(char date[], char room_name[], SOCKET socks, usrData usrInfo) {
	char toSearchByDate[250];

	int rid = get_room_id(room_name);

	sprintf_s(toSearchByDate,"SELECT * FROM `group_history` WHERE rid = %d AND "
		"DATE_FORMAT(create_time, '%%Y-%%m-%%d') = '%s';", rid, date);
	
	cout << toSearchByDate << endl;

	ret = mysql_query(&mysqlConnect, toSearchByDate);  // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_group_by_date failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	//print_table(mysql_store_result(&mysqlConnect));

	res = mysql_store_result(&mysqlConnect);  // Check the res value

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		// Print the result table
		if (fieldCount > 0) {
			int column = mysql_num_fields(res);
			int row = (int)mysql_num_rows(res);
			// usrData hisRes;
			for (int i = 0; field = mysql_fetch_field(res); i++) {
				printf("%25s", field->name);
				printf(" |");
			}
			printf("\n");
			while (nextRow = mysql_fetch_row(res)) {
				for (int j = 0; j < column; j++) {
					printf("%25s", nextRow[j]);
					printf(" |");
					switch (j) {
					case 1:
						memcpy(usrInfo.searchMsg.search_name, nextRow[1],
							sizeof usrInfo.searchMsg.search_name);
						break;
					case 2:
						memcpy(usrInfo.searchMsg.search_time, nextRow[2],sizeof usrInfo.searchMsg.search_time);
						break;
					case 3:
						memcpy(usrInfo.searchMsg.search_content, nextRow[3],
							sizeof usrInfo.searchMsg.search_content);
						break;
					}
				}
				send(socks, (char *)&usrInfo, sizeof usrInfo, 0);
				printf("\n");
			}
		} else {
			printf(
				"No resullt. This is the result of a character splitting query... \n");
		}
	} else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return;
}

void search_private_history(){
	// The selection query
	ret = mysql_query(&mysqlConnect, "SELECT * FROM `private_history`;");

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_private_history failed...Error: %s\n", mysql_error(&mysqlConnect));
		return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

void search_gourp_history(){
	// The selection query
	ret = mysql_query(&mysqlConnect, "SELECT * FROM `group_history`;");

	// If the query failed, close the function
	if (ret != 0) {
		printf("search_gourp_history failed...Error: %s\n", mysql_error(&mysqlConnect));
		return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

void user_sign_up(char user_name[], char pwd[]){
	char signUpInfo[250];

	// connect sql, when user signs up, he/her logins in immediately
	// set status 1
	sprintf_s(signUpInfo,"INSERT INTO users VALUES('%s','%s',%d);"
		,user_name
		,pwd
		,1);

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

void add_room(char admin[], char room_name[]){
	char addRoomInfo[250];

	// connect sql
	sprintf_s(addRoomInfo,"INSERT INTO room_info(administrator,room_name) VALUES('%s','%s');",admin ,room_name);

	ret = mysql_query(&mysqlConnect, addRoomInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("add_room failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

void add_mem(int rid, char mem_name[]){
	char addMemberInfo[250];

	sprintf_s(addMemberInfo,"INSERT INTO room_mem VALUES(%d,'%s');",rid,mem_name);

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

void add_private_chat(char sender[], char creat_time[], char content[], char recevier[]){
	char addPrivateInfo[250];

	sprintf_s(addPrivateInfo,"INSERT INTO private_history VALUES('%s','%s','%s','%s');"
		,sender
		,creat_time
		,content
		,recevier);

	ret = mysql_query(&mysqlConnect, addPrivateInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("add_private_chat failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	// Check the res value
	print_table(mysql_store_result(&mysqlConnect));

	return;
}

char* check_login(char user_name[], char pwd[]){
	char loginInfo[250];

	sprintf_s(loginInfo,"SELECT * FROM users WHERE user_name = '%s';",user_name);

	ret = mysql_query(&mysqlConnect, loginInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("LOGIN message select failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);
	MYSQL_ROW nextRow;

	if (res) {
		//Print the result table
		if (res->row_count > 0) {
			nextRow = mysql_fetch_row(res);

			// check current user is online
			if (strcmp(nextRow[2],"1") == 0){
				// current user is already online
				return "isOnline";
			}

			// check pwd
			if (strcmp(pwd, nextRow[1]) == 0){
				// pwd right
				return set_user_status(nextRow[0], 1);
			} else {
				// pwd wrong
				return "wrongPwd";
			}
		}
		else {
			// new user sign up 
			user_sign_up(user_name, pwd);
			return "new";
		}
	}
	else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
		return "sqlErr";
	}
}

char* set_user_status(char user_name[], int status) {
	char updateQuery[250];
	sprintf_s(updateQuery,"UPDATE users SET status = %d WHERE user_name = '%s';", status, user_name);
	ret = mysql_query(&mysqlConnect, updateQuery);
	if (ret != 0) {
		printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
		return "Failed";
	}
	return "Success";
}

char** get_room_name(char user_name[]){
	char roomNameInfo[300];

	sprintf_s(roomNameInfo,"SELECT `room_name` FROM room_info NATURAL JOIN room_mem WHERE user_name = '%s';",user_name);

	ret = mysql_query(&mysqlConnect, roomNameInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("get_room_name failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);
	MYSQL_ROW nextRow = NULL;

	if (res) {
		// check if the room is empty
		if (res->row_count > 0) {
			// room is not empty
			char** nameList = (char**)malloc((int)res->row_count * sizeof(char*));
			// nextRow = mysql_fetch_row(res);
			for (int i = 0; i < res->row_count; i++){
				nextRow = mysql_fetch_row(res);
				nameList[i] = nextRow[0];
				row_num++;
			}
			return nameList;
		} else {
			// room is empty
			printf("room is empty.\n");
			return NULL;
		}
	}
	else {
		printf("get_room_name mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return NULL;
}

int get_room_id(char room_name[]) {
	char roomIdInfo[300];
	sprintf_s(roomIdInfo,"SELECT `rid` FROM `room_info` WHERE room_name = '%s';",room_name);
	ret = mysql_query(&mysqlConnect, roomIdInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) 
	{
		printf("get_room_id failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);
	MYSQL_ROW nextRow = NULL;
	int rid;

	if (res) 
	{
		if (res->row_count > 0) 
		{
			nextRow = mysql_fetch_row(res);
			rid = atoi(nextRow[0]);
			return rid;
		} 
		else 
		{
			printf("room_name does not exist!\n");
			return -1;
		}
	} 
	else 
	{
		printf("get_room_id mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
	}

	return -1;
}

char* is_room_exist(char room_name[]){
	char roomExistInfo[200];

	sprintf_s(roomExistInfo,"SELECT `administrator` FROM `room_info` WHERE room_name = '%s';", room_name);
	
	ret = mysql_query(&mysqlConnect, roomExistInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0)
	{
		printf("is_room_exist failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);
	MYSQL_ROW nextRow = NULL;

	if (res) 
	{
		if (res->row_count > 0)
		{
			nextRow = mysql_fetch_row(res);
			return nextRow[0]; // the room exists, return admin name
		}
		else 
		{
			return "No"; // the room doesn't exist
		}
	} 
	else 
	{
		printf("is_room_exist mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
		return NULL;
	}
}

int is_person_exist(char user_name[]){
	char personExistInfo[200];

	sprintf_s(personExistInfo,"SELECT * FROM `users` WHERE user_name = '%s';", user_name);

	ret = mysql_query(&mysqlConnect, personExistInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0)
	{
		printf("is_room_exist failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);

	if (res) 
	{
		if (res->row_count > 0)
			return 1; // the person exists
		else
			return 0; // the person doesn't exist
	} 
	else 
	{
		printf("is_person_exist mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));	
	}
	return -1;
}