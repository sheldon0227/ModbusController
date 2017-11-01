#include "copy_dialog.h"
#include "ui_copy_dialog.h"
#include <QDebug>

copy_Dialog::copy_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copy_Dialog)
{
    ui->setupUi(this);
    connect(ui->copyButton,SIGNAL(released()),this,SLOT(handleCopyButton()));
    connect(ui->cancelButton,SIGNAL(released()),this,SLOT(handleCancelButton()));
    connect(ui->copyUnitsListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(handleCopyButton()));
}

copy_Dialog::~copy_Dialog()
{
    delete ui;
}

void copy_Dialog::unitList_rev(QList<QListWidgetItem*> units_list)
{
    for(int i = 0; i<units_list.size() ;i++)
    {
        ui->copyUnitsListWidget->addItem(units_list.at(i)->text());
    }
}

void copy_Dialog::handleCopyButton()
{
    if(ui->copyUnitsListWidget->selectedItems().isEmpty()==0)
    {
        connect(this,SIGNAL(back_send(int)),parent(),SLOT(back_rev(int)));
        emit back_send(ui->copyUnitsListWidget->currentRow());
    }
    done(1);
}

void copy_Dialog::handleCancelButton()
{
    done(0);
}
