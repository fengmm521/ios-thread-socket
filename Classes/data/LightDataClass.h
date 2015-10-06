//
//  LightDataClass.h
//  light
//  这个是创建一些基本的cocos2d类时会用到
//  Created by woodcol on 15/2/24.
//
//

#ifndef __light__LightDataClass__
#define __light__LightDataClass__

#include "cocos2d.h"
#include "UserDataConfig.h"

class LightDataClass:public cocos2d::Ref
{
protected:
    
    virtual ~LightDataClass();
public:
    LightDataClass();
    virtual bool init();
    static LightDataClass* create();
    LightType m_lightTypeData;
};
#endif /* defined(__light__LightDataClass__) */
