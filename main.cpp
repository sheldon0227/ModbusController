#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::MSWindowsFixedSizeDialogHint;
    MainWindow w;
    w.setWindowFlags(flags);
    w.setFixedSize(720,565);
    w.setWindowTitle("Modbus Sender");
    w.show();

    return a.exec();
}

