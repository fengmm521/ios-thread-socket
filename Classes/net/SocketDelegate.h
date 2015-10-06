#ifndef __PushDataDelegate_H__
#define __PushDataDelegate_H__

#include "cocos2d.h"
#include "SocketData.h"
using namespace cocos2d;

class Message;
class PushDataDelegate{

public:
    virtual void pushHandler(SocketData *socketData) = 0;
};

class SendDataDelegate{

public:
	virtual ~SendDataDelegate();
    
    virtual int send(NetWorkCommandStruct module, std::string &msg);
    virtual void sendHandler(SocketData *socketData) = 0 ;
};

class SocketStatusDelegate{

public:
    virtual void statusHandler(EventType eventType) = 0;
    
};

class SocketReciveDelegate{
public:
    virtual void reciveDataHandler(std::string &msg,int type,bool is_push = false) = 0;
};

#endif

