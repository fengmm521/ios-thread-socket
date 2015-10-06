//
//  CallBackVo.h
//  game1
//  这个是创建一些基本的cocos2d类时会用到
//  Created by 俊盟科技1 on 8/21/14.
//
//

#ifndef __game1__CallBackVo__
#define __game1__CallBackVo__
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

typedef void (Ref::*Event_CallBackStr)(std::string,cocos2d::Ref*);
#define event_callbackStr(_SELECTOR) (Event_CallBackStr)(&_SELECTOR)

typedef void (Ref::*Event_CallBackDic)(__Dictionary* dic,cocos2d::Ref*);
#define event_callbackDic(_SELECTOR) (Event_CallBackDic)(&_SELECTOR)


/*******************/

//返回字符串加基类
class EventCallBackStrVO:public Ref
{
public:
	EventCallBackStrVO()
    :m_target(NULL)
    ,m_funf(NULL)
	{}
	Ref* m_target;
	Event_CallBackStr m_funf;
	virtual void execute(std::string e,Ref* obj);
	static EventCallBackStrVO* initWithData(Ref* target,Event_CallBackStr funf);
	virtual bool init();
};

//返回字典加基类
class EventCallBackDicVO:public Ref
{
public:
    EventCallBackDicVO()
    :m_target(NULL)
    ,m_funf(NULL)
    {}
    Ref* m_target;
    Event_CallBackDic m_funf;
    virtual void execute(__Dictionary *dic,Ref* obj);
    static EventCallBackDicVO* initWithData(Ref* target,Event_CallBackDic funf);
    virtual bool init();
};
#endif /* defined(__game1__CallBackVo__) */
