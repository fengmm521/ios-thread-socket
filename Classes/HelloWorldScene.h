#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SocketDelegate.h"
#include "UserDataConfig.h"

class LocalFIleDataManger;

class HelloWorld : public cocos2d::Layer,SocketReciveDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void onEnter();
    virtual void onExite();
    
    
    virtual void reciveDataHandler(std::string &msg,int type,bool is_push = false);
    
    void netStateChangeEvent(std::string msg,cocos2d::Ref *obj);
    
    
    cocos2d::LabelTTF* showText;     //显示服务器状态
    cocos2d::LabelTTF* nowLightTxt;  //显示当前灯开关状态
    
    
    cocos2d::Sprite* m_bg1_0;
    cocos2d::Sprite* m_bg1_1;
    
    cocos2d::Sprite* m_bg2_0;
    cocos2d::Sprite* m_bg2_1;
    
    cocos2d::Sprite* m_light1_n;
    cocos2d::Sprite* m_light1_0;
    cocos2d::Sprite* m_light1_1;
    
    cocos2d::Sprite* m_light2_line;
    cocos2d::Sprite* m_light2_0;
    cocos2d::Sprite* m_light2_1;
    
    void openLight();//开灯
    void closeLight();//关灯
    
    int asktype; //询问系统工作情况目的，1:想关灯,2:想开灯,3:只是查询系统工作情况,0:未设置
    void askSystemType(int type = 3); //询问系统工作情况
    
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
    
private:
    
    LocalFIleDataManger* m_localDataManger;
    
//    int selectBG; //1哦竖屏，2为横屏
//    
//    char lastOpenSend;  //上次开灯时发送的数据字符
//    char lastCloseSend; //上次关灯时发送的数据字符
//    char sendOtherData; //上次复位或查询系统情况发送字符
//    bool m_lightType;   //当前灯是开是关
    
    void sendOpenLight();   //发送开灯请求
    void sendCloseLight();  //发送关灯请求
    void sendResetSys();    //发送复位请求
    
    void setLightTypeTxt(bool type); //设置灯状态文本
};

#endif // __HELLOWORLD_SCENE_H__
