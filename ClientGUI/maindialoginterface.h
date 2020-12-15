#ifndef MAINDIALOGINTERFACE_H
#define MAINDIALOGINTERFACE_H

#include <QMainWindow>
#include <QModelIndex>
#include <string>

#include "client.h"
#include "searchhistory.h"


namespace Ui {
class MainDialogInterface;
}

class MainDialogInterface : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainDialogInterface(QWidget *parent = nullptr);
  ~MainDialogInterface();

  SearchHistory *searchHistory = NULL;

 private:
  // public:
  Ui::MainDialogInterface *ui;
 private slots:
  void receiveData(QString data);
  void on_Send_clicked();
  void displayOnlineList(QString data, nameList *groupList);
  //    void addOnlineList(std::string name);
  //    void deleteOnlineList(std::string name);
  void on_History_clicked();
  void recvSignalToSearch(QString data);
  void showClickedPersonName(QModelIndex index);

 signals:
  void sendSignalToSearch(QString);
};

#endif  // MAINDIALOGINTERFACE_H
