//
//  MCScript.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 12-12-24.
//  Copyright (c) 2012年 DF. All rights reserved.
//

#include "MCScript.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"

MCScript::MCScript()
{
}

MCScript::~MCScript()
{
    CC_SAFE_DELETE(scriptPath_);
}

void
MCScript::run()
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(scriptPath_->getCString());
    if (pstrFileContent)
    {
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(scriptPath_->getCString());
    pEngine->addSearchPath(path.substr(0, path.find_last_of("/")).c_str());
    pEngine->executeScriptFile(path.c_str());
#endif
}

MCScript *
MCScriptMaker::createScript(const char *aScriptPath)
{
    MCScript *trigger = new MCScript;
    CCString *scriptPath;
    
    trigger->autorelease();
    scriptPath = new CCString;
    scriptPath->initWithFormat("%s", aScriptPath);
    trigger->scriptPath_ = scriptPath;
    
    return trigger;
}
