

#include "SocketBuffer.h"
#include<netinet/in.h>
//#include "LoginDataCenter.h"

SocketBuffer::SocketBuffer(){
	memset(body, 0, 1024*100);
    pNow = body+sizeof(uint32_t);
}

SocketBuffer::~SocketBuffer(){
	
}

SocketBuffer* SocketBuffer::defaultSocketBuffer() {
    
    SocketBuffer *buffer = new SocketBuffer();
    buffer->isLogin = false;
    return buffer;
}

void SocketBuffer::writeShort(short val) {
//    if (val == LoginDataCenter::USER_SOCKET_LOGIN) {
//        isLogin = true;
//    }
    val = htons(val);
    memcpy(pNow, &val, sizeof(short));
    pNow += sizeof(short);
}

void SocketBuffer::writeInt(int val) {
    val = htonl(val);
    memcpy(pNow, &val, sizeof(int));
    pNow += sizeof(int);
}

void SocketBuffer::writeString(std::string& val) {
    if (val.length() <= 0) {
        return;
    }
    
    short len = val.size();
    short htons_len = htons(len);

    memcpy(pNow, &htons_len, sizeof(short));
    pNow += 2;
    
    memcpy(pNow, val.c_str(), len);
    pNow += len;
}


short SocketBuffer::readShort() {
    short ret = *(short*)pNow;
    pNow += sizeof(short);
    return ntohs(ret);
}

int SocketBuffer::readInt() {
    int ret = *(int*)pNow;
    pNow += sizeof(int);
    return ntohl(ret);
}

std::string SocketBuffer::readString() {
    short len = readShort();
    
    char *val = (char*)malloc(len+1);
    memset(val, 0, len+1);
    memcpy(val, pNow, len);
    
    std::string str = std::string(val);
    
    free(val);

    pNow += len;
    return str;
}

char SocketBuffer::readChar(){
    char c = *pNow;
    pNow += sizeof(char);
    return c;
}

short SocketBuffer::bufferLen() {
    return pNow - body;
}