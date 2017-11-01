#include "m_client.h"
#include <errno.h>
#include <QDebug>
M_Client::M_Client(const char* Addr, int Port, int Id)
{
    address = Addr;
    port = Port;
    id = Id;
}
int M_Client::Connect()
{
    int rlt = 0;

    //用于记录错误信息并输出
    int iErrMsg;
    //启动WinSock
    WSAData wsaData;
    iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (iErrMsg != NO_ERROR)
        //有错误
    {
        //qDebug()<<"failed with wsaStartup error :"<<iErrMsg;

        rlt = 1;
        return rlt;
    }

    //创建Socket
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET)
        //创建Socket失败
    {
        //qDebug()<<"socket failed with error :"<<WSAGetLastError();

        rlt = 2;
        return rlt;
    }

    //目标服务器数据
    sockaddr_in sockaddrServer;
    sockaddrServer.sin_family = AF_INET;
    sockaddrServer.sin_port = htons(port);
    sockaddrServer.sin_addr.s_addr = inet_addr(address);

    //连接,sock与目标服务器连接
    iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));


    if (iErrMsg < 0)
    {
        //qDebug()<<"connect failed with error :"<<iErrMsg;
        //qDebug()<<"Error:"<<errno;
        rlt = 3;
        return rlt;
    }

    return rlt;
}

int M_Client::SendMsg(const char* msg, int len)
{
    int rlt = 0;

    int iErrMsg = 0;

    //发送消息，指定sock发送消息
    iErrMsg = send(m_sock, msg, len, 0);
    if (iErrMsg < 0)
        //发送失败
    {
        //qDebug()<<"send msg failed with error :"<<iErrMsg;

        rlt = 1;
        return rlt;
    }

    return rlt;
}

void M_Client::Close()
{
    closesocket(m_sock);
}

int M_Client::Modbus_sender_single(int Ref, int addr, int value)
{
    unsigned char Temp_buf[20];
    Temp_buf[0] = Ref;
    Temp_buf[1] = 0;
    Temp_buf[2] = 0;
    Temp_buf[3] = 0;
    Temp_buf[4] = 0;//从ID开始的字节数
    Temp_buf[5] = 9;
    Temp_buf[6] = id;//从机ID
    Temp_buf[7] = 16;//命令代码
    Temp_buf[8] = (addr-1)/256;//addr head
    Temp_buf[9] = (addr-1) % 256;
    Temp_buf[10] = 0;//number of addr
    Temp_buf[11] = 1;
    Temp_buf[12] = 2;//# of Bytes for values
    Temp_buf[13] = value/256;//values
    Temp_buf[14] = value%256;
    int rlt = SendMsg((char*)Temp_buf, 15);
    return rlt;
}
