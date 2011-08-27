//
//  SoraFullscreenBufferHandler.h
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFullscreenBufferHandler_h
#define Sora_SoraFullscreenBufferHandler_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraEvent.h"
#include "SoraDelegate.h"
#include "SoraSprite.h"
#include "function/SoraFunction.h"

namespace sora {
    
    class SoraFullscreenBufferHandler: public SoraSingleton<SoraFullscreenBufferHandler> {
    protected:  
        friend class SoraSingleton<SoraFullscreenBufferHandler>;
        
        SoraFullscreenBufferHandler();
        ~SoraFullscreenBufferHandler();
        
    public:
        void onBufferRender(HSORATEXTURE buffer);
        
        typedef SoraFunction<void(HSORATEXTURE)> DelegateFuncType;
        void registerDelegate(const DelegateFuncType& delegate);
        
    private:
        SoraSprite* mBufferSprite;
        ulong32 mBufferTarget;
        
        DelegateFuncType mDelegate;
    };
    
} // namespace sora


#endif
