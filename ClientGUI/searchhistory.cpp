#include "searchhistory.h"
#include "ui_searchhistory.h"
#include "client.h"
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

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
    ui->historyBrowser->clear();
}

char* SearchHistory::str_handle(char* raw)
{
    char* result = (char*)malloc(2*strlen(raw)*sizeof(char));
    int i = 0, j = 0;
    for(; i < (int)strlen(raw); i++, j++){
        if(raw[i] == '\'' || raw[i] == '"' || raw[i] == '\\'){
            result[j] = '\\';
            j++;
        }
        result[j] = raw[i];
    }
    result[j] = '\0';
    return result;
}

void SearchHistory::on_Search_clicked()
{
    QString name_plain_text = curSelectUsrName;
    QString content_plain_text = ui->Content->text();
    QString time_plain_text = ui->Time->text();

//    qDebug() << name_plain_text << content_plain_text << time_plain_text;

    char *search_name, *search_content,*search_time;

    QByteArray nameByte = name_plain_text.toLocal8Bit();
    QByteArray contentByte = content_plain_text.toLocal8Bit();
    QByteArray timeByte = time_plain_text.toLocal8Bit();

    search_name = nameByte.data();
    search_content = contentByte.data();
    search_time = timeByte.data();

    if(strlen(search_name) == 0){
        QString dlgTitle="Warning!!!";
        QString strInfo="You have to chose a group or a person";
        QMessageBox::warning(this, dlgTitle, strInfo);
        return;
    }

    if(usr.room != -1)
        strcpy(usr.type, "SEARCHROOM");
    else
        strcpy(usr.type, "SEARCH");

    search_name = str_handle(search_name);
    search_content = str_handle(search_content);

    memcpy(usr.searchMsg.search_name, search_name, sizeof usr.searchMsg.search_name);
    memcpy(usr.searchMsg.search_content, search_content, sizeof usr.searchMsg.search_content);
    memcpy(usr.searchMsg.search_time, search_time, sizeof usr.searchMsg.search_time);
    //usr.room = -1;

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

    ui->historyBrowser->clear();

    return;
}

void SearchHistory::recv_From_Main_Dialog(QString data){
    ui->historyBrowser->append(data);
}

void SearchHistory::change_private_his_name(QString data){
    curSelectUsrName = data;
}
