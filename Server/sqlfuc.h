#include <mysql.h>  // for database functions

#include <cstdio>

#include "server.h"

void print_table(MYSQL_RES *res);
void insert_into_group(char user_name[], char creat_time[], char content[], int rid);
void insert_into_private(char sender[], char creat_time[], char content[], char receiver[]);
void search_private_by_name(char sender[], char receiver[]);
void search_group_by_name(char user_name[], int rid);
void search_private_by_content(char sender[], char receiver[], char content[]);
void search_group_by_content(char content[], int rid);
void search_private_by_date(char sender[], char receiver[], char date[]);
void search_group_by_date(char date[], int rid, SOCKET socks, usrData usrInfo);
void search_private_history();
void search_gourp_history();
void user_sign_up(char user_name[], char pwd[]);
void add_room(char admin[], char room_name[]);
void add_mem(int rid, char mem_name[]);
void add_private_chat(char sender[], char creat_time[], char content[], char recevier[]);
void check_login(char user_name[], char pwd[]);