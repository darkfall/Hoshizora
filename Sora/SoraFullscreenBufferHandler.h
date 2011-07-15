//
//  SoraFullscreenBufferHandler.h
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraFullscreenBufferHandler_h
#define Sora_SoraFullscreenBufferHandler_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraEvent.h"
#include "SoraDelegate.h"
#include "SoraSprite.h"

namespace sora {
    
    class SoraFullscreenBufferHandler: public SoraSingleton<SoraFullscreenBufferHandler> {
    protected:  
        friend class SoraSingleton<SoraFullscreenBufferHandler>;
        
        SoraFullscreenBufferHandler();
        ~SoraFullscreenBufferHandler();
        
    public:
        void onBufferRender(HSORATEXTURE buffer);
        void registerDelegate(SoraAbstractDelegate<HSORATEXTURE>* delegate);
        
    private:
        SoraSprite* mBufferSprite;
        ulong32 mBufferTarget;
        
        SoraAbstractDelegate<HSORATEXTURE>* mDelegate;
    };
    
} // namespace sora


#endif
