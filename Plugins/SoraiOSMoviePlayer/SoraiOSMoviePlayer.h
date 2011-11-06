//
//  SoraiOSMoviePlayer.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/10/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraiOSMoviePlayer_h
#define Sora_iPhone_SoraiOSMoviePlayer_h

#include "SoraPlatform.h"
#include "SoraMoviePlayer.h"

#ifdef OS_IOS

namespace sora {
    
    /**
     * iOS MoviePlayer implemention
     * Note that movies on iOS would start immediately
     * And not all movie formats are supported
     **/
    
    class SoraiOSMoviePlayer: public SoraMoviePlayer {
    public:
        SoraiOSMoviePlayer();
        ~SoraiOSMoviePlayer();
        
        bool openMedia(const SoraWString& filePath, const SoraString& dis="RGBA");
        void play();
        void stop();
        void pause();
        
        void setVolume(int32 vol);
        int32 getVolume() const;
        
        void setMute(bool flag) ;
        bool getMute() ;
        
        void setTime(uint64 newtime);
        uint64 getTime() const;
        
        bool isStopped() const ;
        bool isPlaying() const;
        bool isPaused() const ;
        
        float getPlayRate() const ;
        void setPlayRate(float rate);        
        uint64 getLength() const ;
        float getFPS() const ;
        
        uint32 getWidth() const ;
        uint32 getHeight() const ;
        
        void* getPixelData() const ;
        uint32 getFrameCount() const ;
        bool frameChanged() const ;
        void setFinish();
    };
}

#endif // OS_IOS

#endif
