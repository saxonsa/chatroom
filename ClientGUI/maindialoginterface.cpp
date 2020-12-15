#include "maindialoginterface.h"
#include "ui_maindialoginterface.h"
#include "mainwindow.h"

#include "client.h"
#include <QString>
#include <QtDebug>
#include <QTextCodec>
#include <string>
#include <QStringListModel>
#include <QStandardItemModel>
using namespace std;


MainDialogInterface::MainDialogInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDialogInterface)
{
    ui->setupUi(this);

    searchHistory = new SearchHistory;

    connect(this, SIGNAL(sendSignalToSearch(QString)),searchHistory,SLOT(recv_From_Main_Dialog(QString)));
    connect(ui->onlineList,SIGNAL(clicked(QModelIndex)),this,SLOT(showClickedPersonName(QModelIndex)));
    connect(this,SIGNAL(sendSignalToChangeName(QString)),this,SLOT(changeChatRoomName(QString)));
    connect(this,SIGNAL(sendSignalToChangeName(QString)),searchHistory,SLOT(change_private_his_name(QString)));
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

void MainDialogInterface::displayOnlineList(QString data, nameList *groupList) {

    for (int i = 0; i < MAX_ROOM; i++) {
        qDebug() << groupList[i].name;
    }

    if (strcmp(usr.type, "ENTER") == 0) {
        receiveData(data);
    }
    QStringList usrOnlineList;
    for (int i = 0; i < MAX_ALLOWED; i++) {
        if (usr.onlineList[i].uid != -1 && strcmp(usr.onlineList[i].name, name) != 0) {
            usrOnlineList << usr.onlineList[i].name;
        }
    }
    QStringListModel *model = new QStringListModel(usrOnlineList);
    ui->onlineList->setModel(model);

    QStandardItemModel *treeItemModel = new QStandardItemModel(ui->groupList);
    treeItemModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Group Name"));



    for (int i = 0; i < MAX_ROOM; i++) {
        if (groupList[i].uid != -1 && strcmp(groupList[i].name, "") != 0) {
            QStandardItem *treeItem = new QStandardItem(groupList[i].name);
            treeItemModel->appendRow(treeItem);
        }
    }

    ui->groupList->setModel(treeItemModel);
}


void MainDialogInterface::on_History_clicked()
{
    searchHistory->show();
}


void MainDialogInterface::recvSignalToSearch(QString data){
    emit sendSignalToSearch(data);
}

void MainDialogInterface::showClickedPersonName(QModelIndex index){

    QString strTemp; // click person in online list
    strTemp = index.data().toString();
    usr.room = 0;

    emit sendSignalToChangeName(strTemp);


    char* recv_name;
    QByteArray recv_name_byte = strTemp.toLocal8Bit();
    recv_name = recv_name_byte.data();

    memcpy(usr.name, name, sizeof usr.name);
    memcpy(usr.recv_name, recv_name, sizeof szBuff);
    memcpy(usr.type, "SWITCH_PRIVATE_CHAT", sizeof usr.type);

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

}

void MainDialogInterface::changeChatRoomName(QString name){
    this->setWindowTitle("UChat chatting with @ " + name);
    curSelectRmNum =  0;
}
