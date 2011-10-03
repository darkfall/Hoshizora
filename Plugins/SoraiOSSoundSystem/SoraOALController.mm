//
//  SoraOALController.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/15/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraOALController.h"
#include "SoraStringConv.h"

namespace sora {
    
    ALDevice* SoraOALController::mDevice = 0;
    ALContext* SoraOALController::mContext = 0;
    ALChannelSource* SoraOALController::mChannelSource = 0;
    
    SoraOALController::ALBufferList SoraOALController::mLoadedBuffers;
    
    bool SoraOALController::init(const SoraOALParameters& params) {
        mDevice = [[ALDevice alloc] initWithDeviceSpecifier:nil];
        
        sora_assert(mDevice);
        
        mContext = [[ALContext alloc] initOnDevice:mDevice attributes:nil];
        
        sora_assert(mContext);
        
        [OpenALManager sharedInstance].currentContext = mContext;
        [OALAudioSession sharedInstance].handleInterruptions = params.HandleInterruption;
        [OALAudioSession sharedInstance].allowIpod = params.AllowiPodMusic;
        [OALAudioSession sharedInstance].honorSilentSwitch = params.AllowSlientSwitch;
        
        mChannelSource = [[ALChannelSource alloc] initWithSources:params.DefaultChannelSize];
        
        if(!mDevice || !mContext || !mChannelSource)
            return false;
        return true;
    }
    
    void SoraOALController::release() {
        releaseAllBuffers();
        
        [mChannelSource release];
        [mContext release];
        [mDevice release];
    }
    
    ALDevice* SoraOALController::getDevice() {
        return mDevice;
    }
    
    ALContext* SoraOALController::getContext() {
        return mContext;
    }
    
    ALChannelSource* SoraOALController::getChannelSource() {
        return mChannelSource;
    }
    
    ALBuffer* SoraOALController::loadBuffer(const StringType& file) {
        NSString* path = [[NSString alloc] initWithUTF8String:file.c_str()];
        
        ALBuffer* buffer = [[[OpenALManager sharedInstance] bufferFromFile:path] retain];
        if(buffer) {
            mLoadedBuffers.push_back(buffer);
            return buffer;
        }
        [path release];
        return 0;
    }
    
    void SoraOALController::releaseBuffer(ALBuffer* buffer) {
        sora_assert(buffer != nil);
        
        mLoadedBuffers.remove(buffer);
        [buffer release];
    }
    
    void SoraOALController::releaseAllBuffers() {
        ALBufferList::iterator it = mLoadedBuffers.begin();
        while(it != mLoadedBuffers.end()) {
            ALBuffer* buffer = *it;
            [buffer release];
            
            ++it;
        }
        
        mLoadedBuffers.clear();
    }
    
    id<ALSoundSource> SoraOALController::playBuffer(ALBuffer* buffer, float vol, float panin, float pitchin, bool loopin) {
        sora_assert(buffer != nil);
        sora_assert(mChannelSource != nil);
        
        return [mChannelSource play:buffer gain:vol pitch:pitchin pan:panin loop:loopin];
    }
    
    void SoraOALController::stopBuffer(ALBuffer* buffer) {
        sora_assert(mChannelSource != nil);
        
        [mChannelSource stop];
    }
    
} // namespace sora
