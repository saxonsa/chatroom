#include "maindialoginterface.h"
#include "ui_maindialoginterface.h"
#include "mainwindow.h"

#include "client.h"
#include <QString>
#include <QtDebug>
#include <QTextCodec>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QMessageBox>
using namespace std;

MainDialogInterface::MainDialogInterface(QWidget *parent) : QMainWindow(parent),
                                                            ui(new Ui::MainDialogInterface)
{
  ui->setupUi(this);
  ui->widget_4->hide();

  searchHistory = new SearchHistory;
  inviteGroup = new InviteGroup;

  connect(this, SIGNAL(sendSignalToSearch(QString)), searchHistory, SLOT(recv_From_Main_Dialog(QString)));
  connect(ui->onlineList, SIGNAL(clicked(QModelIndex)), this, SLOT(showClickedPersonName(QModelIndex)));
  connect(this, SIGNAL(sendSignalToChangeName(QString)), this, SLOT(changeChatRoomName(QString)));
  connect(this, SIGNAL(sendSignalToChangeName(QString)), searchHistory, SLOT(change_private_his_name(QString)));
  connect(ui->groupList, SIGNAL(clicked(QModelIndex)), this, SLOT(showClickedGroupName(QModelIndex)));
}

MainDialogInterface::~MainDialogInterface()
{
  delete ui;
  closesocket(connect_sock);
  WSACleanup();
}

void MainDialogInterface::receiveData(QString data, QString sname, QString rname)
{
  QString name_str(name), fileName;
  QDir dir;
  QString path("./history/" + name_str + "/");
  qDebug() << "Namestr (name)" << name;
  if (!dir.exists(path))
  {
    dir.mkpath(path);
  }
  else
  {
    if (rname == CurrentSelectName || sname == CurrentSelectName)
    {
      fileName = CurrentSelectName;
    }
    else {
        fileName = sname;
    }



    QFile file_r(path + fileName + ".txt");
    bool isOKr = file_r.open(QIODevice::ReadOnly);
    QString previous_content;

    if (isOKr == true)
    {
      QTextStream stream_r(&file_r);

      previous_content = stream_r.readAll();

      stream_r.flush();
    }
    file_r.close();

    QFile file(path + fileName + ".txt");
    bool isOKw = file.open(QIODevice::WriteOnly);

    if (isOKw == true)
    {
      QTextStream stream(&file);
      if (previous_content == "")
        stream << data;
      else {
        stream << previous_content<< endl;
        stream << data;
      }
      stream.flush();
    }
    file.close();
    if (rname == CurrentSelectName || sname == CurrentSelectName)
    {
      changeChatRoomName(CurrentSelectName);
    }
  }
}

void MainDialogInterface::receiveRoomData(QString data, QString room_name)
{
  QString name_str(name);
  qDebug() << "now in room:" << name_str << data << room_name;
  QDir dir;
  QString path("./history/" + name_str + "/");
  if (!dir.exists(path))
  {
    dir.mkpath(path);
  }
  else
  {
      QFile file_r(path + room_name + ".txt");
      bool isOKr = file_r.open(QIODevice::ReadOnly);
      QString previous_content;

      if (isOKr == true)
      {
        QTextStream stream_r(&file_r);

        previous_content = stream_r.readAll();

        stream_r.flush();
      }

      file_r.close();

      QFile file(path + room_name + ".txt");
      bool isOKw = file.open(QIODevice::WriteOnly);

      if (isOKw == true)
      {
        QTextStream stream(&file);
        if (previous_content == "")
          stream << data;
        else
        {
          stream << previous_content << endl;
          stream << data;
        }
        stream.flush();
      }
      file.close();
      if (room_name == CurrentSelectName)
      {
        changeChatRoomName(CurrentSelectName);
      }
    }
}

void MainDialogInterface::on_Send_clicked()
{
  // send message to server
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");

  QString buffer = ui->textEditor->toPlainText();
  QString transcoding_buffer = codec->toUnicode(buffer.toStdString().c_str());
  QByteArray transfered_buffer = transcoding_buffer.toLocal8Bit();

    char* transfer = str_handle(transfered_buffer.data());

    if(strlen(transfer) == 0){
        QString dlgTitle="Warning!!!";
        QString strInfo="Input can not be empty";
        QMessageBox::warning(this, dlgTitle, strInfo);
        return;
    }
    memcpy(usr.name, name, sizeof usr.name);
    strcpy(usr.type, "CHAT");
    memcpy(usr.msg, transfer, sizeof usr.msg);
    if (usr.room == -1) {
        memcpy(usr.recv_name, g_recv_name, sizeof usr.recv_name);
    }

  msg_len = send(connect_sock, (char *)&usr, sizeof szBuff, 0);

  if (msg_len == SOCKET_ERROR)
  {
    fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
    WSACleanup();
    qDebug() << "SOCKET_ERROR";
  }

  if (msg_len == 0)
  {
    printf("server closed connection\n");
    closesocket(connect_sock);
    WSACleanup();
    qDebug() << "msg_len = 0";
  }

  // Clear the text editor and reset focus
  ui->textEditor->clear();
  ui->textEditor->setFocus();
}

void MainDialogInterface::displayOnlineList(nameList *groupList)
{

  for (int i = 0; i < MAX_ROOM; i++)
  {
    qDebug() << groupList[i].name;
  }

  QStringList usrOnlineList;
  for (int i = 0; i < MAX_ALLOWED; i++)
  {
    if (usr.onlineList[i].uid != -1 && strcmp(usr.onlineList[i].name, name) != 0)
    {
      usrOnlineList << usr.onlineList[i].name;
    }
  }
  QStringListModel *model = new QStringListModel(usrOnlineList);
  ui->onlineList->setModel(model);

  if (strcmp(usr.type, "QUIT") != 0)
  {
    QStandardItemModel *treeItemModel = new QStandardItemModel(ui->groupList);
    treeItemModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Group Name"));
    for (int i = 0; i < MAX_ROOM; i++)
    {
      if (groupList[i].uid != -1 && strcmp(groupList[i].name, "") != 0)
      {
        QStandardItem *treeItem = new QStandardItem(groupList[i].name);
        treeItemModel->appendRow(treeItem);
      }
    }

    ui->groupList->setModel(treeItemModel);
  }
}

void MainDialogInterface::on_History_clicked()
{
  searchHistory->show();
}

void MainDialogInterface::recvSignalToSearch(QString data)
{
  emit sendSignalToSearch(data);
}

void MainDialogInterface::showClickedPersonName(QModelIndex index)
{

  if (!showTextEditor)
  {
    ui->widget_4->show();
  }

  QString strTemp; // click person in online list
  strTemp = index.data().toString();
  usr.room = -1;

  emit sendSignalToChangeName(strTemp);

  char *recv_name;
  QByteArray recv_name_byte = strTemp.toLocal8Bit();
  recv_name = recv_name_byte.data();

  memcpy(g_recv_name, recv_name, sizeof g_recv_name);
  memcpy(usr.name, name, sizeof usr.name);
  memcpy(usr.recv_name, recv_name, sizeof usr.recv_name);
  memcpy(usr.type, "SWITCH_PRIVATE_CHAT", sizeof usr.type);

  msg_len = send(connect_sock, (char *)&usr, sizeof szBuff, 0);

  if (msg_len == SOCKET_ERROR)
  {
    fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
    WSACleanup();
    qDebug() << "SOCKET_ERROR";
  }

  if (msg_len == 0)
  {
    printf("server closed connection\n");
    closesocket(connect_sock);
    WSACleanup();
    qDebug() << "msg_len = 0";
  }
}

void MainDialogInterface::changeChatRoomName(QString name_select)
{
  this->setWindowTitle("UChat @ " + QString(name) + " chatting with " + name_select);
  CurrentSelectName = name_select;
  curSelectRmNum = -1;

  ui->historyBrowser->clear();

  QString name_str(name);

  QString path("./history/" + name_str + "/");

  QFile file(path + CurrentSelectName + ".txt");

  bool isOK = file.open(QIODevice::ReadOnly);

  if (isOK == true)
  {
    QTextStream stream(&file);

    QString strAll = stream.readAll();

    stream.flush();

    ui->historyBrowser->setText(strAll);

    ui->historyBrowser->moveCursor(QTextCursor::End);
  }
  file.close();
}

void MainDialogInterface::showClickedGroupName(QModelIndex index)
{
  if (!showTextEditor)
  {
    ui->widget_4->show();
  }

  QString clickedRoomName;
  clickedRoomName = index.data().toString();

  usr.room = 0;

  char *room_name;
  QByteArray room_name_byte = clickedRoomName.toLocal8Bit();
  room_name = room_name_byte.data();

  emit sendSignalToChangeName(room_name);

  memcpy(usr.name, name, sizeof usr.name);
  memset(usr.recv_name, 0, sizeof usr.recv_name);
  memcpy(usr.room_name, room_name, sizeof usr.room_name);
  memcpy(usr.type, "SWITCH_GROUP_CHAT", sizeof usr.type);

  msg_len = send(connect_sock, (char *)&usr, sizeof szBuff, 0);
  if (msg_len == SOCKET_ERROR)
  {
    fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
    WSACleanup();
    qDebug() << "SOCKET_ERROR";
  }

  if (msg_len == 0)
  {
    printf("server closed connection\n");
    closesocket(connect_sock);
    WSACleanup();
    qDebug() << "msg_len = 0";
  }
}

char* MainDialogInterface::str_handle(char* raw)
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

void MainDialogInterface::on_Invite_clicked()
{
    inviteGroup->show();
}
