#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    return a.exec();
}
