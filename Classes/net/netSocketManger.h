//
//  netSocketManger.h
//  sockettest
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by 俊盟科技1 on 11/14/14.
//
//

#ifndef __sockettest__netSocketManger__
#define __sockettest__netSocketManger__

#include "cocos2d.h"
#include "SocketData.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SocketDelegate.h"
//#include "Protobuf.h"

typedef struct serverAdress
{
    std::string IP;        //服务器IP
    unsigned short port;   //端口
    std::string name;//服务器名称
    
}ServerAdressST;


class netSocketManger:public cocos2d::Ref
{
public:
    static netSocketManger* sharednetSocketManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    netSocketManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~netSocketManger(void);
    
public:
    
    /**
     * Init netSocketManger
     */
    virtual bool init();
    
    void connectSocket();
    void endSocket();
    
    int send(NetWorkCommandStruct module, std::string &msg, SendDataDelegate *delegate);
    
    void removeSendDelegate(SendDataDelegate *delegate);
    
    void setPushDelegate(PushDataDelegate *delegate);
    PushDataDelegate* getPushDelegate();
    
    void setStatusDelegate(SocketStatusDelegate *delegate);
    SocketStatusDelegate* getStatusDelegate();
    
    //获取服务器列表
    std::list<ServerAdressST> getServerList();
    
    ServerAdressST server;
    
    //主线程更新
    void update(float delta);
private:
    
    std::thread threadSend;
    std::thread threadRecive;
    bool isFirstConnect;
    
    unsigned int s_tea; //首次登陆服务器的tokn
    
    void createReciveThread();
    //服务器地址列表
    std::list<ServerAdressST> m_serverList;
    //发送数据线程
    void sendThread();
    //接收数据线程
    void reciveThread();
};

#endif /* defined(__sockettest__netSocketManger__) */
