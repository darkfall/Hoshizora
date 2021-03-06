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

#ifdef OS_WIN32
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
#endif

namespace sora {
    
    class SoraVlcMoviePlayer: public SoraMoviePlayer {
		friend class MP_CTX;
		
    public:
        class MP_CTX {
		public:
            void* pixels;
            void* dummy;
            uint32 frameCount;
            
            uint32 videoWidth;
            uint32 videoHeight;
            
            bool bChanged;
            bool bStopped;
            bool bPlaying;
            bool bPaused;
			
            libvlc_media_player_t* mp;
			libvlc_event_manager_t* evtManager;

            SoraVlcMoviePlayer* pPlayer;
            int32 internal;
			
            MP_CTX(): bChanged(false), frameCount(0), pixels(0), dummy(0), bPlaying(false), bPaused(false), bStopped(true) {}
        };
        
        SoraVlcMoviePlayer();
        ~SoraVlcMoviePlayer();
        
        bool openMedia(const SoraWString& filePath, const SoraString& dis="RGBA");
        void play();
        void stop();
        void pause();
        void resume();
                
        void setVolume(int32 vol);
        int32 getVolume() const;
        
        void setMute(bool flag);
        bool getMute();

        void setTime(uint64 newtime);
        uint64 getTime() const;
        
        uint64 getLength() const;
        float getFPS() const;
        
        uint32 getWidth() const;
        uint32 getHeight() const;
        uint32 getFrameCount() const;        
        bool frameChanged() const;
        void setFinish();
        
        void* getPixelData() const;
        
        bool isStopped() const;
        bool isPlaying() const;
        bool isPaused() const;
        
        int32 getAudioTrackNum() const;
        int32 getAudioChannelNum() const;
        
        void toNextFrame();
     
        float32 getPlayRate() const;
        void setPlayRate(float32 rate);
        
		void setMediaInfo(uint32 w, uint32 h);
    private:
        libvlc_instance_t* vlcInstance;
        libvlc_media_player_t* mp;
        libvlc_media_t* media;
        libvlc_event_manager_t* evtManager;
		
		uint32 videoWidth, videoHeight;
       
        MP_CTX frameData;
		std::string displayFormat;
    };
    
} // namesapce sora

#endif