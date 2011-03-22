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

namespace sora {

    class SoraMoviePlayer {
    public:
        virtual ~SoraMoviePlayer() {}
        
        virtual void openMedia(const SoraWString& filePath, uint32 width, uint32 height, const SoraString& dis="RGBA") {}
        virtual void play() {}
        virtual void stop() {}
        virtual void pause() {}
        
        virtual void setVolume(int32 vol) {}
        virtual int32 getVolume() const { return 0; }
        
        virtual void setMute(bool flag) {}
        virtual bool getMute() { return false; }

        virtual void setTime(s_int64 newtime) {}
        virtual s_int64 getTime() const { return 0; }

        
        virtual s_int64 getLength() const { return 0; }
        virtual float getFPS() const { return 0.f; }
        
        virtual uint32 getWidth() const { return 0; }
        virtual uint32 getHeight() const { return 0; }
        
        virtual void* getPixelData() const { return NULL; }
        virtual uint32 getFrameCount() const { return 0; }
        virtual bool frameChanged() const { return false; }
    };
} // namespace sora

#endif