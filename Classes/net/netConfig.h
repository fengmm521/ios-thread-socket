//
//  netConfig.h
//  sockettest
//  使用结构体来进行游戏属性存储会比较方便快捷
//
//  Created by 俊盟科技1 on 11/13/14.
//
//

#ifndef sockettest_netConfig_h
#define sockettest_netConfig_h

using namespace std;

typedef unsigned long  uLong; /* 32 bits or more */
//socket服务器
#define MAX_BUFF    256   //客户端256B缓存
#define kServerIP "192.168.1.11"
//#define kServerIP "192.168.1.122"
#define kServerPort 8001


// TCP 数据包头 16 bit
// 网关与客户的交互
typedef struct tag_msghead_t {
    unsigned short 	len;	// 数据体长度
    unsigned short  cmd;	// 协议号
    unsigned short 	eno;	// 错误码
    unsigned char 	enc;	// 加密
    unsigned char 	com;	// 压缩
    unsigned int 	tea;	// 会话
    unsigned int 	idx;	// 包索引
}T_MSGHEAD_T, *LP_MSGHEAD_T;

//服务器返回错误码
enum ERROR_CODE {
    ERROR_FAILURE = -1,
    ERROR_SUCCESS = 0,
    ERROR_NET_PUSH = 10,	// 推送消息标识
};

#endif
