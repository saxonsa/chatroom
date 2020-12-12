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
}


MainDialogInterface::~MainDialogInterface()
{
    delete ui;
    closesocket(connect_sock);
    WSACleanup();
}


void MainDialogInterface::receiveData(QString data, nameList* onlineList)
{
    displayOnlineList(onlineList);
    //qDebug() << data;

    // add clinet name to Online List
//    string msg = data.toStdString();
//    string welcome = "Welcome ";
//    string enterMsg = " enters the chatroom!";
//    string name;
//    if (!msg.compare(0, welcome.length(), welcome, 0, welcome.length())) {
//        name = msg.erase(0, welcome.length());
//        addOnlineList(name);
//    }
//    if (msg.length() > enterMsg.length()
//            && !msg.compare(msg.length() - enterMsg.length(), msg.length(), enterMsg, 0, enterMsg.length())
//            && msg.find(":") == string::npos) {
//        name = msg.substr(0, msg.length() - enterMsg.length());
//        addOnlineList(name);
//    }

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

void MainDialogInterface::displayOnlineList(nameList* onlineList) {
    qDebug() << onlineList[0].name;
    QStringList usrOnlineList;
    for (int i = 0; i < MAX_ALLOWED; i++) {
        if (onlineList[i].uid != -1) {
            usrOnlineList << onlineList[i].name;
        }
    }
    QStringListModel *model = new QStringListModel(usrOnlineList);
    ui->onlineList->setModel(model);
}

//void MainDialogInterface::addOnlineList(string name) {
//    QString addName = QString::fromStdString(name);

//    QListWidgetItem* item = new QListWidgetItem;
//    item->setText(addName);
//    ui->onlineList->addItem(item);

//}

//void MainDialogInterface::deleteOnlineList(string name) {
//    QString deleteName = QString::fromStdString(name);

//}

void MainDialogInterface::on_History_clicked()
{
    searchHistory->show();
}
