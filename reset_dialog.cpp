#include "reset_dialog.h"
#include "ui_reset_dialog.h"

reset_Dialog::reset_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reset_Dialog)
{
    ui->setupUi(this);
    ui->valueOfAllLineEdit->setValidator(new QIntValidator(0,65535,this));
    connect(ui->cancelButton,SIGNAL(released()),this,SLOT(handleCancelButton()));
    connect(ui->confirmButton,SIGNAL(released()),this,SLOT(handleConfirmButton()));
}

reset_Dialog::~reset_Dialog()
{
    delete ui;
}

void reset_Dialog::handleCancelButton()
{
    done(0);
}

void reset_Dialog::handleConfirmButton()
{
    emit valueSend(ui->valueOfAllLineEdit->text());
    done(1);
}
