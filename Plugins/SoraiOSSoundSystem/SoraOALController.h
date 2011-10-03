//
//  OALController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/15/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_OALController_h
#define Sora_iPhone_OALController_h

#include "SoraPlatform.h"
#include "SoraOALParam.h"

#ifdef OS_IOS

#import <Foundation/Foundation.h>
#import "ObjectAL/ObjectAL.h"

#include <list>

// this header must be included by a objectc source file
// otherwise would case lots of trouble

namespace sora {
  
    class SoraOALController {
    public:
        static bool init(const SoraOALParameters& params);
        static void release();
        
        static ALDevice* getDevice();
        static ALContext* getContext();
        // we will reserve 32(maximum) number of channels for effects;
        static ALChannelSource* getChannelSource();
        
        static ALBuffer* loadBuffer(const StringType& file);
        
        static void releaseBuffer(ALBuffer* buffer);
        // release all allocated buffers
        // usually called when the soundsystem is shutting down
        static void releaseAllBuffers();
        
        // play a buffer using the default channel
        // soundeffects go through this way for faster playback
        // you can also change the default channel size when initializing
        static id<ALSoundSource> playBuffer(ALBuffer* buffer, float volume, float pan, float pitch, bool loop);
        // notice this would acctually stop all playbacks
        static void stopBuffer(ALBuffer* buffer);
        
    private:
        static ALDevice* mDevice;
        static ALContext* mContext;
        static ALChannelSource* mChannelSource;
        
        // holds all loaded buffers
        typedef std::list<ALBuffer*> ALBufferList;
        static ALBufferList mLoadedBuffers;
    };
    
    
} // namespace sora

#endif // OS_IOS    

#endif
