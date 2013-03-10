//
//  MCScript.h
//  Military Confrontation
//
//  Created by 江宇英 on 12-12-24.
//  Copyright (c) 2012年 DF. All rights reserved.
//

#ifndef __Military_Confrontation__MCScript__
#define __Military_Confrontation__MCScript__

#include "MCObject.h"

#define MCScriptRun(aTriggerScriptPath) MCScriptMaker::createScript(aTriggerScriptPath)->run()

class MCScriptMaker;

class MCScript : public MCObject {
    friend class MCScriptMaker;
private:
    MCScript();
public:
    ~MCScript();
    
    void run();
    
private:
    CCString *scriptPath_;
};

class MCScriptMaker {
public:
    static MCScript *createScript(const char *aScriptPath);
};

#endif /* defined(__Military_Confrontation__MCScript__) */
