//
//  MCSceneList.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneList.h"

#include "JsonBox.h"
#include "MCJSONModifier.h"

static MCSceneList *__shared_scene_list = NULL;

MCSceneList::MCSceneList()
{
    scenes_ = CCArray::create();
    scenes_->retain();
}

MCSceneList::~MCSceneList()
{
    CC_SAFE_RELEASE(scenes_);
}

MCSceneList *
MCSceneList::sharedSceneList()
{
    if (__shared_scene_list == NULL) {
        __shared_scene_list = new MCSceneList;
        if (__shared_scene_list) {
            __shared_scene_list->autorelease();
        }
    }
    
    return __shared_scene_list;
}

void
MCSceneList::loadSceneListFile(const char *aFilepath)
{
    JsonBox::Value in;
    JsonBox::Value v;
    JsonBox::Object o;
    JsonBox::Array packages;
    JsonBox::Array::iterator iter;
    
    MCJSONModifier::setJSONFileVisible(aFilepath, MCJSONVisible);
    in.loadFromFile(aFilepath);
    o = in.getObject();
    MCJSONModifier::setJSONFileVisible(aFilepath, MCJSONInvisible);
    
    packages = o["scenes"].getArray();
    for (iter = packages.begin(); iter != packages.end(); ++iter) {
        v = *iter;
        /* load package */
        CCLog("%s", v.getString().c_str());
    }
}

MCScenePackage *
MCSceneList::packageWithNamed(const char *aName)
{
    CCObject *object;
    MCScenePackage *package;
    
    CCARRAY_FOREACH(scenes_, object) {
        package = (MCScenePackage *) object;
        if (strcmp(package->getName()->getCString(), aName) == 0) {
            return package;
        }
    }
    
    return NULL;
}

MCScenePackage *
MCSceneList::packageAtIndex(mc_index_t anIndex)
{
    return (MCScenePackage *) scenes_->objectAtIndex(anIndex);
}
