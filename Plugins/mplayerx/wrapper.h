//
//  wrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/22/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef MP_WRAPPER_H
#define MP_WRAPPER_H


#include "SoraCore.h"

namespace sora {
    
    class mpMoviePlayer {
    public:
        mpMoviePlayer();
        void playMovieAtPath(const SoraString& path);
        
    private:
    };
    
} // namespace sora

#endif