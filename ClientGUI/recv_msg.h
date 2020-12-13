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
//              printf("server closed connection\n");
              closesocket(connect_sock);
              WSACleanup();
              qDebug() << "server closed connection\n";
              return;
            }

            memcpy(&usr, szBuff, sizeof szBuff);


            if (strcmp(usr.type, "ENTER") == 0) {
                strcat(chatMsg, usr.msg);
                emit enter_success(chatMsg,usr.onlineList);
            } else if (strcmp(usr.type, "CHAT") == 0) {
                sprintf(chatMsg,"%s%s: %s",usr.createTime,usr.name,usr.msg);
                emit recv_success(chatMsg);
            } else if (strcmp(usr.type, "SEARCH") == 0){
                sprintf(chatMsg,"%s\n%s: %s",usr.searchMsg.search_time,usr.searchMsg.search_name,usr.searchMsg.search_content);
                emit search_success(chatMsg);
            }
//            emit recv_success(chatMsg);
        }
        return;
    }

signals:
    void recv_success(QString);
    void enter_success(QString, nameList*);
    void search_success(QString);
};

#endif // TEST_CONNECTION_H
