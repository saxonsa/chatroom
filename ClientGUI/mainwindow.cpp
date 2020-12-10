#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maindialoginterface.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include <cstdio>
#include <string>
#include "recv_msg.h"
#include "client.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Create a new scene
    mainDialog = new MainDialogInterface;

//    connect(this,SIGNAL(sendData(QString)),mainDialog,SLOT(receiveData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_EnterBtn_clicked()
{
    QString ip_address = ui->ip_address->text();
    QString port_number = ui->port_number->text();
    QString name = ui->name->text();

    char *ip,*portNum,*userName;

    QByteArray ipByte = ip_address.toLocal8Bit();
    QByteArray portNumByte = port_number.toLocal8Bit();
    QByteArray userNameByte = name.toLocal8Bit();

    ip = ipByte.data();
    portNum = portNumByte.data();
    userName = userNameByte.data();

    if (client_connect(ip,portNum,userName) == 0){
        Reciever *recver = new Reciever();
        connect(recver, SIGNAL(recv_success(QString)),mainDialog,SLOT(receiveData(QString)));
        recver->start(); // start the thread


        strcpy(usr.name, userName);
        strcpy(usr.type, "ENTER");
        strcpy(usr.msg, userName);

//        char msg[1000];
//        string userInfo = "type: 1, content: ";
//        string content(userName);
//        userInfo = userInfo + content;
//        strcpy(msg, userInfo.c_str());

        msg_len = send(connect_sock, (char*)&usr, sizeof usr, 0);
        if (msg_len == SOCKET_ERROR) {
           fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
           WSACleanup();
        }

        if (msg_len == 0) {
           printf("server closed connection\n");
           closesocket(connect_sock);
           WSACleanup();
        }
        // Hide the login window
        this->hide();
        // Show the dialog interface
        mainDialog->show();
    }
}
