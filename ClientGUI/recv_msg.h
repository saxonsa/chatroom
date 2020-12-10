#ifndef TEST_CONNECTION_H
#define TEST_CONNECTION_H

#include "client.h"
#include <QThread>
#include <QtDebug>


class Reciever : public QThread
{
    Q_OBJECT
    void run() override{
        while (1) {
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

//            printf("%s\n", szBuff);

            emit recv_success(szBuff);
        }
        return;
    }

signals:
    void recv_success(QString);
};

#endif // TEST_CONNECTION_H
