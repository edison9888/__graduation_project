//
//  MCBase64.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBase64__
#define __Military_Confrontation__MCBase64__

#include "MCType.h"

/**
 * output必须自己释放
 */
void MCBase64Encode(mc_byte_t *input, mc_size_t in_len, mc_byte_t **output);

/**
 * output必须自己释放
 */
mc_size_t MCBase64Decode(mc_byte_t *input, mc_size_t in_len, mc_byte_t **output);

#endif /* defined(__Military_Confrontation__MCBase64__) */
