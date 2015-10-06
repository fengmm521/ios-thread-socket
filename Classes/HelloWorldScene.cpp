#include "HelloWorldScene.h"
#include "ClientSocketManger.h"
#include "ObserverManger.h"
#include "LocalFIleDataManger.h"
USING_NS_CC;

#define kScal 1.02f

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
void HelloWorld::onEnter()
{
    Layer::onEnter();
}
void HelloWorld::onExite()
{
    ObserverManger::shareObserverManger()->unregisterCallBackEvent("netstate", this, event_callbackStr(HelloWorld::netStateChangeEvent));
    Layer::onExit();
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    ClientSocketManger::sharedClientSocketManger();
    m_localDataManger = LocalFIleDataManger::sharedLocalFIleDataManger();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    m_bg1_0 = Sprite::create("light1_bg_0.png");
    m_bg1_0->setAnchorPoint(Vec2(0,0));
    m_bg1_0->setPosition(Vec2(0,0));
    this->addChild(m_bg1_0,0);
    
    m_bg1_1 = Sprite::create("light1_bg_1.png");
    m_bg1_1->setAnchorPoint(Vec2(0,0));
    m_bg1_1->setPosition(Vec2(0,0));
    this->addChild(m_bg1_1,0);
    

    m_bg2_0 = Sprite::create("light2_bg_0.png");
    m_bg2_0->setAnchorPoint(Vec2(0,0));
    m_bg2_0->setPosition(Vec2(0,0));
    this->addChild(m_bg2_0,0);
    
    m_bg2_1 = Sprite::create("light2_bg_1.png");
    m_bg2_1->setPosition(Vec2(0,0));
    m_bg2_1->setAnchorPoint(Vec2(0,0));
    this->addChild(m_bg2_1,0);
    

    m_light1_n = Sprite::create("light1_lt_0.png");
    m_light1_n->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
    this->addChild(m_light1_n,1);
    
    m_light1_0 = Sprite::create("light1_lt_1.png");
    m_light1_0->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
    this->addChild(m_light1_0,1);
    
    m_light1_1 = Sprite::create("light1_lt_2.png");
    m_light1_1->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
    this->addChild(m_light1_1,1);
    
    
    m_light2_line = Sprite::create("light2_line.png");
    m_light2_line->setPosition(Vec2(origin.x + 200 + visibleSize.width/2,
                                    origin.y +  800 + visibleSize.height/2));
    this->addChild(m_light2_line,1);
    
    m_light2_0 = Sprite::create("light2_light_0.png");
    m_light2_0->setPosition(Vec2(origin.x + visibleSize.width/2 + 200,
                                 origin.y + visibleSize.height/2 + 300));
    this->addChild(m_light2_0,1);
    
    m_light2_1 = Sprite::create("light2_light_1.png");
    m_light2_1->setPosition(Vec2(origin.x + visibleSize.width/2 + 200,
                                 origin.y + visibleSize.height/2 + 250));
    this->addChild(m_light2_1,1);
    
    
    m_localDataManger->m_lightTypeData.selectBG = 2;
    m_bg1_0->setVisible(false);
    m_bg1_1->setVisible(false);
    
    m_light1_0->setVisible(false);
    m_light1_1->setVisible(false);
    m_light1_n->setVisible(false);
    
    m_light2_1->setVisible(false);
    

    showText = LabelTTF::create("未发送请求!", "Arial", 50);
    showText->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - showText->getContentSize().height));
    this->addChild(showText, 1);
    
    nowLightTxt = LabelTTF::create("灯:关", "Arial", 50);
    nowLightTxt->setPosition(Vec2(origin.x + visibleSize.width/2,
                               origin.y + visibleSize.height - nowLightTxt->getContentSize().height - 100));
    this->addChild(nowLightTxt, 1);
    

    asktype = 0;
    
    ObserverManger::shareObserverManger()->registerCallBackEvent("netstate", this, event_callbackStr(HelloWorld::netStateChangeEvent));
    
    this->setLightTypeTxt(m_localDataManger->m_lightTypeData.lightType);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    if (ClientSocketManger::sharedClientSocketManger()->nettype != 11) {
        ClientSocketManger::sharedClientSocketManger()->connectSocket();
    }
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
        if (m_localDataManger->m_lightTypeData.lightType) {
            this->closeLight();
        }else{
            this->openLight();
        }
    
}
void HelloWorld::netStateChangeEvent(std::string msg,cocos2d::Ref *obj)
{
    __String* str = (__String*)obj;
    int type = atoi(str->getCString());
    switch (type) {
        case 11:
        {
            showText->setString("电灯连接成功!");
        }
            break;
        case 12:
        {
            showText->setString("电灯连接失败");
        }
            break;
        case 13:
        {
            showText->setString("电灯断开连接");
        }
            break;
        default:
            break;
    }
}

void HelloWorld::openLight()//开灯
{
    this->askSystemType(2); //2想开灯
}
void HelloWorld::closeLight()//关灯
{
    this->askSystemType(1); //1想关灯
}
void HelloWorld::askSystemType(int type) //询问系统工作情况
{
    asktype = type;
    //延时10毫秒发送复位请求
    CallFunc* func = CallFunc::create([=]()->void{
        
        m_localDataManger->m_lightTypeData.sendOtherData = '1';
        char data = '1';
        __String* str = __String::createWithFormat("%c",data);
        ClientSocketManger::sharedClientSocketManger()->send(str->getCString(), 100, this);
    });
    this->runAction(Sequence::create(DelayTime::create(0.001),func, NULL));
}
void HelloWorld::sendOpenLight()
{
    //延时10毫秒发送关灯请求
    CallFunc* func = CallFunc::create([=]()->void{
        if (!m_localDataManger->m_lightTypeData.lightType) {
            if (m_localDataManger->m_lightTypeData.lastOpenSend == 'z') {
                m_localDataManger->m_lightTypeData.lastOpenSend = 'a';
            }else{
                m_localDataManger->m_lightTypeData.lastOpenSend++;
            }
            char data = m_localDataManger->m_lightTypeData.lastOpenSend;
            __String* str = __String::createWithFormat("%c",data);
            ClientSocketManger::sharedClientSocketManger()->send(str->getCString(), 100, this);
        }
    });
    this->runAction(Sequence::create(DelayTime::create(0.01),func, NULL));
}
void HelloWorld::sendCloseLight()
{
    //延时10毫秒发送关灯请求
    CallFunc* func = CallFunc::create([=]()->void{
        if (m_localDataManger->m_lightTypeData.lightType) {
            if (m_localDataManger->m_lightTypeData.lastCloseSend == 'Z') {
                m_localDataManger->m_lightTypeData.lastCloseSend = 'A';
            }else{
                m_localDataManger->m_lightTypeData.lastCloseSend++;
            }
            char data = m_localDataManger->m_lightTypeData.lastCloseSend;
            __String* str = __String::createWithFormat("%c",data);
            ClientSocketManger::sharedClientSocketManger()->send(str->getCString(), 100, this);
        }
    });
    this->runAction(Sequence::create(DelayTime::create(0.01),func, NULL));
}
void HelloWorld::sendResetSys()   //发送复位请求
{
    //延时10毫秒发送复位请求
    CallFunc* func = CallFunc::create([=]()->void{
        
        log("发送复位请求中。。。");
        m_localDataManger->m_lightTypeData.sendOtherData = '2';
        char data = '2';
        __String* str = __String::createWithFormat("%c",data);
        
        ClientSocketManger::sharedClientSocketManger()->send(str->getCString(), 100, this);
    });
    this->runAction(Sequence::create(DelayTime::create(0.01),func, NULL));
}
void HelloWorld::setLightTypeTxt(bool type) //设置灯状态文本
{
    if (type) {
        nowLightTxt->setString("灯:开");
        if (m_localDataManger->m_lightTypeData.selectBG == 1) {
            m_bg1_1->setVisible(true);
            m_light1_1->setVisible(true);
            m_light1_0->setVisible(false);
            m_light1_n->setVisible(false);
        }else{
            m_bg2_1->setVisible(true);
            m_light2_1->setVisible(true);
            m_light2_0->setVisible(false);
        }
    }else{
        nowLightTxt->setString("灯:关");
        if (m_localDataManger->m_lightTypeData.selectBG == 1) {
            m_bg1_1->setVisible(false);
            m_light1_1->setVisible(false);
            m_light1_0->setVisible(true);
            m_light1_n->setVisible(false);
        }else{
            m_bg2_1->setVisible(false);
            m_light2_1->setVisible(false);
            m_light2_0->setVisible(true);
        }
    }
}
void HelloWorld::reciveDataHandler(std::string &msg,int type,bool is_push)
{
    if (is_push) {
        log("get light push data");//其实从我们的数据上看，灯是不会主动给客户端推送消息的，最少现在没有这功能
    }else{
        
        if (type == 100) { //我们设置的100是灯的协议编号
            char backstr = msg[0];
        
            if (backstr == m_localDataManger->m_lightTypeData.lastCloseSend) {
                log("关灯操作成功");
                showText->setString("关灯操作成功");
                m_localDataManger->m_lightTypeData.sendOtherData = '#';
                m_localDataManger->m_lightTypeData.lightType = false;
                
            }else if(backstr == m_localDataManger->m_lightTypeData.lastOpenSend){
                log("开灯操作成功");
                showText->setString("开灯操作成功");
                m_localDataManger->m_lightTypeData.sendOtherData = '#';
                m_localDataManger->m_lightTypeData.lightType = true;
            }else if(backstr == '1'){
                log("系统出错了");
                showText->setString("系统出错了");
                m_localDataManger->m_lightTypeData.lightType = false;
                //是否发送复位信息.这里默认是发送
                this->sendResetSys();
            }else if(backstr == '0' && m_localDataManger->m_lightTypeData.sendOtherData == '1'){
                log("系统工作正常");
                switch (asktype) {
                    case 1://请求关灯
                    {
                        this->sendCloseLight();
                    }
                        break;
                    case 2://请求开灯
                    {
                        this->sendOpenLight();
                    }
                        break;
                    case 3://询问系统工作情况
                    {
                        m_localDataManger->m_lightTypeData.sendOtherData = '#';
                        log("系统工作正常");
                        showText->setString("系统工作正常");
                    }
                        break;
                    default:
                        break;
                }
            }else if(backstr == '0' && m_localDataManger->m_lightTypeData.sendOtherData == '2'){
                log("系统复位成功");
                showText->setString("系统复位成功");
                m_localDataManger->m_lightTypeData.lastOpenSend = 'a';
                m_localDataManger->m_lightTypeData.lastCloseSend = 'A';
                m_localDataManger->m_lightTypeData.lightType = false;
                m_localDataManger->m_lightTypeData.sendOtherData = '#';
                switch (asktype) {
                    case 1://关灯
                    {
                        log("正在进行复位后关灯请求!");
                        showText->setString("正在进行复位后关灯请求!");
                        this->sendCloseLight();
                    }
                        break;
                    case 2://开灯
                    {
                        log("正在进行复位后开灯请求!");
                        showText->setString("正在进行复位后开灯请求!");
                        this->sendOpenLight();
                    }
                        break;
                    case 3://询问
                    {
                        log("询问之后系统复位成功");
                        showText->setString("询问之后系统复位成功!");
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
        this->setLightTypeTxt(m_localDataManger->m_lightTypeData.lightType);
    }
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    m_light1_0->setScale(kScal, kScal);
    m_light1_1->setScale(kScal, kScal);
    m_light2_0->setScale(kScal, kScal);
    m_light2_1->setScale(kScal, kScal);
    return true;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    m_light1_0->setScale(1.0f, 1.0f);
    m_light1_1->setScale(1.0f, 1.0f);
    m_light2_0->setScale(1.0f, 1.0f);
    m_light2_1->setScale(1.0f, 1.0f);
    if (m_localDataManger->m_lightTypeData.lightType) {
        this->closeLight();
    }else{
        this->openLight();
    }
}
void HelloWorld::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}
