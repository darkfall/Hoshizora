//
//  SoraVlcMoviePlayer.cpp
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraVlcMoviePlayer.h"
#include "SoraException.h"

#ifndef OS_OSX
#include <memory.h>
#endif

namespace sora {
    
    static void* lock(void* data, void** pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
		if(ctx->pixels) {
			if(ctx->internal == 2) {
				free(ctx->dummy);
				ctx->dummy = malloc(ctx->videoWidth*ctx->videoHeight*4);
			}
			ctx->internal = 0;
			*pixels = ctx->pixels;
		}
		else {
			ctx->internal = 1;
			*pixels = ctx->dummy;
		}
        ++ctx->frameCount;
    
        return 0;
    }
    
    static void unlock(void* data, void* id, void* const* pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
		if(ctx->pixels && ctx->internal != 1) {
			ctx->bChanged = true;
			memcpy(ctx->dummy, ctx->pixels, ctx->pPlayer->getWidth()*ctx->pPlayer->getHeight()*4);
		}
    }
    
    static void display(void* data, void* id) {
       // SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
        
        (void) data;
    }
    
    static void eventHandle(const libvlc_event_t* ev, void* data) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;

        switch (ev->type) {
            case libvlc_MediaPlayerStopped: 
                ctx->bStopped = true; 
                ctx->bPlaying = false; 
                ctx->pPlayer->publishEvent(SORAPB_EV_PLAY_STOPPED);
                break;
            case libvlc_MediaPlayerPaused: 
                ctx->bPaused = true; 
                ctx->bPlaying = false;
                ctx->pPlayer->publishEvent(SORAPB_EV_PLAY_PAUSED);
                break;
            case libvlc_MediaPlayerPlaying:  {
                ctx->pPlayer->publishEvent(ctx->bPaused?SORAPB_EV_PLAY_RESUMED:SORAPB_EV_PLAY_STARTED);
                ctx->bPlaying = true;
                break;
			}
            case libvlc_MediaPlayerEndReached: 
                ctx->bPlaying = false; 
                ctx->bStopped = true; 
                ctx->pPlayer->publishEvent(SORAPB_EV_PLAY_ENDED);
                break;
			case libvlc_MediaPlayerPositionChanged: {
				uint32 w = 0, h;
				if(libvlc_video_get_size(ctx->mp, 0, &w, &h) == -1) {
					libvlc_media_player_stop(ctx->mp);
					ctx->internal = 3;
					SORA->log("error getting video dimensions, exit");
				}
				ctx->pPlayer->setMediaInfo(w, h);
				ctx->bChanged = true;
				//libvlc_media_player_stop(ctx->mp);
				libvlc_event_detach(ctx->evtManager, libvlc_MediaPlayerPositionChanged, eventHandle, ctx);
				break;
			}
				
        }
    }
    
    SoraVlcMoviePlayer::SoraVlcMoviePlayer(): vlcInstance(0), mp(0), media(0), evtManager(0), videoWidth(0), videoHeight(0) {
        const char* vlc_argv[] = {
            "--plugin-path=./Plugins"
            "--ignore-config",
			"-I", "dummy",
			"--verbose", "2"
        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
        
        vlcInstance = libvlc_new(vlc_argc, vlc_argv);
        if(!vlcInstance)
            throw SoraException("Error initializing VLCCore");
        
		mp = NULL;
        evtManager = NULL;
    }
    
    SoraVlcMoviePlayer::~SoraVlcMoviePlayer() {
		if(mp)
			libvlc_media_player_release(mp);
        libvlc_release(vlcInstance);
    }
    
    bool SoraVlcMoviePlayer::openMedia(const SoraWString& filePath, const SoraString& dis) {
        if(media) { libvlc_media_release(media); }
        media = libvlc_media_new_path(vlcInstance, ws2s(filePath).c_str());
		if(mp) {
			libvlc_media_player_release(mp);
		}
		mp = libvlc_media_player_new_from_media(media);
		if(!mp) {
			SORA->log("Error creating vlc media player");
			return false;
		}
		evtManager = libvlc_media_player_event_manager(mp);
        if(!evtManager) {
			SORA->log("Error creating event manager for the media player, event cannot not work");
			return false;
		}
		else {
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPaused, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPlaying, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerStopped, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerEndReached, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPositionChanged, eventHandle, &frameData);
		}
        libvlc_media_release(media);
		
		displayFormat = dis;

        frameData.videoWidth = 0;
        frameData.videoHeight = 0;
        frameData.pixels = NULL;
        frameData.dummy = malloc(1024*1024*4);
		
		frameData.bPaused = frameData.bPlaying = frameData.bChanged = frameData.bStopped = false;
		
		frameData.mp = mp;
        frameData.pPlayer = this;
		frameData.evtManager = evtManager;
		
		libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);

		// play for first frame to get video infomartion
		libvlc_media_player_play(mp);
		while(!frameData.bChanged);
		frameData.bChanged = false;
		libvlc_media_player_stop(mp);
		
		if(frameData.internal == 3) {
			if(mp) {
				libvlc_media_player_release(mp);
			}
			return false;
		}
		
		return true;
	}
    
    void SoraVlcMoviePlayer::setMediaInfo(uint32 w, uint32 h) {
	    videoWidth = frameData.videoWidth = w;
		videoHeight = frameData.videoHeight = h;
		
		frameData.pixels = malloc(w*h*4);
		frameData.internal = 2;
	}
    
    void SoraVlcMoviePlayer::play() {
		libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);
		libvlc_video_set_format(mp, displayFormat.c_str(), videoWidth, videoHeight, videoWidth*4);

        libvlc_media_player_play(mp);
    }
    
    void SoraVlcMoviePlayer::resume() {
        libvlc_media_player_set_pause(mp, 0);
        frameData.bPlaying = true;
        frameData.bPaused = false;
    }
    
    void SoraVlcMoviePlayer::stop() {
        if(!isPlaying()) return;

        libvlc_media_player_stop(mp);
    }
    
    void SoraVlcMoviePlayer::pause() {
        if(!isPlaying()) return;

        libvlc_media_player_pause(mp);
    }
    
    void SoraVlcMoviePlayer::setVolume(int32 vol) {
        libvlc_audio_set_volume(mp, vol);
    }
    
    int32 SoraVlcMoviePlayer::getVolume() const {
        return libvlc_audio_get_volume(mp);
    }
    
    void SoraVlcMoviePlayer::setMute(bool flag) {
        libvlc_audio_set_mute(mp, flag);
    }
    
    bool SoraVlcMoviePlayer::getMute() {
        return libvlc_audio_get_mute(mp);
    }
    
    uint64 SoraVlcMoviePlayer::getLength() const {
        return libvlc_media_player_get_length(mp);
    }
    
    void SoraVlcMoviePlayer::setTime(uint64 newtime) {
        if(!isPlaying()) return;
        
        if(newtime < 0) newtime = 0;
        if(newtime > getLength()) newtime = getLength();
        
        libvlc_media_player_set_time(mp, (libvlc_time_t)newtime);
    }
    
    uint64 SoraVlcMoviePlayer::getTime() const {
        return libvlc_media_player_get_time(mp);
    }
    
    float SoraVlcMoviePlayer::getFPS() const {
        return libvlc_media_player_get_fps(mp);
    }
    
    int32 SoraVlcMoviePlayer::getAudioTrackNum() const {
        return libvlc_audio_get_track_count(mp);
    }
    
    int32 SoraVlcMoviePlayer::getAudioChannelNum() const {
        return libvlc_audio_get_channel(mp);
    }
    
    void SoraVlcMoviePlayer::toNextFrame() {
        if(!isPlaying()) return;
    
        libvlc_media_player_next_frame(mp);
    }

    float32 SoraVlcMoviePlayer::getPlayRate() const {
        return libvlc_media_player_get_rate(mp);
    }
    
    void SoraVlcMoviePlayer::setPlayRate(float32 rate) {
        libvlc_media_player_set_rate(mp, rate);
    }
} // namespace sora