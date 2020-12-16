#ifndef INVITEGROUP_H
#define INVITEGROUP_H

#include <QWidget>

namespace Ui {
class InviteGroup;
}

class InviteGroup : public QWidget
{
    Q_OBJECT

public:
    explicit InviteGroup(QWidget *parent = nullptr);
    ~InviteGroup();

private slots:
    void on_addButton_clicked();

private:
    Ui::InviteGroup *ui;
};

#endif // INVITEGROUP_H
