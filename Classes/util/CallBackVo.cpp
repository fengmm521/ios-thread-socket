//
//  CallBackVo.cpp
//  game1
//
//  Created by 俊盟科技1 on 8/21/14.
//
//

#include "CallBackVo.h"


bool EventCallBackStrVO::init()
{
	return true;
}

//执行方法
void EventCallBackStrVO::execute(std::string e,cocos2d::Ref* obj)
{
	if (m_target  && m_funf)
	{
		(m_target->*(m_funf))(e,obj);
	}
}
//创建方法
EventCallBackStrVO* EventCallBackStrVO::initWithData(cocos2d::Ref* target,Event_CallBackStr func)
{
	EventCallBackStrVO* tmp = new EventCallBackStrVO;
	tmp->autorelease();
	tmp->m_target=target;
	tmp->m_funf=func;
	return tmp;
}


bool EventCallBackDicVO::init()
{
    return true;
}

//执行方法
void EventCallBackDicVO::execute(__Dictionary *dic,cocos2d::Ref* obj)
{
    if (m_target  && m_funf)
    {
        (m_target->*(m_funf))(dic,obj);
    }
}
//创建方法
EventCallBackDicVO* EventCallBackDicVO::initWithData(cocos2d::Ref* target,Event_CallBackDic func)
{
    EventCallBackDicVO* tmp = new EventCallBackDicVO;
    tmp->autorelease();
    tmp->m_target=target;
    tmp->m_funf=func;
    return tmp;
}