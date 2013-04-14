//
//  MCSceneController.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSceneController__
#define __Military_Confrontation__MCSceneController__

#include "MCScene.h"

enum {
    MCPushScene     = 0,
    MCPopScene      = 1,
    MCReplaceScene  = 2
};
typedef mc_enum_t MCChangeSceneMethod;

class MCSceneController {
private:
    MCSceneController();
public:
    ~MCSceneController();
    
    static MCSceneController *sharedSceneController();
    
    /**
     * 提交期待转换的场景信息
     */
//    void pushExpectedScene(MCScene *aNewScene, const char *anEntranceName, MCChangeSceneMethod method = MCReplaceScene);
    
    /**
     * 提交期待转换的场景信息
     */
    void pushExpectedScene(mc_object_id_t anObjectId, const char *anEntranceName, MCChangeSceneMethod method = MCReplaceScene);
    
    /**
     * 请求切换场景
     */
    void requestChangingScene();
    
    /**
     * so-called private method!
     * 不该自己来执行这个方法，转交给lua来运行！
     * 加载场景包
     */
    void __loadScene();
    
    /**
     * so-called private method!
     * 不该自己来执行这个方法，转交给lua来运行！
     * 切换当前场景为aNewScene
     */
    void __changeScene();
    
private:
    MCScene *lastScene_;
    MCScene *currentScene_;
    
    MCScene *expectedScene_;
    CCString *entranceName_;
    MCChangeSceneMethod method_;
    
    CC_SYNTHESIZE_PASS_BY_REF(mc_object_id_t, expectedSceneId_, ExpectedSceneId)
};

#endif /* defined(__Military_Confrontation__MCSceneController__) */
