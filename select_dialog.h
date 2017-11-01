#ifndef SELECT_DIALOG_H
#define SELECT_DIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class select_Dialog;
}

class select_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit select_Dialog(QWidget *parent = 0);
    ~select_Dialog();
signals:
    void unitList_send(QList<QListWidgetItem*>);

private slots:
    void info_rec(QList<QListWidgetItem*>,QList<int>,QList<QListWidgetItem*>);
    void handleCopyButton();
    void back_rev(int);
    void addValuePairs();
    void delValuePairs();
    void handleSaveButton();
    void handleCancelButton();
    void handleConfirmButton();

private:
    Ui::select_Dialog *ui;
};

#endif // SELECT_DIALOG_H
