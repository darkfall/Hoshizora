//
//  SoraConstantStrings.h
//  Sora
//
//  Created by Griffin Bu on 4/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_CONSTANT_STRINGS_H_
#define SORA_CONSTANT_STRINGS_H_

#include "stringId.h"

namespace sora {
    
    // str2id("xxx") constants
    static const stringId STR_ID_PLAY_STARTED      = 18446744072169310163u;
    static const stringId STR_ID_PLAY_ENDED        = 18446744071516978624u;
    static const stringId STR_ID_PLAY_PAUSED       = 18446744070414540033u;
    static const stringId STR_ID_PLAY_RESUMED      = 18446744071005043197u;
    static const stringId STR_ID_PLAY_STOPPED      = 18446744072468109797u;
    
    static void initConstantStrings() {
        SORA_STR_MANAGER->addString("PlayStarted", STR_ID_PLAY_STARTED);
        SORA_STR_MANAGER->addString("PlayEnded",   STR_ID_PLAY_ENDED);
        SORA_STR_MANAGER->addString("PlayPaused",  STR_ID_PLAY_PAUSED);
        SORA_STR_MANAGER->addString("PlayResumed", STR_ID_PLAY_RESUMED);
        SORA_STR_MANAGER->addString("PlayStopped", STR_ID_PLAY_STOPPED);
    }
    
} // namespace sora

#endif