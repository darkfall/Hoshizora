//
//  SoraOALParam.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/15/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraOALParam_h
#define Sora_iPhone_SoraOALParam_h

namespace sora {
    
    struct SoraOALParameters {
        // allow ipod music keep playing or not
        // default: NO
        bool AllowiPodMusic;
        // allow slient switch to turn off all sounds or not
        // default: NO
        bool AllowSlientSwitch;
        
        // default: YES
        bool HandleInterruption;
        
        // default: 32
        int DefaultChannelSize;
        
        SoraOALParameters(bool ipod=false, bool silentSwitch=true, bool interruption=true, int channelsize=32): 
        AllowiPodMusic(ipod), 
        AllowSlientSwitch(silentSwitch),
        HandleInterruption(interruption),
        DefaultChannelSize(channelsize) {}
    };
    
     
} // namespace sora



#endif
