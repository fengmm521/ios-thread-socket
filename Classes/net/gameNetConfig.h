//
//  gameNetConfig.h
//  sockettest
//  游戏相关的网络接口
//
//  Created by 俊盟科技1 on 11/14/14.
//
//
#include "cocos2d.h"
#ifndef sockettest_gameNetConfig_h
#define sockettest_gameNetConfig_h

using namespace std;

//示例宏定义，不使用时可删除
#define kDemoDef "demoDef"


//1.注册帐号
//用户注册游戏
struct CreateUser
{
    std::string userName;
    std::string userPw;
    std::string userEmail;
    std::string userPhone;
    std::string deviceID;
};
typedef struct CreateUser CreateUserNetST;

//2.登陆游戏服务器
struct loginGame
{
    std::string userName;
    std::string userEmail;
    std::string userPhone;
    std::string deviceID;
    std::string userPw;
};
typedef struct loginGame LoginGameNetST;


//999.游戏服务器测试
struct ServerTest
{
    std::string strTest;
    int intTest;
    float floatTest;
    bool boolTest;
};
typedef struct ServerTest ServerTestNetST;

////示例枚举，不使用时可删除
//enum DemoENUM
//{
//    e_a = 0,
//    e_b
//};
//
//typedef enum DemoENUM DemoE;



#endif
