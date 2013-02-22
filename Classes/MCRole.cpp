//
//  MCRole.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRole.h"

const char *__default_response = "....";

/* 下一句话。如果没有则返回NULL；如果本来就没话说则返回默认对白 */
const char *
MCRole::nextSentence()
{
    const char *sentence = NULL;
    if (dialogues_ && nextSentenceIndex_ < dialogues_->count()) {
        sentence = ((CCString *) dialogues_->objectAtIndex(nextSentenceIndex_))->getCString();
    } else if (defaultDialogue_ != NULL) {
        sentence = defaultDialogue_->getCString();
    } else {
        sentence = __default_response;
    }
    
    return sentence;
}
