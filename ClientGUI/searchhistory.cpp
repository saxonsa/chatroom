#include "searchhistory.h"
#include "ui_searchhistory.h"
#include "client.h"
#include <QByteArray>
#include <QDateTime>
#include <QDebug>

SearchHistory::SearchHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchHistory)
{
    ui->setupUi(this);
    ui->Time->setDateTime(QDateTime::currentDateTime());
}

SearchHistory::~SearchHistory()
{
    delete ui;
}

void SearchHistory::on_Exit_clicked()
{
    this->hide();
}

void SearchHistory::on_Search_clicked()
{
    QString name_plain_text = ui->Name->text();
    QString content_plain_text = ui->Content->text();
    QString time_plain_text = ui->Time->text();

    qDebug() << name_plain_text << content_plain_text << time_plain_text;

    char *search_name, *search_content,*search_time;

    QByteArray nameByte = name_plain_text.toLocal8Bit();
    QByteArray contentByte = content_plain_text.toLocal8Bit();
    QByteArray timeByte = time_plain_text.toLocal8Bit();

    search_name = nameByte.data();
    search_content = contentByte.data();
    search_time = timeByte.data();

    strcpy(usr.type, "SEARCH");

    memcpy(usr.searchMsg.search_name,search_name, sizeof usr.searchMsg.search_name);
    memcpy(usr.searchMsg.search_content, search_content, sizeof usr.searchMsg.search_content);
    memcpy(usr.searchMsg.search_time, search_time, sizeof usr.searchMsg.search_time);


    msg_len = send(connect_sock, (char*)&usr, sizeof (szBuff), 0);

    if (msg_len == SOCKET_ERROR) {
       fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
       WSACleanup();
    }

    if (msg_len == 0) {
       printf("server closed connection\n");
       closesocket(connect_sock);
       WSACleanup();
    }

}
