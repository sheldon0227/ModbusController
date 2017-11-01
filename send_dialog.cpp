#include "send_dialog.h"
#include "ui_send_dialog.h"
#include "m_client.h"
#include "copy_dialog.h"
#include <qDebug.h>
#include <QTableWidget>
#include "addr_value.h"
#include <QMessageBox>
#include <QProgressDialog>
using namespace std;
QString IP;
QString PORT;
QString UNIT_ID;
QList<QListWidgetItem*> UNIT_LIST;
int currRow;
send_Dialog::send_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_Dialog)
{
    ui->setupUi(this);
    ui->progressBar->setValue(100);
    //set widget attribute
    ui->activeValue->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->activeValue->verticalHeader()->setVisible(false);
    ui->activeValue->horizontalHeader()->setVisible(false);
    ui->activeValue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set input restrition
    ui->registerAddressLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->valueLineEdit->setValidator(new QIntValidator(0,65535,this));

    //connect
    connect(ui->confirmButton,SIGNAL(released()),this,SLOT(writeOneRegister()));
    connect(ui->cancelButton,SIGNAL(released()),this,SLOT(handleCancelButton()));
    connect(ui->addButton,SIGNAL(released()),this,SLOT(addValuePairs()));
    connect(ui->delButton,SIGNAL(released()),this,SLOT(delValuePairs()));
    connect(ui->saveButton,SIGNAL(released()),this,SLOT(handleSaveButton()));
    connect(ui->resetValueButton,SIGNAL(released()),this,SLOT(handleResetButton()));
    connect(ui->copyButton,SIGNAL(released()),this,SLOT(handleCopyButton()));
    //connect(ui->pushButton,SIGNAL(released()),this,SLOT(test()));
}

send_Dialog::~send_Dialog()
{
    delete ui;
}

void send_Dialog::info_rec(QString ip,QString port,QString unit_id, int CurrRow,QList<QListWidgetItem*> unit_list)
{
    UNIT_LIST = unit_list;
    IP = ip;
    PORT = port;
    UNIT_ID = unit_id;
    currRow = CurrRow;
    vector<addr_value> Temp;
    ui->activeValue->setRowCount(ARRAY.at(currRow).size());
    ui->activeValue->setColumnCount(2);
    ui->activeValue->setColumnWidth(0,280);
    ui->activeValue->setColumnWidth(1,280);
    Temp.assign(ARRAY.at(currRow).begin(),ARRAY.at(currRow).end());
    vector<addr_value>::iterator Iter;
    int m = 0;
    for(Iter=Temp.begin();Iter!=Temp.end();Iter++)
    {
        //QTableWidgetItem* item = new QTableWidgetItem(QString::number(Iter->addr));
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,Iter->addr);
        ui->activeValue->setItem(m,0,item);
        ui->activeValue->item(m,0)->setFlags(ui->activeValue->item(m,0)->flags() & (~Qt::ItemIsEditable));
        ui->activeValue->setItem(m,1,new QTableWidgetItem(QString::number(Iter->value)));
        m++;
    }
    ui->activeValue->sortByColumn(0,Qt::AscendingOrder);
}

void send_Dialog::writeOneRegister()
{
    QMessageBox msgBox;
    handleSaveButton();
    ui->progressBar->setRange(0,ui->activeValue->rowCount());
    for(int i=0;i<ui->activeValue->rowCount();i++)
    {
        M_Client A(IP.toLatin1(),PORT.toInt(),UNIT_ID.toInt());
        if(!A.Connect()){
            A.Modbus_sender_single(i, ui->activeValue->item(i,0)->text().toInt(), ui->activeValue->item(i,1)->text().toInt());
            A.Close();
        }
        else{
            msgBox.information(this,"Send Value Error!","Can not send value "+ui->activeValue->item(i,1)->text()+" to register#"+ui->activeValue->item(i,0)->text()+"\n"+"Please check connection!");
            A.Close();
            break;
        }
        ui->progressBar->setValue(i+1);
        if(i+1==ui->activeValue->rowCount())
            msgBox.information(this,"Sending Completed!","All values are sended.");
    }
    //done(1);
}

void send_Dialog::handleCancelButton()
{
    done(0);
}

void send_Dialog::addValuePairs()
{
    if(ui->registerAddressLineEdit->text()!="")
    {
    QMessageBox msgBox;
    ui->progressBar->setValue(0);
    bool unique = true;
    for(int i=0;i<ui->activeValue->rowCount();i++)
    {
        if(ui->registerAddressLineEdit->text()==ui->activeValue->item(i,0)->text())
        {
            unique = false;
        }
    }
    if(unique == true)
    {
        ui->activeValue->setRowCount(ui->activeValue->rowCount()+1);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole, ui->registerAddressLineEdit->text().toInt());
        ui->activeValue->setItem(ui->activeValue->rowCount()-1,0,item);
        ui->activeValue->item(ui->activeValue->rowCount()-1,0)->setFlags(ui->activeValue->item(ui->activeValue->rowCount()-1,0)->flags() & (~Qt::ItemIsEditable));
        if(ui->valueLineEdit->text()!="")
            ui->activeValue->setItem(ui->activeValue->rowCount()-1,1,new QTableWidgetItem(ui->valueLineEdit->text()));
        else
            ui->activeValue->setItem(ui->activeValue->rowCount()-1,1,new QTableWidgetItem("0"));

        ui->activeValue->sortByColumn(0,Qt::AscendingOrder);
    }
    else
    {
        msgBox.information(this,"Add Register Error!","This Register is added to list!");
    }
    ui->registerAddressLineEdit->clear();
    ui->valueLineEdit->clear();
    }
}

void send_Dialog::delValuePairs()
{
    ui->progressBar->setValue(0);
    ui->activeValue->removeRow(ui->activeValue->currentRow());
}

void send_Dialog::handleSaveButton()
{
    vector<addr_value> Temp;
    ui->progressBar->setRange(0,ui->activeValue->rowCount());
    for(int i=0;i<ui->activeValue->rowCount();i++)
    {
        addr_value T = {ui->activeValue->item(i,0)->text().toInt(),ui->activeValue->item(i,1)->text().toInt()};
        Temp.push_back(T);
        ui->progressBar->setValue(i+1);
    }
    ARRAY.at(currRow).assign(Temp.begin(),Temp.end());
    //done(1);
}

void send_Dialog::handleResetButton()
{
    ui->progressBar->setValue(0);
    if(!ui->valueLineEdit->text().isEmpty())
    {
        for(int i=0;i<ui->activeValue->rowCount();i++)
        {
            ui->activeValue->setItem(i,1,new QTableWidgetItem(ui->valueLineEdit->text()));
        }
        ui->valueLineEdit->clear();
    }
}

void send_Dialog::handleCopyButton()
{
    copy_Dialog copyDialog(this);
    copyDialog.setWindowTitle("Select the unit you wanna copy");
    connect(this,SIGNAL(unitList_send(QList<QListWidgetItem*>)),&copyDialog,SLOT(unitList_rev(QList<QListWidgetItem*>)));
    emit unitList_send(UNIT_LIST);
    copyDialog.exec();
}

void send_Dialog::back_rev(int copyTarget)
{
    ui->progressBar->setValue(0);
    vector<addr_value> Temp;
    Temp.assign(ARRAY.at(copyTarget).begin(),ARRAY.at(copyTarget).end());
    ui->activeValue->setRowCount(ARRAY.at(copyTarget).size());
    vector<addr_value>::iterator Iter;
    int m = 0;
    for(Iter=Temp.begin();Iter!=Temp.end();Iter++)
    {
        //QTableWidgetItem* item = new QTableWidgetItem(QString::number(Iter->addr));
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,Iter->addr);
        ui->activeValue->setItem(m,0,item);
        ui->activeValue->item(m,0)->setFlags(ui->activeValue->item(m,0)->flags() & (~Qt::ItemIsEditable));
        ui->activeValue->setItem(m,1,new QTableWidgetItem(QString::number(Iter->value)));
        m++;
    }
    ui->activeValue->sortByColumn(0,Qt::AscendingOrder);
}


void send_Dialog::test()
{
    QProgressDialog progress_dialog("Sending values to Units...","Cancel",0,10000,this);
    progress_dialog.setWindowModality(Qt::WindowModal);
    progress_dialog.setMinimumDuration(0);
    progress_dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    progress_dialog.setFixedSize(500,120);
    for(int i=0; i<10000; i++)
    {
        ui->registerAddressLineEdit->setText(QString::number(i,10));
        ui->valueLineEdit->setText("65535");
        addValuePairs();
        progress_dialog.setValue(i);
    }
}
