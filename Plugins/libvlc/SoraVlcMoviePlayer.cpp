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
    
        ctx->bChanged = false;
        return 0;
    }
    
    static void unlock(void* data, void* id, void* const* pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;

        ctx->bChanged = true;
    }
    
    static void display(void* data, void* id) {
        (void) data;
    }
    
    SoraVlcMoviePlayer::SoraVlcMoviePlayer(): vlcInstance(0), mp(0), media(0), evtManager(0) {
        const char* vlc_argv[] = {
            "--plugin-path=./Plugins"
            "--ignore-config",
            "--vout", "vmem",
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
        if(media) {
            libvlc_media_release(media);
        }
        if(frameData.pixels) 
            free(frameData.pixels);
        
        media = libvlc_media_new_path(vlcInstance, ws2s(filePath).c_str());
        libvlc_media_player_set_media(mp, media);
                
        frameData.videoWidth = width;
        frameData.videoHeight = height;

        frameData.pixels = malloc(frameData.videoWidth * frameData.videoHeight * 4);
                
        libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);
        libvlc_video_set_format(mp, dis.c_str(), frameData.videoWidth, frameData.videoHeight, frameData.videoWidth*4);
    }
    
    void SoraVlcMoviePlayer::play() {
        libvlc_media_player_play(mp);
    }
    
    void SoraVlcMoviePlayer::stop() {
        libvlc_media_player_stop(mp);
    }
    
    void SoraVlcMoviePlayer::pause() {
        libvlc_media_player_stop(mp);
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
        libvlc_media_player_set_time(mp, (libvlc_time_t)newtime);
    }
    
    s_int64 SoraVlcMoviePlayer::getTime() const {
        return libvlc_media_player_get_time(mp);
    }
    
    float SoraVlcMoviePlayer::getFPS() const {
        return libvlc_media_player_get_fps(mp);
    }

    
} // namespace sora