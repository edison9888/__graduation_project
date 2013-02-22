//
//  MCType.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCType_h
#define Military_Confrontation_MCType_h

#include <cstdio>
#include <cstdlib>

typedef unsigned char mc_byte_t;
typedef unsigned int mc_uint_t;
typedef unsigned short int mc_ushort_t;
typedef unsigned int mc_index_t;
typedef unsigned int mc_size_t;
typedef mc_uint_t mc_id_t;
typedef mc_uint_t mc_tag_t;
typedef int mc_dict_key_t;

typedef mc_ushort_t mc_enum_t;
#define MCMakeEnum(offset) (1 << (offset))

#define MCObjectIdToDickKey(o_id) \
(0 | ((mc_dict_key_t)(o_id).class_) * 0x1000000 \
| ((mc_dict_key_t)(o_id).sub_class_) * 0x10000 \
| ((mc_dict_key_t)(o_id).index_) * 0x100 \
| ((mc_dict_key_t)(o_id).sub_index_) \
)

#define MCObjectIdIsEqualsTo(o1, o2) (MCObjectIdToDickKey(o1)) == MCObjectIdToDickKey(o2))

#define MCObjectIdMake(c, sc, i, si)

/*
 * 物件ID结构体
 * 比如：W140 - 武器类 - 剑类 - 巨剑类型 - 默认武器(铜矿)
 */
typedef struct MCObjectID mc_object_id_t;
struct MCObjectID {
    char class_; /* 主类型 */
    char sub_class_; /* 子类型 */
    char index_; /* 第一索引 */
    char sub_index_; /* 第二索引 */
};

const mc_object_id_t NULLObjectId = {0, 0, 0, 0};

/* MCRole 角色属性类型 */
typedef mc_ushort_t mc_hp_t; /* 生命值类型 */
typedef mc_ushort_t mc_pp_t; /* 体力值类型 */
typedef mc_ushort_t mc_load_t; /* 负重类型 */

#endif
