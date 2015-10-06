

#include "SocketDelegate.h"
#include "netSocketManger.h"


SendDataDelegate::~SendDataDelegate(){
	netSocketManger::sharednetSocketManger()->removeSendDelegate(this);
}


int SendDataDelegate::send(NetWorkCommandStruct module, std::string &msg){
    return netSocketManger::sharednetSocketManger()->send(module, msg, this);
}


