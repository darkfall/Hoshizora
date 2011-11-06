//
//  SoraiOSSoundFile.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/15/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSSoundFile.h"

namespace sora {
    
    SoraiOSMusicFile::SoraiOSMusicFile(bool bStream) {
        bIsStream = bStream;
        mMusicTrack = [[OALAudioTrack alloc] init];
    }
    
    SoraiOSMusicFile::SoraiOSMusicFile(const StringType& sFilePath, bool bStream) {
        bIsStream = bStream;
        mMusicTrack = [[OALAudioTrack alloc] init];
        
        readFile(sFilePath);
    }
    
    SoraiOSMusicFile::~SoraiOSMusicFile() {
        if(mMusicTrack)
            [mMusicTrack release];
    }
    
    int32 SoraiOSMusicFile::readFile(const StringType& sFilePath) {
        if(!bIsStream) {
            NSString* path = [[[NSString alloc] initWithUTF8String:sFilePath.c_str()] retain];
            if(mMusicTrack) {
                bool result = [mMusicTrack preloadFile: path];
                [path release];
                return result?1:0;
            }
            [path release];
        }
        
        setFilePath(sFilePath);
        return 0;
    }
    
    int32 SoraiOSMusicFile::readFileMem(void* ptr, uint32 size) {
        // we don't support memory here
        return 0;
    }
    
    void SoraiOSMusicFile::closeFile() {
        [mMusicTrack stop];
    }
    
    void SoraiOSMusicFile::play() {
        if([mMusicTrack playing]) {
            [mMusicTrack stop];
        }
        
        if(!bIsStream)
            [mMusicTrack play];
        else {
            NSString* path = [[[NSString alloc] initWithUTF8String:getFilePath().c_str()] retain];
            [mMusicTrack playFile:path];
            [path release];
        }
        
        publishEvent(SoraPlaybackEvent::Started);
    }
    
    void SoraiOSMusicFile::stop() {
        [mMusicTrack stop];
        publishEvent(SoraPlaybackEvent::Stopped);
    }
    
    void SoraiOSMusicFile::pause() {
        [mMusicTrack setPaused: true];
        publishEvent(SoraPlaybackEvent::Paused);
    }
    
    void SoraiOSMusicFile::resume() {
        [mMusicTrack setPaused: false];
        publishEvent(SoraPlaybackEvent::Resumed);
    }
    
    void SoraiOSMusicFile::setVolume(float vol) {
        [mMusicTrack setVolume: vol];
    }
    
    void SoraiOSMusicFile::setPan(float pan) {
        [mMusicTrack setPan: pan];
    }
    
    void SoraiOSMusicFile::setPitch(float pitch) {
      //  [mMusicTrack setPitch: pitch];
    }
    
    float SoraiOSMusicFile::getPan() const {
        return [mMusicTrack pan];
    }
    
    float SoraiOSMusicFile::getPitch() const {
        return 0.f;
    }
    
    float SoraiOSMusicFile::getVolume() const {
        return [mMusicTrack volume];
    }
    
    
    void SoraiOSMusicFile::setPosition(float x, float y, float z) {
        
    }
    
    void SoraiOSMusicFile::getPosition(float& x, float& y, float& z) {
        
    }
    
    void SoraiOSMusicFile::setMusicPosition(int32 pos) {
        [mMusicTrack setCurrentTime:pos];
    }
    
    int32 SoraiOSMusicFile::getMusicPosition() const {
        return [mMusicTrack currentTime];
    }
    
    
    int32 SoraiOSMusicFile::getLength() const {
        return [mMusicTrack duration];
    }
    
    void SoraiOSMusicFile::setRepeat(bool flag) {
        if(flag)
            [mMusicTrack setNumberOfLoops:-1];
        else
            [mMusicTrack setNumberOfLoops:0];
    }
    
    bool SoraiOSMusicFile::isRepeat() const {
        return [mMusicTrack numberOfLoops] == -1;
    }
    
    bool SoraiOSMusicFile::isPlaying() const {
        return [mMusicTrack playing];
    }

    SoraiOSSoundEffectFile::SoraiOSSoundEffectFile() {
        mBuffer = nil;
        mRepeat = false;
        mVolume = 1.f;
        mPitch = 1.f;
        mPan = 0.f;
    }
    
    SoraiOSSoundEffectFile::SoraiOSSoundEffectFile(const StringType& path) {
        readFile(path);
        mRepeat = false;
        mVolume = 1.f;
        mPitch = 1.f;
        mPan = 0.f;
    }
    
    SoraiOSSoundEffectFile::~SoraiOSSoundEffectFile() {
        if(mBuffer)
            SoraOALController::releaseBuffer(mBuffer);
    }
    
    int32 SoraiOSSoundEffectFile::readFile(const StringType& sFilePath) {
        mBuffer = SoraOALController::loadBuffer(sFilePath);
        return mBuffer != nil;
    }
    
    int32 SoraiOSSoundEffectFile::readFileMem(void* ptr, uint32 size) {
        // no memory buffer support here
        return 0;
    }
    
    void SoraiOSSoundEffectFile::closeFile() {
        if(mBuffer) {
            SoraOALController::releaseBuffer(mBuffer);
            mBuffer = nil;
        }
    }
    
    void SoraiOSSoundEffectFile::play() {
        if(mBuffer) {
            mSoundSource = SoraOALController::playBuffer(mBuffer, mVolume, mPan, mPitch, mRepeat);
        }
    }
    
    void SoraiOSSoundEffectFile::stop() {
        if(mBuffer && mRepeat) {
            [mSoundSource stop];
        }
    }
    
    void SoraiOSSoundEffectFile::setPosition(float x, float y, float z) {
        
    }
    
    void SoraiOSSoundEffectFile::getPosition(float& x, float& y, float& z) {
        
    }
    
    void SoraiOSSoundEffectFile::setVolume(float vol) {
        mVolume = vol;
    }
    
    void SoraiOSSoundEffectFile::setPan(float pan) {
        mPan = pan;
    }
    
    void SoraiOSSoundEffectFile::setPitch(float pitch) {
        mPitch = pitch;
    }
    
    float SoraiOSSoundEffectFile::getPan() const {
        return mPan;
    }
    
    float SoraiOSSoundEffectFile::getPitch() const {
        return mPitch;
    }
    
    float SoraiOSSoundEffectFile::getVolume() const {
        return mVolume;
    }
    
    void SoraiOSSoundEffectFile::setRepeat(bool flag) {
        mRepeat = flag;
    }
    
    bool SoraiOSSoundEffectFile::isRepeat() const {
        return mRepeat;
    }
    
    bool SoraiOSSoundEffectFile::isPlaying() const {
        // unknown
        ALChannelSource* source = SoraOALController::getChannelSource();
        if(source != nil) {
            return [source playing];
        }
        return false;
    }
    
} // namespace sora
