//
//  LightDataClass.cpp
//  light
//
//  Created by woodcol on 15/2/24.
//
//

#include "LightDataClass.h"

LightDataClass::LightDataClass()
{
  
}

LightDataClass::~LightDataClass()
{
    
}
LightDataClass* LightDataClass::create()
{
    LightDataClass* tmp = new LightDataClass();
    tmp->init();
    return tmp;
}
bool LightDataClass::init()
{
    //////////////////////////////
    // 1. super init first
    bool bRet = false;
    do
    {
        m_lightTypeData.selectBG = 2;
        m_lightTypeData.lastOpenSend = 'a';
        m_lightTypeData.lastCloseSend = 'A';
        m_lightTypeData.lightType = false;
        m_lightTypeData.sendOtherData = '#';
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}

