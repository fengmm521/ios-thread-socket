//
//  ObserverManger.h
//  game1
//  创建观查者管理类,
//  Created by 俊盟科技1 on 8/20/14.
//
//

#ifndef __game1__ObserverManger__
#define __game1__ObserverManger__

#include "cocos2d.h"
#include "CallBackVo.h"
using namespace cocos2d;
using namespace std;

/*******************************************
 **/
class ObserverManger: public cocos2d::Ref
{
public:
	virtual bool init();
    
	virtual void callBackEvent(const char *e,Ref* obj);//发送回调函数
    
	//////////////////////////////////////////////////////////////////
	virtual void registerCallBackEvent(const char *name,Ref* ins,Event_CallBackStr selector);
    
	virtual void unregisterCallBackEvent(const char *name,Ref* ins,Event_CallBackStr selector);
    
 	virtual void unregisterCallBackEvent(const char *name);//取消注册回
    
private:
	map<string,std::list<Ref*>> m_eventMapStr;
    
public:
	static ObserverManger* shareObserverManger();
};

#endif /* defined(__game1__ObserverManger__) */
