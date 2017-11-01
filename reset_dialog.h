#ifndef RESET_DIALOG_H
#define RESET_DIALOG_H

#include <QDialog>

namespace Ui {
class reset_Dialog;
}

class reset_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit reset_Dialog(QWidget *parent = 0);
    ~reset_Dialog();
signals:
    void valueSend(QString);
private slots:
    void handleCancelButton();
    void handleConfirmButton();
private:
    Ui::reset_Dialog *ui;

};

#endif // RESET_DIALOG_H
