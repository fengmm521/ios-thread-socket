//
//  LocalFIleDataManger.cpp
//  light
//
//  Created by woodcol on 15/2/24.
//
//

#include "LocalFIleDataManger.h"
#include "LightDataClass.h"
#include "LightDataClass.h"
static LocalFIleDataManger *s_sharedLocalFIleDataManger = nullptr;

LocalFIleDataManger *LocalFIleDataManger::sharedLocalFIleDataManger()
{
    if (s_sharedLocalFIleDataManger == nullptr)
    {
        s_sharedLocalFIleDataManger = new LocalFIleDataManger();
        if (!s_sharedLocalFIleDataManger || !s_sharedLocalFIleDataManger->init())
        {
            CC_SAFE_DELETE(s_sharedLocalFIleDataManger);
        }
    }
    return s_sharedLocalFIleDataManger;
}

void LocalFIleDataManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedLocalFIleDataManger);
}

LocalFIleDataManger::LocalFIleDataManger(void)
{
  
}


LocalFIleDataManger::~LocalFIleDataManger(void)
{
   
}


bool LocalFIleDataManger::init()
{
    bool bRet = false;
    do
    {
        this->readUserData();
        
        //this->saveUserData();
        bRet = true;
    }
    while (0);
    
    return bRet;
}
void LocalFIleDataManger::readUserData()
{
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string filepath = writablePath + kDataSavePath;
    if (FileUtils::getInstance()->isFileExist(filepath)) {
        __Dictionary* s_localUserData = __Dictionary::createWithContentsOfFile(filepath.c_str());
        __String* selectBG = (__String*)(s_localUserData->objectForKey("selectBG"));
        __String* lastOpenSend = (__String*)(s_localUserData->objectForKey("lastOpenSend"));
        __String* lastCloseSend = (__String*)(s_localUserData->objectForKey("lastCloseSend"));
        __String* lightType = (__String*)(s_localUserData->objectForKey("lightType"));
        __String* sendOtherData = (__String*)(s_localUserData->objectForKey("sendOtherData"));
        
        
        m_lightTypeData.selectBG = selectBG->intValue();
        m_lightTypeData.lastOpenSend = lastOpenSend->intValue();
        m_lightTypeData.lastCloseSend = (char)lastCloseSend->intValue();
        m_lightTypeData.lightType =  lightType->intValue() != 0 ? true:false;
        m_lightTypeData.sendOtherData = sendOtherData->intValue();;
        log("read data ok!--%d-%c-%c-%d-%c",m_lightTypeData.selectBG,m_lightTypeData.lastOpenSend,m_lightTypeData.lastCloseSend,m_lightTypeData.lightType,m_lightTypeData.sendOtherData);
        log("ok");
    }else{
        __Dictionary* s_localUserData = __Dictionary::create();
        m_lightTypeData.selectBG = 2;
        m_lightTypeData.lastOpenSend = 'a';
        m_lightTypeData.lastCloseSend = 'A';
        m_lightTypeData.lightType = false;
        m_lightTypeData.sendOtherData = '#';
        __String* selectBG = __String::createWithFormat("%d",m_lightTypeData.selectBG);
        __String* lastOpenSend = __String::createWithFormat("%d",m_lightTypeData.lastOpenSend);
        __String* lastCloseSend = __String::createWithFormat("%d",m_lightTypeData.lastCloseSend);
        __String* lightType = __String::createWithFormat("%d",m_lightTypeData.lightType);
        __String* sendOtherData = __String::createWithFormat("%d",m_lightTypeData.sendOtherData);
        s_localUserData->setObject(selectBG, "selectBG");
        s_localUserData->setObject(lastOpenSend, "lastOpenSend");
        s_localUserData->setObject(lastCloseSend, "lastCloseSend");
        s_localUserData->setObject(lightType, "lightType");
        s_localUserData->setObject(sendOtherData, "sendOtherData");
        s_localUserData->writeToFile(filepath.c_str());
    }
}
void LocalFIleDataManger::saveUserData()
{
    __Dictionary* s_localUserData = __Dictionary::create();
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string filepath = writablePath + kDataSavePath;
    __String* selectBG = __String::createWithFormat("%d",m_lightTypeData.selectBG);
    __String* lastOpenSend = __String::createWithFormat("%d",m_lightTypeData.lastOpenSend);
    __String* lastCloseSend = __String::createWithFormat("%d",m_lightTypeData.lastCloseSend);
    __String* lightType = __String::createWithFormat("%d",m_lightTypeData.lightType);
    __String* sendOtherData = __String::createWithFormat("%d",m_lightTypeData.sendOtherData);
    s_localUserData->setObject(selectBG, "selectBG");
    s_localUserData->setObject(lastOpenSend, "lastOpenSend");
    s_localUserData->setObject(lastCloseSend, "lastCloseSend");
    s_localUserData->setObject(lightType, "lightType");
    s_localUserData->setObject(sendOtherData, "sendOtherData");
    s_localUserData->writeToFile(filepath.c_str());
}