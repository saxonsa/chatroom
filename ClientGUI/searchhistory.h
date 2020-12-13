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

private:
    Ui::SearchHistory *ui;
};

#endif // SEARCHHISTORY_H
