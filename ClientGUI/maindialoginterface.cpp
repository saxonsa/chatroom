#include "maindialoginterface.h"
#include "ui_maindialoginterface.h"
#include "mainwindow.h"

#include "client.h"
#include <QString>
#include <QtDebug>
#include <QTextCodec>
#include <string>
#include <QStringListModel>
using namespace std;


MainDialogInterface::MainDialogInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDialogInterface)
{
    ui->setupUi(this);

    searchHistory = new SearchHistory;

    connect(this, SIGNAL(sendSignalToSearch(QString)),searchHistory,SLOT(recv_From_Main_Dialog(QString)));
}


MainDialogInterface::~MainDialogInterface()
{
    delete ui;
    closesocket(connect_sock);
    WSACleanup();
}


void MainDialogInterface::receiveData(QString data)
{
    ui->historyBrowser->append(data);
}

void MainDialogInterface::on_Send_clicked()
{
     // send message to server
    QTextCodec *codec =QTextCodec::codecForName("UTF-8");

    QString buffer = ui->textEditor->toPlainText();
    QString transcoding_buffer = codec->toUnicode(buffer.toStdString().c_str());
    QByteArray transfered_buffer = transcoding_buffer.toLocal8Bit();

    char *bufferToString = transfered_buffer.data();

    strcpy(usr.type, "CHAT");
    strcpy(usr.msg, bufferToString);


    // normal msg: type: 0
//    char msg[1000];
//    string chatMsg = "type: 0, content: ";
//    string content(bufferToString);
//    chatMsg = chatMsg + content;
//    strcpy(msg, chatMsg.c_str());

    msg_len = send(connect_sock, (char*)&usr, sizeof szBuff, 0);

    if (msg_len == SOCKET_ERROR) {
      fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
      WSACleanup();
      qDebug() << "SOCKET_ERROR";
    }

    if (msg_len == 0) {
      printf("server closed connection\n");
      closesocket(connect_sock);
      WSACleanup();
      qDebug() << "msg_len = 0";
    }

    // Clear the text editor and reset focus
    ui->textEditor->clear();
    ui->textEditor->setFocus();

}

void MainDialogInterface::displayOnlineList(QString data) {
    if (strcmp(usr.type, "ENTER") == 0) {
        receiveData(data);
    }
    QStringList usrOnlineList;
    for (int i = 0; i < MAX_ALLOWED; i++) {
        if (usr.onlineList[i].uid != -1) {
            usrOnlineList << usr.onlineList[i].name;
        }
    }
    QStringListModel *model = new QStringListModel(usrOnlineList);
    ui->onlineList->setModel(model);
}

void MainDialogInterface::on_History_clicked()
{
    searchHistory->show();

}


void MainDialogInterface::recvSignalToSearch(QString data){
    emit sendSignalToSearch(data);
}
