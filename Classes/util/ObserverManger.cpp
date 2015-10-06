//
//  ObserverManger.cpp
//  game1
//
//  Created by 俊盟科技1 on 8/20/14.
//
//

#include "ObserverManger.h"

static ObserverManger *s_sharedObserverManger = NULL;
bool ObserverManger::init()
{
    m_eventMapStr.clear();
    
	return true;
}
ObserverManger* ObserverManger::shareObserverManger(void)
{
	if (!s_sharedObserverManger)
	{
		s_sharedObserverManger = new ObserverManger();
		s_sharedObserverManger->init();
	}
	return s_sharedObserverManger;
}

//发出消息
void ObserverManger::callBackEvent(const char *e,cocos2d::Ref* obj)
{
    std::string tmp = std::string(e);
    std::list<cocos2d::Ref*> refList =m_eventMapStr[e];
	if(refList.empty())
    {
		return;
    }
    for (std::list<cocos2d::Ref*>::iterator it = refList.begin(); it != refList.end(); it++) {
        EventCallBackStrVO* fun=(EventCallBackStrVO*)*it;
		if(fun)
		{
			fun->execute(e,obj);
		}
    }
}

//注册
void ObserverManger::registerCallBackEvent(const char *name,cocos2d::Ref* ins,Event_CallBackStr selector)
{
	EventCallBackStrVO *fun=EventCallBackStrVO::initWithData(ins,selector);
    fun->retain();
    std::list<cocos2d::Ref*> arr=m_eventMapStr[name];
    
	if(arr.empty())
	{
        arr.push_back(fun);
        m_eventMapStr[name].clear();
        m_eventMapStr[name] = arr;
        
	}else{
        arr.push_back(fun);
        m_eventMapStr[name].clear();
        m_eventMapStr[name] = arr;
    }
}

//取消注册有字符串数据的方法
void ObserverManger::unregisterCallBackEvent(const char* name,cocos2d::Ref* ins,Event_CallBackStr selector)
{
    std::list<cocos2d::Ref*> refList = m_eventMapStr[name];
	if(refList.empty())
	{
		return;
	}
    
    for (std::list<cocos2d::Ref*>::iterator it = refList.begin(); it != refList.end();)
    {
        EventCallBackStrVO* tmp = (EventCallBackStrVO*)(*it);
        if(tmp->m_target == ins && selector == tmp->m_funf){
            (*it)->release();
            it = refList.erase(it);
        }else{
            it++;
        }
    }
    m_eventMapStr[name].clear();
    m_eventMapStr[name] = refList;
}

//取消注册
void ObserverManger::unregisterCallBackEvent(const char *name)
{
    
    map<string,std::list<Ref*>>::iterator ittmp = m_eventMapStr.find(name);
    if (ittmp != m_eventMapStr.end()) {
        
        std::list<Ref*> refList = m_eventMapStr[name];
        if(!refList.empty())
        {
            refList.clear();
            m_eventMapStr.erase(ittmp);
        }
    }
}
