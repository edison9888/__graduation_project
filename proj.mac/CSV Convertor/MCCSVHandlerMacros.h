//
//  MCCSVHandlerMacros.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCCSVHandlerMacros_h
#define Military_Confrontation_MCCSVHandlerMacros_h

#define MCDefineIndexAndKey(name, index, key) \
static const NSUInteger kMC##name##Index = (index); \
static const NSString   *kMC##name##Key = @""#key"" \

#endif
