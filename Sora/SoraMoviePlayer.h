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
#include "SoraPlaybackEvent.h"

namespace sora {

    class SORA_API SoraMoviePlayer: public SoraPlaybackEventHandler {
    public:
        virtual bool openMedia(const wchar_t* filePath, const char* dis="RGBA") = 0;
        virtual void play() = 0;
        virtual void stop() = 0;
        virtual void pause() = 0;
        
        virtual void setVolume(int32 vol) = 0;
        virtual int32 getVolume() const = 0;
        
        virtual void setMute(bool flag) = 0;
        virtual bool getMute() = 0;

        virtual void setTime(uint64 newtime) = 0;
        virtual uint64 getTime() const = 0;

        virtual bool isStopped() const = 0;
        virtual bool isPlaying() const = 0;
        virtual bool isPaused() const = 0;
        
        virtual float32 getPlayRate() const = 0;
        virtual void setPlayRate(float32 rate) = 0;
        
        virtual uint64 getLength() const = 0;
        virtual float getFPS() const = 0;
        
        virtual uint32 getWidth() const = 0;
        virtual uint32 getHeight() const = 0;
        
        virtual void* getPixelData() const = 0;
        virtual uint32 getFrameCount() const = 0;
        virtual bool frameChanged() const = 0;
        virtual void setFinish() = 0;
    
    };
} // namespace sora

#endif