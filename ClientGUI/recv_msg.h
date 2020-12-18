#ifndef TEST_CONNECTION_H
#define TEST_CONNECTION_H

#include "client.h"
#include "maindialoginterface.h"
#include <QThread>
#include <QtDebug>
#include <string>
#include <iostream>
using namespace std;

class Reciever : public QThread
{
    Q_OBJECT
    void run() override{
        while (1) {
            char chatMsg[] = "";

            msg_len = recv(connect_sock, szBuff, sizeof szBuff, 0);

            if (msg_len == SOCKET_ERROR) {
              fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
              closesocket(connect_sock);
              WSACleanup();
              qDebug() << SOCKET_ERROR;
              return;
            }

            if (msg_len == 0) {
              closesocket(connect_sock);
              WSACleanup();
              qDebug() << "server closed connection\n";
              return;
            }

            memcpy(&usr, szBuff, sizeof szBuff);
            if (strcmp(usr.type, "QUIT") != 0 && strcmp(usr.name, name) == 0) {
                for (int i = 0; i < MAX_ROOM; i++) {
                    memcpy(groupList[i].name, usr.groupList[i].name, sizeof groupList[i].name);
                    groupList[i].uid = usr.groupList[i].uid;
                }
            }

            if (strcmp(usr.type, "ENTER") == 0 || strcmp(usr.type, "QUIT") == 0) {
            //                strcat(chatMsg, usr.msg);                
                emit enter_success((nameList*)&groupList, usr.enter,isFirstTime); //
                if (isFirstTime == 0) isFirstTime = 1;
            } else if (strcmp(usr.type, "CHAT") == 0) {                
                if(usr.room == -1){
                    sprintf(chatMsg,"%s\n%s: %s",usr.createTime,usr.name,usr.msg);
                    qDebug() << "private:" << chatMsg;
                    emit recv_success(chatMsg,usr.name,usr.recv_name);
                }else{
                    sprintf(chatMsg,"%s\n%s: %s",usr.createTime,usr.name,usr.msg);
                    qDebug() << "room:" << chatMsg << usr.room_name;
                    emit recv_room_success(chatMsg,usr.room_name);
                }
            } else if (strcmp(usr.type, "SEARCH") == 0 || strcmp(usr.type, "SEARCHROOM") == 0){
                if (strcmp(usr.type, "SEARCH") == 0)
                    sprintf(chatMsg,"%s\n%s: %s",usr.searchMsg.search_time,usr.searchMsg.search_name,usr.searchMsg.search_content);
                else sprintf(chatMsg,"%s\n%s: %s",usr.searchMsg.search_time,usr.searchMsg.search_name,usr.searchMsg.search_content);
                qDebug() << chatMsg;
                emit search_success(chatMsg);
            } else if (strcmp(usr.type, "Login") == 0){
                emit login_success(usr.name);
            } else if (strcmp(usr.type, "LoginF") == 0){
                emit login_failed(usr.msg);
                return;
            }
        }
        return;
    }

signals:
    void recv_success(QString,QString,QString);
    void recv_room_success(QString,QString);
    void enter_success(nameList*,char*,int);
    void search_success(QString);
    void login_success(char*);
    void login_failed(char*);
private:
    int isFirstTime = 0;
};

#endif // TEST_CONNECTION_H
