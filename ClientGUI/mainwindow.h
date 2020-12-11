#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QString>
#include "maindialoginterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MainDialogInterface *mainDialog = NULL;

private slots:
    void on_EnterBtn_clicked();

    void on_ExitBtn_clicked();

signals:
    void sendData(QString);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
