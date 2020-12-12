#ifndef MAINDIALOGINTERFACE_H
#define MAINDIALOGINTERFACE_H

#include <QMainWindow>
#include <string>
#include "client.h"

namespace Ui {
class MainDialogInterface;
}

class MainDialogInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainDialogInterface(QWidget *parent = nullptr);
    ~MainDialogInterface();

private:
//public:
    Ui::MainDialogInterface *ui;
private slots:
    void receiveData(QString data, nameList* onlineList);
    void on_Send_clicked();
    void displayOnlineList(nameList* onlineList);
//    void addOnlineList(std::string name);
//    void deleteOnlineList(std::string name);
};

#endif // MAINDIALOGINTERFACE_H
