#ifndef SEARCHHISTORY_H
#define SEARCHHISTORY_H

#include <QWidget>

namespace Ui {
class SearchHistory;
}

class SearchHistory : public QWidget
{
    Q_OBJECT

public:
    explicit SearchHistory(QWidget *parent = nullptr);
    ~SearchHistory();

private slots:
    void on_Exit_clicked();

    void on_Search_clicked();

    void recv_From_Main_Dialog(QString data);

    void change_private_his_name(QString data);

private:
    Ui::SearchHistory *ui;
    QString curSelectUsrName;
};

#endif // SEARCHHISTORY_H
