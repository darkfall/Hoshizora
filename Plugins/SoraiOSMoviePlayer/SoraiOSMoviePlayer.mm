//
//  SoraiOSMoviePlayer.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/10/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSMoviePlayer.h"
#include "SoraiOSMoviePlayerImpl.h"

#include "SoraStringConv.h"

namespace sora {

    static SoraiOSMoviePlayerImpl* g_videoPlayer;
    
    SoraiOSMoviePlayer::SoraiOSMoviePlayer() {
        g_videoPlayer = [SoraiOSMoviePlayerImpl alloc];
    }
    
    SoraiOSMoviePlayer::~SoraiOSMoviePlayer() {
        [g_videoPlayer release];
    }
    
    
    bool SoraiOSMoviePlayer::openMedia(const SoraWString& filePath, const SoraString& dis) {
        std::string spath = sora::ws2s(filePath);
        NSString* nspath = [[NSString alloc] initWithUTF8String: spath.c_str()];
        bool result = [g_videoPlayer playMovie: nspath];
        [nspath release];
        return result;
    }
    
    void SoraiOSMoviePlayer::play() {
        
    }
    
    void SoraiOSMoviePlayer::stop() {
        
    }
    
    void SoraiOSMoviePlayer::pause() {
        
    }
    
    void SoraiOSMoviePlayer::setVolume(int32 vol) {
        
    }
    
    int32 SoraiOSMoviePlayer::getVolume() const {
        return 0;
    }
    
    void SoraiOSMoviePlayer::setMute(bool flag) {
        
    }
    
    bool SoraiOSMoviePlayer::getMute() {
        return false;
    }
    
    void SoraiOSMoviePlayer::setTime(uint64 newtime) {
        
    }
    
    uint64 SoraiOSMoviePlayer::getTime() const {
        return 0;
    }
    
    bool SoraiOSMoviePlayer::isStopped() const {
        return false;
    }
    
    bool SoraiOSMoviePlayer::isPlaying() const {
        return false;
    }
    
    bool SoraiOSMoviePlayer::isPaused() const {
        return false;
    }
    
    float32 SoraiOSMoviePlayer::getPlayRate() const {
        return 0.f;
    }
    
    void SoraiOSMoviePlayer::setPlayRate(float32 rate) {
        
    }
    
    uint64 SoraiOSMoviePlayer::getLength() const {
        return 0;
    }
    
    float SoraiOSMoviePlayer::getFPS() const {
        return 0.f;
    }
    
    uint32 SoraiOSMoviePlayer::getWidth() const {
        return 0;
    }
    
    uint32 SoraiOSMoviePlayer::getHeight() const {
        return 0;
    }
    
    void* SoraiOSMoviePlayer::getPixelData() const {
        return NULL;
    }
    
    uint32 SoraiOSMoviePlayer::getFrameCount() const {
        return 0;
    }
    
    bool SoraiOSMoviePlayer::frameChanged() const {
        return false;
    }
    
    void SoraiOSMoviePlayer::setFinish() {
    
    }
    
} // namespace sora