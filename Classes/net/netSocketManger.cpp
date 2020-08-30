//
//  netSocketManger.cpp
//  sockettest
//
//  Created by 俊盟科技1 on 11/14/14.
//
//

#include "netSocketManger.h"
#include "netConfig.h"
#include "ODSocket.h"
#include "Queue.h"
#include <thread>
//#include <endian.h>
#include "zlib.h"
#include<netinet/in.h>


static ODSocket cSocket;
bool isConnect = false;

using namespace cocos2d;
typedef std::map<uint64_t, SendDataDelegate*> SendDelegateMap;

SendDelegateMap m_sendDelegateList;             //向服务器发送消息代理

PushDataDelegate *m_pushDelegate = NULL;        //接收服务器消息代理

SocketStatusDelegate *m_statusDelegate = NULL;  //网络状态代理

CQueue<SocketData>  m_sendQueue;//发送连表
CQueue<SocketData>  m_mainQueue;//接收连表
CEvent *m_sendEvent = NULL;
uint64_t m_sn = 0;
bool m_isFirst;
//std::thread::id m_sendNtid;
//std::thread::id m_recvNtid;
std::mutex m_mutexx;

static netSocketManger *s_sharednetSocketManger = nullptr;

netSocketManger *netSocketManger::sharednetSocketManger()
{
    
    if (s_sharednetSocketManger == nullptr)
    {
        s_sharednetSocketManger = new netSocketManger();
        if (!s_sharednetSocketManger || !s_sharednetSocketManger->init())
        {
            CC_SAFE_DELETE(s_sharednetSocketManger);
        }
    }
    return s_sharednetSocketManger;
}

void netSocketManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharednetSocketManger);
}

netSocketManger::netSocketManger(void)
{
  m_isFirst = true;
}

netSocketManger::~netSocketManger(void)
{
   
}

bool netSocketManger::init()
{
    bool bRet = false;
    do
    {
        
        server.IP = kServerIP;
        server.port = kServerPort;
        server.name = "light";
        
        isFirstConnect = true;
        
        //查看服务器反回数据方法
        Director::getInstance()->getScheduler()->scheduleUpdate(this, -1, false);
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}

int netSocketManger::send(NetWorkCommandStruct module, std::string &body, SendDataDelegate *delegate)
{
    if (!delegate ||  !isConnect) {
        return -1;
    }
    
    m_sn++;
    
    m_sendDelegateList[m_sn] = delegate;
    
    SocketData *data = newSocketData();
    data->sn = m_sn;
    
    if (body.empty()) {
        data->body = "";
        data->bodyLen = 0;
    }else {
        int bodyStrLen = (int)body.size();
        //data->body = (char*)malloc(bodyStrLen);
        const char* tmp = body.data();
        data->sendData = (char*)malloc(bodyStrLen);
        memcpy(data->sendData, tmp, bodyStrLen);
        data->bodyLen = bodyStrLen;
    }
    
    data->module.cmd = module.cmd;
    
    m_sendQueue.Push(data);
    
    m_sendEvent->Post();
    
    return (int)m_sn;
}

void netSocketManger::removeSendDelegate(SendDataDelegate *delegate)
{
    SendDelegateMap::iterator it = m_sendDelegateList.begin();
    while (it != m_sendDelegateList.end()) {
        if (it->second == delegate)
            m_sendDelegateList.erase(it++);
        else
            ++it;
    }
}

void netSocketManger::setPushDelegate(PushDataDelegate *delegate)
{
    m_pushDelegate = delegate;
}
PushDataDelegate* getPushDelegate()
{
    return m_pushDelegate;
}

void netSocketManger::setStatusDelegate(SocketStatusDelegate *delegate)
{
    m_statusDelegate = delegate;
}
SocketStatusDelegate* netSocketManger::getStatusDelegate()
{
    return m_statusDelegate;
}

//主线程更新
void netSocketManger::update(float dt)
{
    if (!m_mainQueue.IsEmpty()) {
        SocketData *data = m_mainQueue.Pop();
        
        if (data->eventType == REQUEST) {
            if (m_pushDelegate) {
                m_pushDelegate->pushHandler(data);
            }
        }else if (data->eventType == RESPONSE) {
            SendDelegateMap::iterator it = m_sendDelegateList.find(data->sn);
            if (it != m_sendDelegateList.end()) {
                SendDataDelegate *delegate = it->second;
                m_sendDelegateList.erase(it);
                delegate->sendHandler(data);
            }
        }else if (data->eventType == CONNECT_SUCCEED || data->eventType == CONNECT_FAIL || data->eventType == DISCONNECT) {
            if (data->eventType != CONNECT_SUCCEED) {
                if (!threadSend.joinable()) {//线程已启动
                    threadSend.~thread();
                    delete m_sendEvent;
                    m_sendEvent = NULL;
                    
                }
                if (!threadRecive.joinable()) {
                    threadRecive.~thread();
                }
                
                while (!m_sendQueue.IsEmpty()) {
                    SocketData *sendData = m_sendQueue.Pop();
                    //litaoming update
                    //CC_SAFE_FREE(data->body);
                    //CC_SAFE_FREE(sendData->reciveBody);
                    free(sendData);
                }
                
                while (!m_mainQueue.IsEmpty()) {
                    SocketData *mainData = m_mainQueue.Pop();
                    if (mainData != data) {
                        //CC_SAFE_FREE(mainData->body);
                        //CC_SAFE_FREE(mainData->addContent);
                    }
                    
                }
                if (isConnect) {
                    cSocket.Close();
                    cSocket.Clean();
                    isConnect = false;
                }
                m_isFirst = true;
                m_sendDelegateList.clear();
            }
            if (m_statusDelegate) {
                m_statusDelegate->statusHandler(data->eventType);
            }
        }
        //CC_SAFE_FREE(data->body);
        //CC_SAFE_FREE(data->addContent);
        free(data);
    }
}
void netSocketManger::connectSocket()
{
    
    if (!isConnect) {
        cSocket.Init();
        cSocket.Create(AF_INET,SOCK_STREAM,0);
        if (m_sendEvent == NULL) {
            m_sendEvent = new CEvent();
        }
        
        s_tea = 0;
        threadSend = std::thread(&netSocketManger::sendThread,this);//创建一个分支线程，回调到sendThread函数里
        threadSend.detach();
    }
    
    log("send thread is run");
}

void netSocketManger::createReciveThread()
{
    
    threadRecive = std::thread(&netSocketManger::reciveThread,this);
    threadRecive.detach();//启动接收线程
}

void netSocketManger::endSocket()
{
    if (!threadSend.joinable()) {//线程已启动
        threadSend.~thread();
        delete m_sendEvent;
        m_sendEvent = NULL;
        
    }
    if (!threadRecive.joinable()) {
        threadRecive.~thread();
    }

    while (!m_sendQueue.IsEmpty()) {
        SocketData *sendData = m_sendQueue.Pop();
        //CC_SAFE_FREE(sendData->body);
        free(sendData);
    }
    
    while (!m_mainQueue.IsEmpty()) {
        SocketData *mainData = m_mainQueue.Pop();
        CC_SAFE_FREE(mainData);
        //CC_SAFE_FREE(mainData->addContent);
    }
    
    if (isConnect) {
        cSocket.Close();
        cSocket.Clean();
        isConnect = false;
    }
    m_isFirst = true;
    m_sendDelegateList.clear();
}
std::list<ServerAdressST> netSocketManger::getServerList()
{
    return m_serverList;
}
//发送数据线程
void netSocketManger::sendThread()
{

    //bool connect = cSocket.Connect(kServerIP,kServerPort);
    bool connect = cSocket.Connect(netSocketManger::sharednetSocketManger()->server.IP.c_str(),netSocketManger::sharednetSocketManger()->server.port);
   // cSocket.Send("bbb",strlen("bbb")+1,0);

    if (connect) {
        isConnect = true;
        SocketData *errorData = newSocketData();
        errorData->eventType = CONNECT_SUCCEED;
        m_mainQueue.Push(errorData);
        //创建接收线程
        m_mutexx.lock();
        netSocketManger::sharednetSocketManger()->createReciveThread();
        m_mutexx.unlock();
        m_sendEvent->Lock();
        while (true) {
            while (!m_sendQueue.IsEmpty()) {
                SocketData *data = m_sendQueue.Pop();
                uLong comprLen = data->bodyLen;
                const char *compr = data->sendData;
                
                T_MSGHEAD_T msgHead;
                
                msgHead.cmd = (unsigned short)data->module.cmd;
                msgHead.com = 0;
                msgHead.enc = 0;
                msgHead.eno = 0;
                msgHead.idx = (uInt)data->sn;
                msgHead.len = (unsigned short)comprLen;
                msgHead.tea = 0;//s_tea;
                
                unsigned char *sendData = (unsigned char*)malloc(comprLen);
                unsigned int pos = 0;
                
                memcpy(&sendData[pos], compr, comprLen);//body
                pos += comprLen;
                
                int ret = cSocket.Send((char*)sendData,pos,0);
                log("发送:%s",compr);
                if (ret <= 0) {
                    m_sendEvent->Unlock();
                    free(sendData);
                    SocketData *errorData = newSocketData();
                    errorData->eventType = DISCONNECT;
                    m_mainQueue.Push(errorData);
                    return;
                }

                free(data->sendData);
                free(data);
                free(sendData);

                log("-----发送数据长度len:%d------",msgHead.len);
                log("-----------");
                
            }
            
            m_sendEvent->Wait();
        }
        m_sendEvent->Unlock();
    }else {
        isConnect = false;
        SocketData *errorData = newSocketData();
        errorData->eventType = CONNECT_FAIL;
        m_mainQueue.Push(errorData);
    }
}
//接收数据线程
void netSocketManger::reciveThread()
{
    char buff[MAX_BUFF];
    int recv_len = 0;
    bool hasMorePackets = false;
 
    while(true)
    {
        
        if( !hasMorePackets ) {
            
            int ret = cSocket.Recv(buff, MAX_BUFF - recv_len, 0);
            
            if (ret <= 0) {
                isConnect = false;
                SocketData *errorData = newSocketData();
                errorData->eventType = DISCONNECT;
                m_mainQueue.Push(errorData);
                break;
            }
            if(ret < 1)
                break;
        }
       
            T_MSGHEAD_T reciveCmd;
            reciveCmd.len = 1;        //接收缓存中的前64个字节数据
            reciveCmd.enc = 0;        //数据是否加密标志
            reciveCmd.com = 0;        //数据是否压缩标志，这两个我们都设置为否，因为我们的数据本身只有一位
            reciveCmd.idx = (uInt)m_sn;      //包索引也设置为固定值，因为我们只有一个接口，这里发arduino马上就会回.
            reciveCmd.tea = 0;
            unsigned int pos = 0;
            
            SocketData *data = newSocketData();
            data->module.cmd = 100;   //先将100设置为灯的协议号
            data->module.eno = 0;     //主动将错误码设置为0,即正常
            data->eventType = RESPONSE;
            data->sn = reciveCmd.idx;
            std::string datax(&buff[pos],reciveCmd.len);
            data->body = std::string(datax);
            pos += reciveCmd.len;
            data->bodyLen = reciveCmd.len;
        
        log("接收:%s",data->body.c_str());
            log("*************************************************");
            log("*************************************************");
            
            m_mainQueue.Push(data);
            
            hasMorePackets = false;
        
    }
    
}
