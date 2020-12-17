#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ExitBtn_clicked()
{
    this->close();
}

char* MainWindow::str_handle(char* raw)
{
    char* result = (char*)malloc(2*strlen(raw)*sizeof(char));
    int i = 0, j = 0;
    for(; i < (int)strlen(raw); i++, j++){
        if(raw[i] == '\'' || raw[i]=='"' || raw[i]=='\\'){
            result[j] = '\\';
            j++;
        }
        result[j] = raw[i];
    }
    result[j] = '\0';
    return result;
}

void MainWindow::on_EnterBtn_clicked()
{
    QString ip_address = ui->ip_address->text();
    QString port_number = ui->port_number->text();
    QString name = ui->name->text();
    QString password = ui->pw->text();

    char *ip,*portNum,*userName,*pwd;

    QByteArray ipByte = ip_address.toLocal8Bit();
    QByteArray portNumByte = port_number.toLocal8Bit();
    QByteArray userNameByte = name.toLocal8Bit();
    QByteArray pwdByte = password.toLocal8Bit();

    ip = ipByte.data();
    portNum = portNumByte.data();
    userName = userNameByte.data();
    pwd = pwdByte.data();

    if(strlen(ip) == 0 || strlen(portNum) == 0 || strlen(userName) == 0 || strlen(pwd) == 0){
        QString dlgTitle="Warning!!!";
        QString strInfo="Input can not be empty";
        QMessageBox::warning(this, dlgTitle, strInfo);
        return;
    }

    if (client_connect(ip,portNum,userName) == 0){

        Reciever *recver = new Reciever();
        connect(recver, SIGNAL(recv_success(QString,QString,QString)),mainDialog,SLOT(receiveData(QString,QString,QString)));
        connect(recver, SIGNAL(recv_room_success(QString,QString)),mainDialog,SLOT(receiveRoomData(QString,QString)));
        connect(recver, SIGNAL(enter_success(nameList*,char*,int)),mainDialog,SLOT(displayOnlineList(nameList*,char*,int)));
        connect(recver, SIGNAL(search_success(QString)),mainDialog,SLOT(recvSignalToSearch(QString)));
        connect(recver, SIGNAL(login_success(char*)),this,SLOT(recv_Login_signal(char*)));
        connect(recver, SIGNAL(login_failed(char*)),this,SLOT(reject_Login_signal(char*)));
        recver->start(); // start the thread

        strcpy(usr.name, str_handle(userName));
        strcpy(usr.pwd, str_handle(pwd));
        strcpy(usr.type, "LOGIN");

        memset(usr.msg, 0, sizeof usr.msg);

        msg_len = send(connect_sock, (char*)&usr, sizeof szBuff, 0);

        if (msg_len == SOCKET_ERROR) {
           fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
           WSACleanup();
        }

        if (msg_len == 0) {
           printf("server closed connection\n");
           closesocket(connect_sock);
           WSACleanup();
        }

    } else {
        // error message
        QString dlgTitle="Warning!!!";
        QString strInfo="Connection fails.";
        QMessageBox::warning(this, dlgTitle, strInfo);
    }

}

void MainWindow::recv_Login_signal(char* usr_name){

    if (strcmp(name, "") == 0) strcpy(name, usr.name);

    strcpy(usr.name, usr_name);

    strcpy(usr.type, "ENTER");

    memset(usr.msg, 0, sizeof usr.msg);

    for (int i = 0; i < MAX_ALLOWED; i++) {
        usr.onlineList[i].uid = -1;
        memset(usr.onlineList[i].name, 0, sizeof usr.onlineList[i].name);
    }
    for (int i = 0; i < MAX_ROOM; i++) {
        usr.groupList[i].uid = -1;
        memset(usr.groupList[i].name, 0, sizeof usr.groupList[i].name);
        groupList[i].uid = -1;
        memset(groupList[i].name, 0, sizeof groupList[i].name);
    }

    msg_len = send(connect_sock, (char*)&usr, sizeof szBuff, 0);

    if (msg_len == SOCKET_ERROR) {
       fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
       WSACleanup();
       return;
    }

    if (msg_len == 0) {
       printf("server closed connection\n");
       closesocket(connect_sock);
       WSACleanup();
       return;
    }
    // Hide the login window
    this->hide();
    // Show the dialog interface
    mainDialog->show();

    mainDialog->setWindowTitle("UChat @ " + QString(name));
}

void MainWindow::reject_Login_signal(char* msg){
    QString dlgTitle="Warning!!!";
    QString strInfo=msg;
    QMessageBox::warning(this, dlgTitle, strInfo);
}
