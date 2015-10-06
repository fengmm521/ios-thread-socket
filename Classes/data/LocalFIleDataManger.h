//
//  LocalFIleDataManger.h
//  light
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by woodcol on 15/2/24.
//
//

#ifndef __light__LocalFIleDataManger__
#define __light__LocalFIleDataManger__

#include "cocos2d.h"
#include "UserDataConfig.h"
using namespace cocos2d;
class LocalFIleDataManger:public cocos2d::Ref
{
public:
    static LocalFIleDataManger* sharedLocalFIleDataManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    LocalFIleDataManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~LocalFIleDataManger(void);
    
public:
    
    
    
    
    
    void readUserData();
    void saveUserData();
    
    
    LightType m_lightTypeData; //当前灯状态
    
    /**
     * Init LocalFIleDataManger
     */
    virtual bool init();
    
private:
    
    __Dictionary* readLocalDataFromFile(std::string &path);
    bool saveUserDataToFile(std::string &path,__Dictionary* data);
    
};

#endif /* defined(__light__LocalFIleDataManger__) */
