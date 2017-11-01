#ifndef SEND_DIALOG_H
#define SEND_DIALOG_H

#include <QDialog>
#include <QListWidget>
namespace Ui {
class send_Dialog;
}

class send_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit send_Dialog(QWidget *parent = 0);
    ~send_Dialog();
signals:
    void unitList_send(QList<QListWidgetItem*>);

private slots:
    void info_rec(QString,QString,QString,int,QList<QListWidgetItem*>);
    void back_rev(int);
    void writeOneRegister();
    void handleCancelButton();
    void addValuePairs();
    void delValuePairs();
    void handleSaveButton();
    void handleResetButton();
    void handleCopyButton();
    void test();
private:
    Ui::send_Dialog *ui;
};

#endif // SEND_DIALOG_H
