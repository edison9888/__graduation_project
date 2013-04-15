//
//  MCFlagManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <vector>
#include <string.h>
using namespace std;

#include "MCBase64.h"
#include "JsonBox.h"
#include "MCFlagManager.h"
#include "MCGameState.h"

const char *kMCFlagsKey = "ZmxhZ3M"; /* flags的BASE64编码没有最后的= */

static MCFlagManager *__shared_flag_manager = NULL;

const char *kMCFlagsFilepath = "F000.jpkg";

/* 任务状态 */
static const mc_object_id_t kMCTaskFlagId = {'F', '0', '0', '1'};
/* 高级区域封锁 */
static const mc_object_id_t kMCAreaBlockedFlagId = {'F', '0', '0', '2'};
/* 是否在重生点出现 */
static const mc_object_id_t kMCSpawnFlagId = {'F', '0', '0', '3'};

static vector<string>
split(string& str,const char* c)
{
    char *cstr;
    char *p;
    string stdString;
    vector<string> result;
    
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while (p != NULL) {
        result.push_back(p);
        p = strtok(NULL,c);
    }
    
    delete cstr;
    
    return result;
}

MCFlagManager::MCFlagManager()
{
    sourceFlags_ = CCDictionary::create();
    sourceFlags_->retain();
    flags_ = CCDictionary::create();
    flags_->retain();
}

MCFlagManager::~MCFlagManager()
{
    
}

MCFlagManager *
MCFlagManager::sharedFlagManager()
{
    if (__shared_flag_manager == NULL) {
        __shared_flag_manager = new MCFlagManager;
        __shared_flag_manager->loadAllFlags();
    }
    
    return __shared_flag_manager;
}

MCFlag *
MCFlagManager::flagForObjectId(mc_object_id_t anObjectId)
{
    int key = MCObjectIdToDickKey(anObjectId);
    MCFlag *flag;
    
    flag = (MCFlag *) flags_->objectForKey(key);
    if (flag == NULL) {
        flag = (MCFlag *) sourceFlags_->objectForKey(key);
    }
    
    return flag;
}

MCFlagState
MCFlagManager::flagStateForObjectId(mc_object_id_t anObjectId)
{
    MCFlag *flag = flagForObjectId(anObjectId);
    
    if (flag) {
        return flag->getState();
    }
    
    return MCInvalidState;
}

void
MCFlagManager::updateFlag(MCFlag *aFlag)
{
    flags_->setObject(aFlag, MCObjectIdToDickKey(aFlag->getID()));
}

/**
 * load all flags
 */
void
MCFlagManager::loadAllFlags()
{
    JsonBox::Value v;
    JsonBox::Object root;
    JsonBox::Object::iterator flagsIterator;
    JsonBox::Object flagValueRoot;
    const char *c_str_flag_id;
    mc_object_id_t flag_id;
    MCFlag *flag;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCFlagsFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCFlagsFilepath).c_str());
#endif
    
    root = v.getObject();
    
    for (flagsIterator = root.begin(); flagsIterator != root.end(); ++flagsIterator) {
        c_str_flag_id = flagsIterator->first.c_str();
        flagValueRoot = flagsIterator->second.getObject();
        flag_id.class_ = c_str_flag_id[0];
        flag_id.sub_class_ = c_str_flag_id[1];
        flag_id.index_ = c_str_flag_id[2];
        flag_id.sub_index_ = c_str_flag_id[3];
        
        flag = MCFlag::create(flag_id, flagValueRoot);
        sourceFlags_->setObject(flag, MCObjectIdToDickKey(flag_id));
    }
    
    /* 从存档读取数据 */
    string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCFlagsKey, "");
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
        const char *input = data.c_str();
        char  *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        data.assign(output);
        vector<string> result = split(data, ",");
        for (vector<string>::iterator iterator = result.begin(); iterator != result.end(); ++iterator) {
            if (iterator->at(4) != '-') { /* 第4位为separator,若非则为不合法数据 */
                continue;
            }
            mc_object_id_t o_id = {
                iterator->at(0),
                iterator->at(1),
                iterator->at(2),
                iterator->at(3)
            };
            string count = iterator->substr(5);
            MCFlagState state = atoi(count.c_str()) - 1; /* MCInvalidState为-1。。。储存的时候多加了1。。。 */
            MCFlag *flag = (MCFlag *) flags_->objectForKey(MCObjectIdToDickKey(o_id));
            if (flag) {
                flag->setState(state);
            }
        }
    }
    
    /* 初始化特殊flag */
    taskFlag_ = flagForObjectId(kMCTaskFlagId);
    areaBlockedFlag_ = flagForObjectId(kMCAreaBlockedFlagId);
    spawnFlag_ = flagForObjectId(kMCSpawnFlagId);
}

/**
 * 清除数据
 */
void
MCFlagManager::erase()
{
//    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
//    
//    userDefault->setStringForKey(kMCFlagsKey, "");
    
    delete __shared_flag_manager;
    __shared_flag_manager = NULL;
}

/**
 * save all flags
 */
void
MCFlagManager::saveAllFlags()
{
    string data;
    
    CCDictElement *elem;
    MCFlag *flag;
    mc_byte_t b_o_id[5] = {0};
    const char *c_s_o_id = (const char *) b_o_id;
    CCDICT_FOREACH(flags_, elem) {
        flag = (MCFlag *) elem->getObject();
        mc_object_id_t o_id = flag->getID();
        b_o_id[0] = o_id.class_;
        b_o_id[1] = o_id.sub_class_;
        b_o_id[2] = o_id.index_;
        b_o_id[3] = o_id.sub_index_;
        data.append(c_s_o_id);
        data.append("-");
        string status;
        status.assign(1, flag->getState() + '1'); /* MCInvalidState为-1。。。从1开始吧。。。 */
        data.append(status);
        data.append(",");
    }
    if (data.size() > 0) {
        data.erase(data.size() - 1);
    }
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCFlagsKey, output);
    delete []output;
}
