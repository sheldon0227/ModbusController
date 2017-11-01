#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QDebug>

#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void info_sendTosel(QList<QListWidgetItem *>,QList<int>,QList<QListWidgetItem*>);
    void info_sender(QString,QString,QString,int,QList<QListWidgetItem*>);
private slots:
    void handleConnectButton();
    void enterUnit(QListWidgetItem* Item);
    void importConfig();
    void exportConfig();
    void sendAll();
    void resetAll();
    void checkConnection();
    void handleSetSelection();
    void handleSendSelect();
    void keyPressEvent(QKeyEvent *event);
    void delItem();
    void valueRec(QString v);
    void setBlackColor();
private:
    void setColor(int,QString,QString,Qt::GlobalColor);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
