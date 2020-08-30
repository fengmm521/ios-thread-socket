//
//  UserDataConfig.h
//  light
//  使用结构体来进行游戏属性存储会比较方便快捷
//
//  Created by woodcol on 15/2/24.
//
//

#ifndef light_UserDataConfig_h
#define light_UserDataConfig_h

using namespace std;

//示例宏定义，不使用时可删除
#define kDataSavePath "data.plist"


//示例结构体,不使用时可删除
struct LightType_ST
{
    int selectBG; //1哦竖屏，2为横屏
    
    char lastOpenSend;  //上次开灯时发送的数据字符
    char lastCloseSend; //上次关灯时发送的数据字符
    char sendOtherData; //上次复位或查询系统情况发送字符
    bool lightType;   //当前灯是开是关
};

typedef struct LightType_ST LightTypeST;

//示例枚举，不使用时可删除
enum DemoENUM
{
    e_a = 0,
    e_b
};

typedef enum DemoENUM DemoE;



#endif
