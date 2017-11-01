#include "select_dialog.h"
#include "ui_select_dialog.h"
#include "addr_value.h"
#include "m_client.h"
#include "copy_dialog.h"
#include <QDebug>
#include <QMessageBox>
using namespace std;

QList<QListWidgetItem*> UNITS;
QList<int> POS;
QList<QListWidgetItem*> UNITS_LIST;

select_Dialog::select_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_Dialog)
{
    ui->setupUi(this);
    //table widget setting
    ui->activeValue->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->activeValue->verticalHeader()->setVisible(false);
    ui->activeValue->horizontalHeader()->setVisible(false);
    ui->activeValue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //input restriction
    ui->registerAddressLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->valueLineEdit->setValidator(new QIntValidator(0,65535,this));

    //connect
    connect(ui->addButton,SIGNAL(released()),this,SLOT(addValuePairs()));
    connect(ui->delButton,SIGNAL(released()),this,SLOT(delValuePairs()));
    connect(ui->saveButton,SIGNAL(released()),this,SLOT(handleSaveButton()));
    connect(ui->cancelButton,SIGNAL(released()),this,SLOT(handleCancelButton()));
    connect(ui->confirmButton,SIGNAL(released()),this,SLOT(handleConfirmButton()));
    connect(ui->copyButton,SIGNAL(released()),this,SLOT(handleCopyButton()));
}

select_Dialog::~select_Dialog()
{
    delete ui;
}

void select_Dialog::info_rec(QList<QListWidgetItem*> units,QList<int> pos,QList<QListWidgetItem*> unit_list)
{
    UNITS = units;
    POS = pos;
    UNITS_LIST = unit_list;
    ui->activeValue->setColumnCount(2);
    ui->activeValue->setColumnWidth(0,280);
    ui->activeValue->setColumnWidth(1,280);
}

void select_Dialog::addValuePairs()
{
    if(ui->registerAddressLineEdit->text()!="")
    {
    QMessageBox msgBox;
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

void select_Dialog::delValuePairs()
{
    ui->activeValue->removeRow(ui->activeValue->currentRow());
}

void select_Dialog::handleSaveButton()
{
    QMessageBox msgBox;
    for(int i=0;i<ui->activeValue->rowCount();i++)
    {
        for(int j=0;j<POS.size();j++)
        {
            bool flag = false;
            vector<addr_value>::iterator innerIter;
            for(innerIter=ARRAY.at(POS[j]).begin();innerIter!=ARRAY.at(POS[j]).end();innerIter++)
            {
                if(innerIter->addr == ui->activeValue->item(i,0)->text().toInt())
                {
                    innerIter->value = ui->activeValue->item(i,1)->text().toInt();
                    flag = true;
                }
            }
            if(flag == false)
            {
                addr_value T = {ui->activeValue->item(i,0)->text().toInt(),ui->activeValue->item(i,1)->text().toInt()};
                ARRAY.at(POS.at(j)).push_back(T);
            }
        }
    }
    msgBox.information(this,"Saving Completed!","All values are saved.");
    //done(1);
}

void select_Dialog::handleConfirmButton()
{
    QMessageBox msgBox;
    bool flag = true;
    for(int index=0;index<UNITS.size();index++)
    {
        int i = 0;
        QString ip = "";
        QString port = "";
        QString unit_id = "";
        while(UNITS.at(index)->text()[i]!=':')
        {
            ip.append(UNITS.at(index)->text()[i]);
            i++;
        }
        i++;
        while(UNITS.at(index)->text()[i]!='[')
        {
            port.append(UNITS.at(index)->text()[i]);
            i++;
        }
        i++;
        while(UNITS.at(index)->text()[i]!=']')
        {
            unit_id.append(UNITS.at(index)->text()[i]);
            i++;
        }
        for(int j=0;j<ui->activeValue->rowCount();j++)
        {
            M_Client A(ip.toLatin1(),port.toInt(),unit_id.toInt());
            if(!A.Connect()){
                A.Modbus_sender_single(i, ui->activeValue->item(j,0)->text().toInt(), ui->activeValue->item(j,1)->text().toInt());
                A.Close();
            }
            else{
                msgBox.information(this,"Send Value Error!","Can not send ["+ui->activeValue->item(j,1)->text()+","+ui->activeValue->item(j,0)->text()+"] to "+ip+" port:"+port+" unit:"+unit_id+"\n"+"Please check connection!");
                A.Close();
                flag = false;
                break;
            }
        }
        if(flag==false)
            break;
        if(index+1==UNITS.size())
            msgBox.information(this,"Sending Completed!","All values are sended.");
    }
    if(flag)
        handleSaveButton();
    else
        msgBox.information(this,"Notice!","Values are not saved due to connect problem\nIf you still want to save, click Save.");
}

void select_Dialog::handleCancelButton()
{
    done(0);
}





void select_Dialog::handleCopyButton()
{
    copy_Dialog copyDialog(this);
    copyDialog.setWindowTitle("Select the unit you wanna copy");
    connect(this,SIGNAL(unitList_send(QList<QListWidgetItem*>)),&copyDialog,SLOT(unitList_rev(QList<QListWidgetItem*>)));
    emit unitList_send(UNITS_LIST);
    copyDialog.exec();
}

void select_Dialog::back_rev(int copyTarget)
{
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



