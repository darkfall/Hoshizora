//
//  SoraMoviePlayer.h
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_MOVIE_PLAYER_H_
#define SORA_MOVIE_PLAYER_H_


#include "SoraPlatform.h"
#include "SoraObject.h"

namespace sora {

    class SoraMoviePlayer {
    public:
        virtual ~SoraMoviePlayer();
        
        virtual void openMedia(const SoraWString& filePath) {}
        virtual void play() {}
        virtual void stop() {}
        virtual void pause() {}
        
        virtual void setVolume(int32 vol) {}
        virtual int32 getVolume() {}
        
        virtual void setMute(bool flag) {}
        virtual bool getMute() {}
        
        virtual s_int64 getLength() {}
        virtual s_int64 getTime() {}
    };
} // namespace sora

#endif