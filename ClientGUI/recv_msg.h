#ifndef TEST_CONNECTION_H
#define TEST_CONNECTION_H

#include "client.h"
#include "maindialoginterface.h"
#include <QThread>
#include <QtDebug>
#include <string>
using namespace std;

class Reciever : public QThread
{
    Q_OBJECT
    void run() override{
        while (1) {
            char *chatMsg = "";
//            usrData recvData;
//            qDebug()<< "I am inside the thread!";
//            qDebug()<< connect_sock;
            msg_len = recv(connect_sock, szBuff, sizeof(szBuff), 0);

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

            strcat(chatMsg, usr.createTime);
            strcat(chatMsg, "\n");
            strcat(chatMsg, usr.name);
            strcat(chatMsg, ":");
            strcat(chatMsg, usr.msg);

            emit recv_success(chatMsg);
        }
        return;
    }

signals:
    void recv_success(QString);
};

#endif // TEST_CONNECTION_H
