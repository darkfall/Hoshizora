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
    
    SoraiOSMusicFile::SoraiOSMusicFile(const SoraWString& sFilePath, bool bStream) {
        bIsStream = bStream;
        mMusicTrack = [[OALAudioTrack alloc] init];
        
        readFile(sFilePath);
    }
    
    SoraiOSMusicFile::~SoraiOSMusicFile() {
        if(mMusicTrack)
            [mMusicTrack release];
    }
    
    int32 SoraiOSMusicFile::readFile(const SoraWString& sFilePath) {
        if(!bIsStream) {
            NSString* path = [[[NSString alloc] initWithUTF8String:ws2s(sFilePath).c_str()] retain];
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
    
    int32 SoraiOSMusicFile::readFileMem(void* ptr, ulong32 size) {
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
            NSString* path = [[[NSString alloc] initWithUTF8String:ws2s(sFilePath).c_str()] retain];
            [mMusicTrack playFile:path];
            [path release];
        }
    }
    
    void SoraiOSMusicFile::stop() {
        [mMusicTrack stop];
    }
    
    void SoraiOSMusicFile::pause() {
        [mMusicTrack setPaused: true];
    }
    
    void SoraiOSMusicFile::resume() {
        [mMusicTrack setPaused: false];
    }
    
    void SoraiOSMusicFile::setVolume(float32 vol) {
        [mMusicTrack setVolume: vol];
    }
    
    void SoraiOSMusicFile::setPan(float32 pan) {
        [mMusicTrack setPan: pan];
    }
    
    void SoraiOSMusicFile::setPitch(float32 pitch) {
      //  [mMusicTrack setPitch: pitch];
    }
    
    float32 SoraiOSMusicFile::getPan() const {
        return [mMusicTrack pan];
    }
    
    float32 SoraiOSMusicFile::getPitch() const {
        return 0.f;
    }
    
    float32 SoraiOSMusicFile::getVolume() const {
        return [mMusicTrack volume];
    }
    
    
    void SoraiOSMusicFile::setPosition(float32 x, float32 y, float32 z) {
        
    }
    
    void SoraiOSMusicFile::getPosition(float32& x, float32& y, float32& z) {
        
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
    
    SoraiOSSoundEffectFile::SoraiOSSoundEffectFile(const SoraWString& path) {
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
    
    int32 SoraiOSSoundEffectFile::readFile(const SoraWString& sFilePath) {
        mBuffer = SoraOALController::loadBuffer(sFilePath);
        return mBuffer != nil;
    }
    
    int32 SoraiOSSoundEffectFile::readFileMem(void* ptr, ulong32 size) {
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
    
    void SoraiOSSoundEffectFile::setPosition(float32 x, float32 y, float32 z) {
        
    }
    
    void SoraiOSSoundEffectFile::getPosition(float32& x, float32& y, float32& z) {
        
    }
    
    void SoraiOSSoundEffectFile::setVolume(float32 vol) {
        mVolume = vol;
    }
    
    void SoraiOSSoundEffectFile::setPan(float32 pan) {
        mPan = pan;
    }
    
    void SoraiOSSoundEffectFile::setPitch(float32 pitch) {
        mPitch = pitch;
    }
    
    float32 SoraiOSSoundEffectFile::getPan() const {
        return mPan;
    }
    
    float32 SoraiOSSoundEffectFile::getPitch() const {
        return mPitch;
    }
    
    float32 SoraiOSSoundEffectFile::getVolume() const {
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
