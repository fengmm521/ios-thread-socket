//
//  ClientSocketManger.cpp
//  sockettest
//
//  Created by 俊盟科技1 on 11/18/14.
//
//

#include "ClientSocketManger.h"
#include "netSocketManger.h"
#include "ObserverManger.h"
//#include "UI2ServerErroMsg.h"
#include "netConfig.h"
#define kServerDisconnect "serverDisconnect"
static ClientSocketManger *s_sharedClientSocketManger = nullptr;

ClientSocketManger *ClientSocketManger::sharedClientSocketManger()
{
    if (s_sharedClientSocketManger == nullptr)
    {
        s_sharedClientSocketManger = new ClientSocketManger();
        if (!s_sharedClientSocketManger || !s_sharedClientSocketManger->init())
        {
            CC_SAFE_DELETE(s_sharedClientSocketManger);
        }
    }
    return s_sharedClientSocketManger;
}

void ClientSocketManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedClientSocketManger);
}

ClientSocketManger::ClientSocketManger(void)
{
  
}


ClientSocketManger::~ClientSocketManger(void)
{
   
}


bool ClientSocketManger::init()
{
    bool bRet = false;
    do
    {
        //服务器推送消息回调注册
        netSocketManger::sharednetSocketManger()->setPushDelegate(this);
        //服务器状态改变消息回调注册
        netSocketManger::sharednetSocketManger()->setStatusDelegate(this);
        
        nettype = 13;
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
//与服务器建立连接
void ClientSocketManger::connectSocket()
{
    netSocketManger::sharednetSocketManger()->connectSocket();
}
//与服务器断开连接
void ClientSocketManger::endSocket()
{
    netSocketManger::sharednetSocketManger()->endSocket();
}
//向服务器发送数据

int ClientSocketManger::send(std::string msg,int type,SocketReciveDelegate* reciveDel)
{
    struct NetWorkCommandStruct tmpcom;
    tmpcom.cmd = type;
    tmpcom.msgLen = (int)msg.size();
    m_reciveDataMap[type] = reciveDel;
    return this->SendDataDelegate::send(tmpcom, msg);
}
//服务器主动消息回调
void ClientSocketManger::pushHandler(SocketData *socketData)
{
    int type = socketData->module.cmd;
    for (std::map<SocketReciveDelegate*,SocketReciveDelegate*>::iterator it = m_serverPushDataMap.begin(); it != m_serverPushDataMap.end(); it++) {
        it->second->reciveDataHandler(socketData->body,type,true);
    }
}
//注册接受服务器推送消息的对象
void ClientSocketManger::registerFromServerPushDataCallBack(SocketReciveDelegate* pushDele)
{
    if (m_serverPushDataMap.find(pushDele) == m_serverPushDataMap.end()) {
        m_serverPushDataMap[pushDele] = pushDele;
    }
}
//移除来自服务器推送消息的对象
void ClientSocketManger::unregisterFromServerPushDataCallBack(SocketReciveDelegate* pushDele)
{
    std::map<SocketReciveDelegate*,SocketReciveDelegate*>::iterator it = m_serverPushDataMap.begin();
    while (it != m_serverPushDataMap.end()) {
        if (it->first == pushDele)
            m_serverPushDataMap.erase(it++);
        else
            ++it;
    }
}
//发送消息后，服务器返回消息
void ClientSocketManger::sendHandler(SocketData *socketData)
{
    int type = socketData->module.cmd;
    
    
    switch (socketData->module.eno) {
        case 0://正常
        {
            std::map<int,SocketReciveDelegate*>::iterator it = m_reciveDataMap.begin();
            if (m_reciveDataMap.find(type) != m_reciveDataMap.end()) {
                m_reciveDataMap[type]->reciveDataHandler(socketData->body, type);
                while (it != m_reciveDataMap.end()) {
                    if (it->first == type)
                        m_reciveDataMap.erase(it++);
                    else
                        ++it;
                }
            }
        }
            break;
        default://服务器错误
        {
//            UI2ServerErroMsg * tmpmsg = UI2ServerErroMsg::create(socketData->module.eno);
//            Director::getInstance()->getRunningScene()->addChild(tmpmsg,2000);
            std::map<int,SocketReciveDelegate*>::iterator it = m_reciveDataMap.begin();
            if (m_reciveDataMap.find(type) != m_reciveDataMap.end()) {
                while (it != m_reciveDataMap.end()) {
                    if (it->first == type)
                        m_reciveDataMap.erase(it++);
                    else
                        ++it;
                }
            }
        }
            break;
    }
    
}
void ClientSocketManger::statusHandler(EventType eventType)
{
//    enum EventType{
//        REQUEST = 1,
//        RESPONSE = 2,
//        CONNECT_SUCCEED = 11,
//        CONNECT_FAIL = 12,
//        DISCONNECT = 13
//    };
    //当网络状态发生改变时(连接成功，连接错误，断开连接)，会调用当前方法。
    log("net status change for %d",eventType);
    __String* str = __String::createWithFormat("%d",eventType);
    ObserverManger::shareObserverManger()->callBackEvent("netstate",str);
    nettype = eventType;
    
    
    
}
