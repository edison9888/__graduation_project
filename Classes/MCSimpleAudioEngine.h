//
//  MCSimpleAudioEngine.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSimpleAudioEngine__
#define __Military_Confrontation__MCSimpleAudioEngine__

#include <string>

class MCSimpleAudioEngine {
private:
    MCSimpleAudioEngine();
public:
    
    static MCSimpleAudioEngine *sharedSimpleAudioEngine();
    
    void setExpectedMusic(const char *aMusicFilepath);
    
    void playMusic();
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    
    void playEffect(const char *anEffectFilepath);
    
private:
    std::string lastPlayingMusicFilepath_;
    std::string expectedMusicFilepath_;
};

#endif /* defined(__Military_Confrontation__MCSimpleAudioEngine__) */
