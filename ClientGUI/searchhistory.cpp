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


}
