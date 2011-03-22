//
//  SoraVlcMoviePlayer.h
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_VLC_MOVIE_PLAYER_H_
#define SORA_VLC_MOVIE_PLAYER_H_    

#include "SoraCore.h"
#include "SoraMoviePlayer.h"
#include "vlc/vlc.h"

namespace sora {
    
    class SoraVlcMoviePlayer: public SoraMoviePlayer {
    public:
        typedef struct tagMP_CTX {
            void* pixels;
            uint32 frameCount;
            
            uint32 videoWidth;
            uint32 videoHeight;
            
            bool bChanged;
            
            tagMP_CTX(): bChanged(false), frameCount(0) {}
        } MP_CTX;
        
        SoraVlcMoviePlayer();
        ~SoraVlcMoviePlayer();
        
        void openMedia(const SoraWString& filePath, uint32 width, uint32 height, const SoraString& dis="RGBA");
        void play();
        void stop();
        void pause();
        
        void setVolume(int32 vol);
        int32 getVolume() const;
        
        void setMute(bool flag);
        bool getMute();

        void setTime(s_int64 newtime);
        s_int64 getTime() const;

        
        s_int64 getLength() const;
        float getFPS() const;
        
        uint32 getWidth() const { return libvlc_video_get_width(mp); }
        uint32 getHeight() const { return libvlc_video_get_height(mp); }
        uint32 getFrameCount() const { return frameData.frameCount; }        
        bool frameChanged() const { return frameData.bChanged; }
        
        void* getPixelData() const { return frameData.pixels; }
        
    private:
        libvlc_instance_t* vlcInstance;
        libvlc_media_player_t* mp;
        libvlc_media_t* media;
        libvlc_event_manager_t* evtManager;
       
        MP_CTX frameData;
    };
    
} // namesapce sora

#endif