//
//  wrapper.mm
//  Sora
//
//  Created by Griffin Bu on 3/22/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "wrapper.h"
#import "CoreController.h"
#import "PlayerController.h"

namespace sora {
    static PlayerController* pcontroller;
    static CoreController* controller;
    
    mpMoviePlayer::mpMoviePlayer() {
        controller = [[CoreController alloc] init];
        pcontroller = [[PlayerController alloc] init];
    }
    
    void mpMoviePlayer::playMovieAtPath(const SoraString& path) {
        NSString* mediaPath = [[NSString alloc] initWithUTF8String:path.c_str()];
       // [controller playMedia: mediaPath];
        [pcontroller playMedia: mediaPath];
        [mediaPath release];
    }
    
}