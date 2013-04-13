//
//  MCTaskAccessor.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <string.h>
#include <vector>
using namespace std;

#include "MCTaskAccessor.h"
#include "MCBase64.h"

const char *kMCTasksKey = "dGFza3M"; /* tasks的BASE64编码没有最后的= */

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

MCTaskAccessor::MCTaskAccessor()
{
    tasks_ = CCDictionary::create();
    tasks_->retain();
}

MCTaskAccessor::~MCTaskAccessor()
{
    CC_SAFE_RELEASE(tasks_);
}

/**
 * 从数据包加载任务
 */
bool
MCTaskAccessor::loadTasks(const char *aFilePath)
{
    bool result = false;
    
    do {
        JsonBox::Value in;
        JsonBox::Object o;
        JsonBox::Object tasksDict;
        JsonBox::Object::iterator oIter;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCString* pstrFileContent = CCString::createWithContentsOfFile(aFilePath);
        if (pstrFileContent) {
            in.loadFromString(pstrFileContent->getCString());
        }
#else
        in.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(aFilePath).c_str());
#endif
        
        CC_BREAK_IF(! in.isObject());
        
        tasksDict = in.getObject();
        for (oIter = tasksDict.begin(); oIter != tasksDict.end(); ++oIter) {
            std::string t_id = oIter->first;
            const char *s_t_id = t_id.c_str();
            mc_object_id_t o_id = {
                s_t_id[0],
                s_t_id[1],
                s_t_id[2],
                s_t_id[3]
            };
            MCTask *task = new MCTask;
            task->autorelease();
            if (task) {
                task->setID(o_id);
                o = oIter->second.getObject();
                task->loadTaskContent(o);
                /* 默认为未完成状态，然后会在加载存档数据的时候更新为正确状态 */
                task->setTaskStatus(MCTaskUncompleted);
                task->proto_ = NULL;
                tasks_->setObject(task, MCObjectIdToDickKey(o_id));
            }
        }
        
        result = true;
    } while (0);
    
    return result;
}

/**
 * 储存数据
 */
void
MCTaskAccessor::saveData()
{
    string data;
    
    CCDictElement *elem;
    MCTask *task;
    mc_byte_t b_o_id[5] = {0};
    const char *c_s_o_id = (const char *) b_o_id;
    CCDICT_FOREACH(tasks_, elem) {
        task = (MCTask *) elem->getObject();
        mc_object_id_t o_id = task->getID();
        b_o_id[0] = o_id.class_;
        b_o_id[1] = o_id.sub_class_;
        b_o_id[2] = o_id.index_;
        b_o_id[3] = o_id.sub_index_;
        data.append(c_s_o_id);
        data.append("-");
        string status;
        status.assign(1, task->getTaskStatus() + '0');
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
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCTasksKey, output);
    delete []output;
}

/**
 * 从存档读取数据
 */
void
MCTaskAccessor::loadData()
{
    string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCTasksKey, "");
    if (data.size() > 0) {
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
            MCTaskStatus status = atoi(count.c_str());
            MCTask *task = (MCTask *) tasks_->objectForKey(MCObjectIdToDickKey(o_id));
            if (task) {
                task->setTaskStatus(status);
            }
        }
    }
}

MCTask *
MCTaskAccessor::taskWithObjectId(mc_object_id_t anObjectId)
{
    return (MCTask *) tasks_->objectForKey(MCObjectIdToDickKey(anObjectId));
}
