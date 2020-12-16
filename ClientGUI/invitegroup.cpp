#include "invitegroup.h"
#include "ui_invitegroup.h"
#include "client.h"
#include <QMessageBox>

InviteGroup::InviteGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InviteGroup)
{
    ui->setupUi(this);
}

InviteGroup::~InviteGroup()
{
    delete ui;
}

void InviteGroup::on_addButton_clicked()
{
    QString invite_name = ui->PName->text();
    QString room_name = ui->GName->text();
    QString request_name = name;

    QByteArray invite_name_byte = invite_name.toLocal8Bit();
    QByteArray room_name_byte = room_name.toLocal8Bit();
    QByteArray request_name_byte = request_name.toLocal8Bit();

    char* invite_name_char = invite_name_byte.data();
    char* room_name_char = room_name_byte.data();
    char* request_name_char = request_name_byte.data();

    if(strlen(invite_name_char) == 0 || strlen(room_name_char) == 0){
        QString dlgTitle="Warning!!!";
        QString strInfo="You have to enter the person name and the room name!";
        QMessageBox::warning(this, dlgTitle, strInfo);
        return;
    }

    memcpy(usr.name, request_name_char, sizeof usr.name);
    memcpy(usr.msg, room_name_char, sizeof usr.msg);
    memcpy(usr.invite_name, invite_name_char, sizeof usr.invite_name);

    strcpy(usr.type, "INVITE");

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

    return;
}
