//
//  MCTestRoleBaseInfo.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestRoleBaseInfo__
#define __Military_Confrontation__MCTestRoleBaseInfo__

#include "MCTestLayer.h"
#include "MCRoleBaseInfo.h"
#include "MCRoleManager.h"

class MCTestRoleBaseInfo : public MCTestLayer {
public:
    
    static CCScene *scene() {
        CCScene *scene = new CCScene;
        
        scene->init();
        MCTestRoleBaseInfo *l = new MCTestRoleBaseInfo;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        MCRoleBaseInfoGroup *group = MCRoleBaseInfoGroup::create();
        MCRoleBaseInfo *info;
        MCRoleManager *rm = MCRoleManager::sharedRoleManager();
        mc_object_id_t oid = {
            'N', '0', '0', '1'
        };
        for (int i = 0; i< 5; ++i) {
            MCRole *role = rm->roleForObjectId(oid);
            role->setHP(100-i);
            role->setMaxHP(100+i);
            role->setPP(100-i);
            role->setMaxPP(100+i);
            info = MCRoleBaseInfo::create(role);
            group->addRoleBaseInfo(info);
            oid.sub_index_ += 1;
        }
        
        l->addChild(group);
        
        scene->addChild(l);
        
        return scene;
    }
};


#endif /* defined(__Military_Confrontation__MCTestRoleBaseInfo__) */
