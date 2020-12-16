#ifndef MAINDIALOGINTERFACE_H
#define MAINDIALOGINTERFACE_H

#include <QMainWindow>
#include <QModelIndex>
#include <string>

#include "client.h"
#include "searchhistory.h"

namespace Ui
{
  class MainDialogInterface;
}

class MainDialogInterface : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainDialogInterface(QWidget *parent = nullptr);
  ~MainDialogInterface();

  SearchHistory *searchHistory = NULL;

  char* str_handle(char raw[]);

private:
  // public:
  Ui::MainDialogInterface *ui;
  QString CurrentSelectName;
  int showTextEditor = 0;
private slots:
  void receiveData(QString data,QString sname,QString rname);
  void receiveRoomData(QString data,QString room_name);
  void on_Send_clicked();
  void displayOnlineList(QString data, nameList *groupList);
  void on_History_clicked();
  void recvSignalToSearch(QString data);
  void showClickedPersonName(QModelIndex index);
  void changeChatRoomName(QString);

  void showClickedGroupName(QModelIndex index);

signals:
  void sendSignalToSearch(QString);
  void sendSignalToChangeName(QString);
};

#endif // MAINDIALOGINTERFACE_H
