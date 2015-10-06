#ifndef __SocketBuffer_H__
#define __SocketBuffer_H__

#include "cocos2d.h"

using namespace cocos2d;


class SocketBuffer{
private:
    
public:
	~SocketBuffer();
    
    static SocketBuffer* defaultSocketBuffer();
    
    char body[1024*100];
    char *pNow;
    
    bool isLogin;
    
    void writeShort(short val);
    void writeInt(int val);
    void writeString(std::string& val);
    
    short readShort();
    int readInt();
    std::string readString();
    char readChar();
    
    short bufferLen();
    
protected:
    SocketBuffer();
};

#endif



