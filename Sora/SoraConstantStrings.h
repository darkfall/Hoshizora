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
	/*
	long　num=100L;
	long　long　num=100LL;
	unsigned　long=100UL;
	unsigned　long　long　num=100ULL;
	*/
    
    // str2id("xxx") constants
    static const ulong64 STR_ID_PLAY_STARTED      = 18446744072169310163UL;
    static const ulong64 STR_ID_PLAY_ENDED        = 18446744071516978624UL;
    static const ulong64 STR_ID_PLAY_PAUSED       = 18446744070414540033UL;
    static const ulong64 STR_ID_PLAY_RESUMED      = 18446744071005043197UL;
    static const ulong64 STR_ID_PLAY_STOPPED      = 18446744072468109797UL;
    
    static void initConstantStrings() {
        SORA_STR_MANAGER->addString(L"PlayStarted", STR_ID_PLAY_STARTED);
        SORA_STR_MANAGER->addString(L"PlayEnded",   STR_ID_PLAY_ENDED);
        SORA_STR_MANAGER->addString(L"PlayPaused",  STR_ID_PLAY_PAUSED);
        SORA_STR_MANAGER->addString(L"PlayResumed", STR_ID_PLAY_RESUMED);
        SORA_STR_MANAGER->addString(L"PlayStopped", STR_ID_PLAY_STOPPED);
    }
    
} // namespace sora

#endif