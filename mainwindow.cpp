#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reset_dialog.h"
#include "send_dialog.h"
#include "m_client.h"
#include "addr_value.h"
#include "select_dialog.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QKeyEvent>
#include <QMessageBox>


using namespace std;
//#pragma comment(lib,"ws2_32.lib")
QString VALUE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //输入限制
    QIntValidator* ipV = new QIntValidator(0,255,this);
    ui->iPAddressLineEdit_1->setValidator(ipV);
    ui->iPAddressLineEdit_2->setValidator(ipV);
    ui->iPAddressLineEdit_3->setValidator(ipV);
    ui->iPAddressLineEdit_4->setValidator(ipV);
    ui->portLineEdit->setValidator(new QIntValidator(0,99999,this));
    ui->unitIDLineEdit->setValidator(new QIntValidator(0,99999, this));
    //默认输入
    ui->checkConnectionBox->setChecked(true);
    ui->portLineEdit->setText("502");

    //表格多选
    ui->activeUnitWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //连接信号
    connect(ui->handleConnectButton,SIGNAL(released()),this,SLOT(handleConnectButton()));
    connect(ui->activeUnitWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(enterUnit(QListWidgetItem*)));
    //    connect(ui->importButton,SIGNAL(released()),this,SLOT(importConfig()));
    //    connect(ui->exportButton,SIGNAL(released()),this,SLOT(exportConfig()));
    connect(ui->delItemButton,SIGNAL(released()),this,SLOT(delItem()));
    //  connect(ui->sendAllButton,SIGNAL(released()),this,SLOT(sendAll()));
    // connect(ui->resetValueButton,SIGNAL(released()),this,SLOT(resetAll()));
    //connect(ui->setSelectedButton,SIGNAL(released()),this,SLOT(handleSetSelection()));
    // connect(ui->sendSelectedButton,SIGNAL(released()),this,SLOT(handleSendSelect()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(exportConfig()));
    connect(ui->actionImport,SIGNAL(triggered(bool)),this,SLOT(importConfig()));
    connect(ui->actionSet_All_Register,SIGNAL(triggered(bool)),this,SLOT(resetAll()));
    connect(ui->action_Send_All,SIGNAL(triggered(bool)),this,SLOT(sendAll()));
    connect(ui->actionSet_Seleted_Register,SIGNAL(triggered(bool)),this,SLOT(handleSetSelection()));
    connect(ui->actionSend_Seleted_Units,SIGNAL(triggered(bool)),this,SLOT(handleSendSelect()));
    connect(ui->actionCheck_Connections_2,SIGNAL(triggered(bool)),this,SLOT(setBlackColor()));
    connect(ui->actionCheck_Connections_2,SIGNAL(triggered(bool)),this,SLOT(checkConnection()));
    //状态栏版本号
    ui->statusBar->setSizeGripEnabled(false);
    QLabel *version = new QLabel(this);
    version->setText("Version 1.1");
    version->setAlignment(Qt::AlignLeft);
    ui->statusBar->addWidget(version);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(event->key()==Qt::Key_Enter||event->key()==Qt::Key_Return)
    {
        handleConnectButton();
    }

}

void MainWindow::handleConnectButton()
{
    QMessageBox msgBox;
    if(ui->iPAddressLineEdit_1->text().isEmpty()!=0)
    {
        ui->iPAddressLineEdit_1->setText("0");
    }
    if(ui->iPAddressLineEdit_2->text().isEmpty()!=0)
    {
        ui->iPAddressLineEdit_2->setText("0");
    }
    if(ui->iPAddressLineEdit_3->text().isEmpty()!=0)
    {
        ui->iPAddressLineEdit_3->setText("0");
    }
    if(ui->iPAddressLineEdit_4->text().isEmpty()!=0)
    {
        ui->iPAddressLineEdit_4->setText("0");
    }
    if(ui->unitIDLineEdit->text().isEmpty()!=0)
    {
        ui->unitIDLineEdit->setText("0");
    }
    if(ui->portLineEdit->text().isEmpty()!=0)
    {
        msgBox.information(this,"Port is empty","Please enter Port#");
    }
    else
    {
        QString ipAddress;
        ipAddress = ui->iPAddressLineEdit_1->text()+"."+ui->iPAddressLineEdit_2->text()+"."+ui->iPAddressLineEdit_3->text()+"."+ui->iPAddressLineEdit_4->text();
        M_Client A(ipAddress.toLatin1(),ui->portLineEdit->text().toInt(),ui->unitIDLineEdit->text().toInt());
        if(ui->activeUnitWidget->findItems(ipAddress+":"+ui->portLineEdit->text()+"["+ui->unitIDLineEdit->text()+"]",0).isEmpty()==1)
        {
            if(ui->checkConnectionBox->isChecked())
            {
                if(A.Connect()==0)
                {
                    ui->activeUnitWidget->addItem(ipAddress+":"+ui->portLineEdit->text()+"["+ui->unitIDLineEdit->text()+"]");
                    vector<addr_value> temp;
                    ARRAY.push_back(temp);
                    A.Close();
                }
                else
                {
                    msgBox.information(this,"Connection Error!","Cannot connect entered unit!");
                }
            }
            else
            {
                ui->activeUnitWidget->addItem(ipAddress+":"+ui->portLineEdit->text()+"["+ui->unitIDLineEdit->text()+"]");
                vector<addr_value> temp;
                ARRAY.push_back(temp);
            }
        }
        else
        {
            msgBox.information(this,"Connection Error!","This unit is added!");
        }
    }
}

void MainWindow::enterUnit(QListWidgetItem* Item)
{
    QString content = Item->text();
    int currRow = ui->activeUnitWidget->row(Item);

    int i = 0;
    QString ip = "";
    QString port = "";
    QString unit_id = "";
    while(content[i]!=':')
    {
        ip.append(content[i]);
        i++;
    }
    i++;
    while(content[i]!='[')
    {
        port.append(content[i]);
        i++;
    }
    i++;
    while(content[i]!=']')
    {
        unit_id.append(content[i]);
        i++;
    }
    QList<QListWidgetItem*> Temp_all;
    for(int i=0;i<ui->activeUnitWidget->count();i++)
    {
        Temp_all.push_back(ui->activeUnitWidget->item(i));
    }
    //send_Dialog* singleUnitDialog  = new send_Dialog(this);
    //connect(this,SIGNAL(info_sender(QString,QString,QString)),singleUnitDialog,SLOT(info_rec(QString,QString,QString)));
    send_Dialog singleUnitDialog;
    singleUnitDialog.setWindowTitle(" ====== IP: "+ip+"    "+"Port: "+port+"    "+"Unit: "+unit_id+" ======");
    connect(this,SIGNAL(info_sender(QString,QString,QString,int,QList<QListWidgetItem*>)),&singleUnitDialog,SLOT(info_rec(QString,QString,QString,int,QList<QListWidgetItem*>)));
    emit info_sender(ip,port,unit_id,currRow,Temp_all);
    singleUnitDialog.exec();
}

void MainWindow::importConfig()
{
    QString iniFileName = QFileDialog::getOpenFileName(this,tr("Open config"),"/",tr("config file(*.ini)"));
    ui->handleConnectButton->hide();
    QSettings *read = new QSettings (iniFileName,QSettings::IniFormat);
    int count = read->value("/stat/amount").toInt();
    ui->activeUnitWidget->clear();
    ARRAY.clear();
    for(int i=0;i<count;i++)
    {
        ui->activeUnitWidget->addItem(read->value("/des/unit"+QString::number(i,10)).toString());
        vector<addr_value> inputBuf;
        for(int j=0;j<read->value("/des/unit"+QString::number(i,10)+"/amount").toInt();j++)
        {
            addr_value pair_buf = {read->value("/des/unit"+QString::number(i,10)+"/addr"+QString::number(j,10)).toInt(),
                                   read->value("/des/unit"+QString::number(i,10)+"/value"+QString::number(j,10)).toInt()};
            inputBuf.push_back(pair_buf);
        }
        ARRAY.push_back(inputBuf);
    }
    delete read;
    ui->handleConnectButton->show();
}

void MainWindow::exportConfig()
{
    QString iniFileName = QFileDialog::getSaveFileName(this,tr("Open config"),"/",tr("config file(*.ini)"));
    QFile del;
    del.remove(iniFileName);
    ui->handleConnectButton->hide();
    QSettings *write = new QSettings (iniFileName,QSettings::IniFormat);
    write->setValue("/stat/amount",ui->activeUnitWidget->count());
    for(int i=0;i<ui->activeUnitWidget->count();i++)
    {
        write->setValue("/des/unit"+QString::number(i,10),ui->activeUnitWidget->item(i)->text());
        vector<addr_value>::iterator innerIter;
        int index = 0;
        for(innerIter = ARRAY.at(i).begin();innerIter!=ARRAY.at(i).end();innerIter++)
        {
            write->setValue("/des/unit"+QString::number(i,10)+"/addr"+QString::number(index,10),innerIter->addr);
            write->setValue("/des/unit"+QString::number(i,10)+"/value"+QString::number(index,10),innerIter->value);
            index++;
        }
        write->setValue("/des/unit"+QString::number(i,10)+"/amount",index);
    }
    delete write;
    ui->handleConnectButton->show();
}

void MainWindow::delItem()
{
    if(ui->activeUnitWidget->currentItem()!=Q_NULLPTR)
    {
        ARRAY.erase(ARRAY.begin()+ui->activeUnitWidget->currentRow());
        QListWidgetItem *item = ui->activeUnitWidget->takeItem(ui->activeUnitWidget->currentRow());
        delete item;
        ui->activeUnitWidget->setCurrentItem(NULL);
    }
}

void MainWindow::sendAll()
{
    QMessageBox msgBox;

    int count = 0;
    for(unsigned int i=0;i<ARRAY.size();i++)
    {
        count+=ARRAY.at(i).size();
    }
    int num = 0;
    QProgressDialog progress_dialog("Sending values to Units...","Cancel",0,count,this);
    progress_dialog.setWindowModality(Qt::WindowModal);
    progress_dialog.setMinimumDuration(0);
    progress_dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    progress_dialog.setFixedSize(500,120);
    progress_dialog.setWindowTitle("Sending values to Units...");
    //progress_dialog.setAutoClose(false);
    for(unsigned int m = 0; m<ARRAY.size();m++)
    {
        int i = 0;
        QString ip = "";
        QString port = "";
        QString unit_id = "";
        while(ui->activeUnitWidget->item(m)->text()[i]!=':')
        {
            ip.append(ui->activeUnitWidget->item(m)->text()[i]);
            i++;
        }
        i++;
        while(ui->activeUnitWidget->item(m)->text()[i]!='[')
        {
            port.append(ui->activeUnitWidget->item(m)->text()[i]);
            i++;
        }
        i++;
        while(ui->activeUnitWidget->item(m)->text()[i]!=']')
        {
            unit_id.append(ui->activeUnitWidget->item(m)->text()[i]);
            i++;
        }
        vector<addr_value>::iterator innerIter;
        for(innerIter=ARRAY.at(m).begin(); innerIter!=ARRAY.at(m).end() ; innerIter++)
        {
            progress_dialog.setLabelText("Sending values to.."+ui->activeUnitWidget->item(m)->text());
            M_Client A(ip.toLatin1(),port.toInt(),unit_id.toInt());
            if(!A.Connect())
            {
            A.Modbus_sender_single(num, innerIter->addr , innerIter->value);
            A.Close();
            progress_dialog.setValue(++num);
            }
            else
            {

                int rlt = msgBox.information(this,"Sending Error!","Cannot Connect to"+ui->activeUnitWidget->item(m)->text(),QMessageBox::Ignore|QMessageBox::Cancel);
                if(rlt == QMessageBox::Cancel)
                {
                    return;
                }
                A.Close();
                break;
            }
        }
    }
}

void MainWindow::resetAll()
{
    reset_Dialog Dialog;
    Dialog.setWindowTitle("Reset all registers in list");
    connect(&Dialog,SIGNAL(valueSend(QString)),this,SLOT(valueRec(QString)));
    Dialog.exec();
    for(unsigned int i = 0;i < ARRAY.size(); i++)
    {
        vector<addr_value>::iterator innerIter;
        for(innerIter=ARRAY.at(i).begin();innerIter!=ARRAY.at(i).end();innerIter++)
        {
            innerIter->value = VALUE.toInt();
        }
    }
}

void MainWindow::handleSetSelection()
{
    if(ui->activeUnitWidget->selectedItems().isEmpty()==0)
    {
        QList<int> Temp;
        QList<QListWidgetItem*> Temp_Item = ui->activeUnitWidget->selectedItems();

        for(int i=0;i<Temp_Item.size();i++)
        {
            Temp.push_back(ui->activeUnitWidget->row(Temp_Item.at(i)));
        }
        QList<QListWidgetItem*> Temp_all;
        for(int i=0;i<ui->activeUnitWidget->count();i++)
        {
            Temp_all.push_back(ui->activeUnitWidget->item(i));
        }
        select_Dialog multiUnitDialog;
        multiUnitDialog.setWindowTitle("Selected Units");
        connect(this,SIGNAL(info_sendTosel(QList<QListWidgetItem*>,QList<int>,QList<QListWidgetItem*>)),&multiUnitDialog,SLOT(info_rec(QList<QListWidgetItem*>,QList<int>,QList<QListWidgetItem*>)));
        emit info_sendTosel(Temp_Item,Temp,Temp_all);
        multiUnitDialog.exec();
    }
}

void MainWindow::handleSendSelect()
{
    if(ui->activeUnitWidget->selectedItems().isEmpty()==0)
    {
        QList<int> Temp;
        QList<QListWidgetItem*> Temp_Item = ui->activeUnitWidget->selectedItems();
        for(int i=0;i<Temp_Item.size();i++)
        {
            Temp.push_back(ui->activeUnitWidget->row(Temp_Item.at(i)));
        }

        int count = 0;
        for(int i=0;i<Temp.size();i++)
        {
            count+=ARRAY.at(Temp.at(i)).size();
        }
        int num = 0;
        QProgressDialog progress_dialog("Sending values to Units...","Cancel",0,count,this);
        progress_dialog.setWindowModality(Qt::WindowModal);
        progress_dialog.setMinimumDuration(0);
        progress_dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        progress_dialog.setFixedSize(500,120);
        progress_dialog.setWindowTitle("Sending values to Units...");


        for(int index=0;index<Temp.size();index++)
        {
            int i = 0;
            QString ip = "";
            QString port = "";
            QString unit_id = "";
            while(Temp_Item.at(index)->text()[i]!=':')
            {
                ip.append(Temp_Item.at(index)->text()[i]);
                i++;
            }
            i++;
            while(Temp_Item.at(index)->text()[i]!='[')
            {
                port.append(Temp_Item.at(index)->text()[i]);
                i++;
            }
            i++;
            while(Temp_Item.at(index)->text()[i]!=']')
            {
                unit_id.append(Temp_Item.at(index)->text()[i]);
                i++;
            }
            vector<addr_value>::iterator innerIter;
            for(innerIter=ARRAY.at(Temp.at(index)).begin();innerIter!=ARRAY.at(Temp.at(index)).end();innerIter++)
            {
                progress_dialog.setLabelText("Sending values to.."+Temp_Item.at(index)->text());
                M_Client A(ip.toLatin1(),port.toInt(),unit_id.toInt());
                A.Connect();
                A.Modbus_sender_single(num, innerIter->addr , innerIter->value);
                A.Close();
                progress_dialog.setValue(++num);
            }
        }
    }
}

void MainWindow::valueRec(QString v)
{
    VALUE = v;
}

void MainWindow::checkConnection()
{

    ui->activeUnitWidget->repaint();
    if(ui->activeUnitWidget->count()!=0)
    {
        int rowCount = 0;
        while(rowCount!=ui->activeUnitWidget->count())
        {
            if(ui->activeUnitWidget->item(rowCount)->textColor()==Qt::blue||ui->activeUnitWidget->item(rowCount)->textColor()==Qt::red)
            {
                rowCount++;
                continue;
            }
            else
            {
                int i = 0;
                QString ip = "";
                QString port = "";
                while(ui->activeUnitWidget->item(rowCount)->text()[i]!=':')
                {
                    ip.append(ui->activeUnitWidget->item(rowCount)->text()[i]);
                    i++;
                }
                i++;
                while(ui->activeUnitWidget->item(rowCount)->text()[i]!='[')
                {
                    port.append(ui->activeUnitWidget->item(rowCount)->text()[i]);
                    i++;
                }
                M_Client check(ip.toLatin1(),port.toInt(),rowCount);
                int rlt = check.Connect();
                check.Close();
                check.~M_Client();

                if(!rlt)
                {
                    setColor(rowCount,ip,port,Qt::darkGreen);
                    ui->activeUnitWidget->repaint();
                    rowCount++;
                }
                else{
                    setColor(rowCount,ip,port,Qt::red);
                    ui->activeUnitWidget->repaint();
                    rowCount++;
                }
            }
        }
    }
}

void MainWindow::setColor(int start,QString ip_ref,QString port_ref,Qt::GlobalColor color)
{
    for(int m=start;m<ui->activeUnitWidget->count();m++)
    {
        int i = 0;
        QString ip = "";
        QString port = "";
        while(ui->activeUnitWidget->item(m)->text()[i]!=':')
        {
            ip.append(ui->activeUnitWidget->item(m)->text()[i]);
            i++;
        }
        i++;
        while(ui->activeUnitWidget->item(m)->text()[i]!='[')
        {
            port.append(ui->activeUnitWidget->item(m)->text()[i]);
            i++;
        }
        if(ip==ip_ref&&port==port_ref)
        {
            ui->activeUnitWidget->item(m)->setTextColor(color);
            ui->activeUnitWidget->repaint();
            Sleep(100);
        }
    }
}

void MainWindow::setBlackColor()
{
    for(int i=0;i<ui->activeUnitWidget->count();i++)
    {
        ui->activeUnitWidget->item(i)->setTextColor(Qt::black);
        ui->activeUnitWidget->repaint();
    }
    Sleep(300);
}
