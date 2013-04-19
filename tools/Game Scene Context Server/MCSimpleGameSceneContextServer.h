//
//  MCSimpleGameSceneContextServer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__File__
#define __Military_Confrontation__File__

#if MC_DEBUG_SERVER == 1

#include <pthread.h>
#include "JsonBox.h"

extern const int kMCListeningPort;
extern const int kMCNotifyingPort;

#define __MCBuildLog(format, ...) CCString::createWithFormat(format, ##__VA_ARGS__)->getCString()

#define MCNetLog(format, ...)  MCSimpleGameSceneContextServer::defaultSimpleGameSceneContextServer()->sendData(CCString::createWithFormat("%s(%d)::%s\n", __FILE__, __LINE__, __MCBuildLog(format, ##__VA_ARGS__))->getCString())

#define MCNetLOG(format, ...) \
    do { \
        if (__log__) \
            MCNetLog(format, ##__VA_ARGS__); \
        else \
            CCLog(format, ##__VA_ARGS__); \
    } while (0)

/**
 * 以JSON的格式发送场景中所有对象的数据给请求方，包括场景的信息。
 */
class MCSimpleGameSceneContextServer {
public:
    MCSimpleGameSceneContextServer()
    : isRunning_(false)
    , serverSockectFD_(-1)
    , logSockectFD_(-1)
    , port_(-1) {}
    
    static MCSimpleGameSceneContextServer *defaultSimpleGameSceneContextServer();
    
    inline bool isRunning() {
        return isRunning_;
    }
    
    inline int serverSockectFD() {
        return serverSockectFD_;
    }
    
    inline int port() {
        return port_;
    }
    
    inline void setPort(int var) {
        port_ = var;
    }
    
    void runloop();
    
    void notifySceneDidChange();
    
    void sendData(const char *data);
    
    void getGameSceneContextData(JsonBox::Object &data);
    
private:
    bool isRunning_;
    
    int serverSockectFD_;
    int logSockectFD_;
    int port_;
    
    pthread_t pid;
};

#endif

#endif /* defined(__Military_Confrontation__File__) */
