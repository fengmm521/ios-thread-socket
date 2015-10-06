//
//  ClientSocketManger.h
//  sockettest
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by 俊盟科技1 on 11/18/14.
//
//

#ifndef __sockettest__ClientSocketManger__
#define __sockettest__ClientSocketManger__

#include "cocos2d.h"
#include "SocketDelegate.h"
class ClientSocketManger:public cocos2d::Ref,PushDataDelegate,SendDataDelegate,SocketStatusDelegate
{
public:
    static ClientSocketManger* sharedClientSocketManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    ClientSocketManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~ClientSocketManger(void);
    
public:
    
    /**
     * Init ClientSocketManger
     */
    virtual bool init();
    
    int nettype; //13:未连接,11:连接成功,12:连接错误
    
    //与服务器建立连接
    void connectSocket();
    //与服务器断开连接
    void endSocket();
    int getNetType(); //获取当前网络类型
    bool isConnect(); //当前是否连接网络
    
    //发送消息后，服务器返回消息
    virtual void sendHandler(SocketData *socketData);
    //向服务器发送数据,data:数据，type:接口编号,接收消息回调
    std::map<int,SocketReciveDelegate*> m_reciveDataMap;

    int send(std::string msg,int type,SocketReciveDelegate* reciveDel);
    //服务器主动消息回调
    virtual void pushHandler(SocketData *socketData);
    //接收来自服务器退送消息的对象map
    std::map<SocketReciveDelegate*,SocketReciveDelegate*> m_serverPushDataMap;
    //注册接受服务器推送消息的对象
    void registerFromServerPushDataCallBack(SocketReciveDelegate* pushDele);
    //移除来自服务器推送消息的对象
    void unregisterFromServerPushDataCallBack(SocketReciveDelegate* pushDele);
    
    //服务器状态改变回调
    virtual void statusHandler(EventType eventType);
    
};

#endif /* defined(__sockettest__ClientSocketManger__) */
