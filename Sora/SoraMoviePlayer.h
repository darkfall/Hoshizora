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

    class SoraMoviePlayer: public SoraPlaybackEventHandler {
    public:
        virtual ~SoraMoviePlayer() {}
        
        /* @param width and height are hack for vlc
         to be solved
         */
        virtual bool openMedia(const SoraWString& filePath, const SoraString& dis="RGBA") { return false; }
        virtual void play() {}
        virtual void stop() {}
        virtual void pause() {}
        
        virtual void setVolume(int32 vol) {}
        virtual int32 getVolume() const { return 0; }
        
        virtual void setMute(bool flag) {}
        virtual bool getMute() { return false; }

        virtual void setTime(s_int64 newtime) {}
        virtual s_int64 getTime() const { return 0; }

        virtual bool isStopped() const { return false; }
        virtual bool isPlaying() const { return false; }
        virtual bool isPaused() const { return false; }
        
        virtual float32 getPlayRate() const { return 0.f; }
        virtual void setPlayRate(float32 rate) {}
        
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