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
        *pixels = ctx->pixels;
        ++ctx->frameCount;
    
        return 0;
    }
    
    static void unlock(void* data, void* id, void* const* pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
        ctx->bChanged = true;
        memcpy(ctx->dummy, ctx->pixels, ctx->pPlayer->getWidth()*ctx->pPlayer->getHeight()*4);
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
            case libvlc_MediaPlayerPlaying: 
                ctx->pPlayer->publishEvent(ctx->bPaused?SORAPB_EV_PLAY_RESUMED:SORAPB_EV_PLAY_STARTED);
                ctx->bPlaying = true; 
                break;
            case libvlc_MediaPlayerEndReached: 
                ctx->bPlaying = false; 
                ctx->bStopped = true; 
                ctx->pPlayer->publishEvent(SORAPB_EV_PLAY_ENDED);
                break;
        }
    }
    
    SoraVlcMoviePlayer::SoraVlcMoviePlayer(): vlcInstance(0), mp(0), media(0), evtManager(0) {
        const char* vlc_argv[] = {
            "--plugin-path=./Plugins"
            "--ignore-config",
			"-I", "dummy"
        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
        
        vlcInstance = libvlc_new(vlc_argc, vlc_argv);
        if(!vlcInstance)
            throw SoraException("Error initializing VLCCore");
        
        mp = libvlc_media_player_new(vlcInstance);
        if(!mp)
            throw SoraException("Error creating vlc media player");
        
        evtManager = libvlc_media_player_event_manager(mp);
        if(!evtManager)
            throw SoraException("Error creating event manager for player");
    }
    
    SoraVlcMoviePlayer::~SoraVlcMoviePlayer() {
        libvlc_media_player_release(mp);
        libvlc_release(vlcInstance);
    }
    
    void SoraVlcMoviePlayer::openMedia(const SoraWString& filePath, uint32 width, uint32 height, const SoraString& dis) {
        if(media) { libvlc_media_release(media); }
        media = libvlc_media_new_path(vlcInstance, ws2s(filePath).c_str());
        libvlc_media_player_set_media(mp, media);
        libvlc_media_release(media);
        
        libvlc_event_attach(evtManager, libvlc_MediaPlayerPaused, eventHandle, &frameData);
        libvlc_event_attach(evtManager, libvlc_MediaPlayerPlaying, eventHandle, &frameData);
        libvlc_event_attach(evtManager, libvlc_MediaPlayerStopped, eventHandle, &frameData);
        libvlc_event_attach(evtManager, libvlc_MediaPlayerEndReached, eventHandle, &frameData);
        libvlc_event_attach(evtManager, libvlc_MediaPlayerPositionChanged, eventHandle, &frameData);

		setMediaInfo(width, height);

        libvlc_video_set_format(mp, dis.c_str(), frameData.videoWidth, frameData.videoHeight, frameData.videoWidth*4);
		SORA->logf("w: %d, h: %d", getWidth(), getHeight());
    }
    
    void SoraVlcMoviePlayer::setMediaInfo(uint32 w, uint32 h) {
        if(frameData.pixels) 
            free(frameData.pixels);
        if(frameData.dummy)
            free(frameData.dummy);
        
        frameData.videoWidth = w;
        frameData.videoHeight = h;
        frameData.pixels = malloc(frameData.videoWidth * frameData.videoHeight * 4);
        frameData.dummy = malloc(frameData.videoWidth * frameData.videoHeight * 4);
        
        frameData.pPlayer = this;
        
        libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);
    }
    
    void SoraVlcMoviePlayer::play() {
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
    
    s_int64 SoraVlcMoviePlayer::getLength() const {
        return libvlc_media_player_get_length(mp);
    }
    
    void SoraVlcMoviePlayer::setTime(s_int64 newtime) {
        if(!isPlaying()) return;
        
        if(newtime < 0) newtime = 0;
        if(newtime > getLength()) newtime = getLength();
        
        libvlc_media_player_set_time(mp, (libvlc_time_t)newtime);
    }
    
    s_int64 SoraVlcMoviePlayer::getTime() const {
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
    
    void SoraVlcMoviePlayer::bindSprite(SoraSprite* pSprite) {
        frameData.pSpr = pSprite;
    }
} // namespace sora