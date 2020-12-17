#pragma once

#include <mysql.h>  // for database functions

#include <cstdio>

#include "server.h"

void print_table(MYSQL_RES *res);
void insert_into_group(char user_name[], char creat_time[], char content[], int rid);
void insert_into_private(char sender[], char creat_time[], char content[], char receiver[]);
void search_private_by_name(char sender[], char receiver[]);
void search_group_by_name(char user_name[], int rid);
void search_private_by_content(char* sender, SOCKET socks, usrData usrInfo);
void search_group_by_content(char content[], char date[], char room_name[], SOCKET socks, usrData usrInfo);
void search_private_by_date(char* sender, SOCKET socks, usrData usrInfo);
void search_group_by_date(char date[], char room_name[], SOCKET socks, usrData usrInfo);
void search_private_history();
void search_gourp_history();
void user_sign_up(char user_name[], char pwd[]);
void add_room(char admin[], char room_name[]);
void add_mem(int rid, char mem_name[]);
void add_private_chat(char sender[], char creat_time[], char content[], char recevier[]);
char* check_login(char user_name[], char pwd[]);
char* set_user_status(char user_name[], int status);
// char** get_room_mem(int rid);
char** get_room_name(char user_name[]);
int get_room_id(char room_name[]);
char* is_room_exist(char room_name[]);
int is_person_exist(char user_name[]);