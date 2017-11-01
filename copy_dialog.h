#ifndef COPY_DIALOG_H
#define COPY_DIALOG_H

#include <QDialog>
#include <QListWidget>
namespace Ui {
class copy_Dialog;
}

class copy_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit copy_Dialog(QWidget *parent = 0);
    ~copy_Dialog();
private slots:
    void unitList_rev(QList<QListWidgetItem*>);
    void handleCopyButton();
    void handleCancelButton();
signals:
    void back_send(int);
private:
    Ui::copy_Dialog *ui;
};

#endif // COPY_DIALOG_H
