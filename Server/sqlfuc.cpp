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
      "Insert INTO group_history(user_name,create_time,content,room_name) "
      "VALUES('%s','%s','%s',%d);",
      user_name, creat_time, content, rid);

  printf("%s \n", user_name);

  ret = mysql_query(&mysqlConnect,
                    toInsertHistory);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
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

  ret = mysql_query(&mysqlConnect,
                    toInsertHistory);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
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

  ret =
      mysql_query(&mysqlConnect, toSearchByName);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
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
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
    // return;
  }

  // Check the res value
  print_table(mysql_store_result(&mysqlConnect));

  return;
}

void search_private_by_content(char sender[], char receiver[], char content[]) {
  char toSearchByContent[250];  // vague query

  sprintf_s(toSearchByContent,
            "SELECT * FROM `private_history` WHERE user_name = '%s' AND "
            "recevie_name = '%s' AND content LIKE '%s';",
            sender, receiver, content);

  ret = mysql_query(&mysqlConnect,
                    toSearchByContent);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
    // return;
  }

  // Check the res value
  print_table(mysql_store_result(&mysqlConnect));

  return;
}

void search_group_by_content(char content[], int rid) {
  char toSearchByContent[250];  // vague query

  sprintf_s(
      toSearchByContent,
      "SELECT * FROM `group_history` WHERE rid = %d AND content LIKE '%s';",
      rid, content);

  ret = mysql_query(&mysqlConnect,
                    toSearchByContent);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
    // return;
  }

  // Check the res value
  print_table(mysql_store_result(&mysqlConnect));

  return;
}

void search_private_by_date(char sender[], char receiver[], char date[]) {
  char toSearchByDate[250];

  sprintf_s(
      toSearchByDate,
      "SELECT * FROM `private_history` WHERE user_name = '%s' AND recevie_name "
      "= '%s' AND DATE_FORMAT(create_time, '%Y%m%d') = '%s';",
      sender, receiver, date);

  ret =
      mysql_query(&mysqlConnect, toSearchByDate);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
    // return;
  }

  // Check the res value
  print_table(mysql_store_result(&mysqlConnect));

  return;
}

void search_group_by_date(char date[], int rid, SOCKET socks, usrData usrInfo) {
  char toSearchByDate[250];

  sprintf_s(toSearchByDate,
            "SELECT * FROM `group_history` WHERE rid = %d AND "
            "DATE_FORMAT(create_time, '%Y-%m-%d') = '%s';",
            rid, date);

  ret =
      mysql_query(&mysqlConnect, toSearchByDate);  // Pass the query to database

  // If the query failed, close the function
  if (ret != 0) {
    printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
    // return;
  }

  // Check the res value
  print_table(mysql_store_result(&mysqlConnect));

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

char* check_login(char user_name[], char pwd[]){
	char loginInfo[250];

	sprintf_s(loginInfo,"SELECT * FROM users WHERE user_name = '%s';",user_name);

	cout << loginInfo << endl;

	ret = mysql_query(&mysqlConnect, loginInfo); // Pass the query to database

	// If the query failed, close the function
	if (ret != 0) {
		printf("Add member failed...Error: %s\n", mysql_error(&mysqlConnect));
		// return;
	}

	MYSQL_RES *res = mysql_store_result(&mysqlConnect);
	MYSQL_FIELD *field; 
	MYSQL_ROW nextRow;

	if (res) {
		int fieldCount = mysql_field_count(&mysqlConnect);
		//Print the result table
		if (fieldCount > 0) {
			nextRow = mysql_fetch_row(res);

			// current status is online
			if (nextRow[2] == "1"){
				//send(clients[i].client_socket, (char*)&usrInfo, BufferSize, 0);
				return "isOnline";
			}

			// check pwd
			if (strcmp(pwd, nextRow[1]) == 0){
				// pwd right
				char updateQuery[250];
				sprintf_s(updateQuery,"UPDATE users SET status = 1 WHERE user_name = '%s';", nextRow[0]);
				ret = mysql_query(&mysqlConnect, updateQuery);
				if (ret != 0) {
					printf("Query failed...Error: %s\n", mysql_error(&mysqlConnect));
					return "Failed";
				}
				return "Success";
			} else {
				// pwd wrong, send back an error message
				return "wrongPwd";
			}

		}
		else {
			// new user sign up 
			//user_sign_up();
			return "new";
		}
	}
	else {
		printf("mysql_store_result...Error: %s\n", mysql_error(&mysqlConnect));
		return "sqlErr";
	}
}