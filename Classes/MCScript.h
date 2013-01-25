//
//  MCScript.h
//  Military Confrontation
//
//  Created by 江宇英 on 12-12-24.
//  Copyright (c) 2012年 DF. All rights reserved.
//

#ifndef __Military_Confrontation__MCScript__
#define __Military_Confrontation__MCScript__

typedef void *CCPoint;

#include <iostream>

enum {
    MCUnknownEventTrigger  = 0x00,   /* 未知触发器 */
    MCLocationEventTrigger = 0x01,   /* 位置触发器 */
    MCFlagEventTrigger     = 0x02,   /* 标志触发器 */
    MCCommonEventTrigger   = MCLocationEventTrigger & MCFlagEventTrigger
};
typedef unsigned int MCEventTriggerType;

class MCEventTriggerDelegate {
public:
    virtual const char *identifier()=0;           /* 触发器标识符 */
    virtual MCEventTriggerType eventTrigger()=0;    /* 触发器类型 */
    
    virtual CCPoint location();     /* 触发位置 */
    virtual unsigned int flags();   /* 触发标志 */
};


#endif /* defined(__Military_Confrontation__MCScript__) */
