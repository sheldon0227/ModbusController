#pragma once
#include <stdio.h>
#include<windows.h>
//#pragma comment(lib,"ws2_32.lib")
//#pragma comment(lib,"libws2_32.a")

class M_Client
{
public:
    //Constructor
    M_Client(const char* Addr, int Port, int Id);
    ~M_Client(){};
    //连接
    int Connect();
    //发送TCP包
    int SendMsg(const char* msg, int len);
    //关闭
    void Close();
    //发送modbus包
    int Modbus_sender_single(int Ref, int addr, int value);

private:
    SOCKET m_sock;
    int port;
    const char* address;
    int id;
};






