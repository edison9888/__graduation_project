//
//  MCDungeonMaster.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCDungeonMaster.h"
#include "MCBase64.h"

const char *kMCSpawnPointKey = "c3Bhd24tcG9pbnQ"; /* spawn-point的BASE64编码没有最后的= */
const char *kMCSpawnPointDefaultValue = "U1AwMQ=="; /* SP01的BASE64编码 */

static MCDungeonMaster *__shared_dungeon_master = NULL;

MCDungeonMaster *
MCDungeonMaster::sharedDungeonMaster()
{
    if (__shared_dungeon_master == NULL) {
        __shared_dungeon_master = new MCDungeonMaster;
        __shared_dungeon_master->loadSpawnPoint();
    }
    
    return __shared_dungeon_master;
}

void
MCDungeonMaster::saveSpawnPoint()
{
    char c_str_sp_id[5] = {0};
    c_str_sp_id[0] = spawnPointID_.class_;
    c_str_sp_id[1] = spawnPointID_.sub_class_;
    c_str_sp_id[2] = spawnPointID_.index_;
    c_str_sp_id[3] = spawnPointID_.sub_index_;
    const char *input = c_str_sp_id;
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCSpawnPointKey, output);
    delete []output;
}

void
MCDungeonMaster::loadSpawnPoint()
{
    std::string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCSpawnPointKey, kMCSpawnPointDefaultValue);
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        data.assign(output);
        const char *c_str_o_id = data.c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        spawnPointID_ = o_id;
    }
}
