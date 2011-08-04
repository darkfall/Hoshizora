//
//  SoraFullscreenBufferHandler.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraFullscreenBufferHandler.h"

namespace sora {
    
    SoraFullscreenBufferHandler::SoraFullscreenBufferHandler() {
        mBufferSprite = new SoraSprite(0);
        assert(mBufferSprite);
        
        mDelegate = NULL;
    }
    
    SoraFullscreenBufferHandler::~SoraFullscreenBufferHandler() {
        if(mDelegate) {
            delete mDelegate;
            mDelegate = NULL;
        }
    }
    
    void SoraFullscreenBufferHandler::onBufferRender(HSORATEXTURE bufferTex) {
        if(!mDelegate) {    
            if(mBufferSprite) {
                mBufferSprite->setTexture(bufferTex);
                mBufferSprite->render();
            } else {
                mBufferSprite = new SoraSprite(bufferTex);
                if(!mBufferSprite) {
                    THROW_SORA_EXCEPTION(NullPointerException, "No buffer sprite available");
                }
            }
        } else 
            mDelegate->notify(this, bufferTex);
    }

    void SoraFullscreenBufferHandler::registerDelegate(SoraAbstractDelegate<HSORATEXTURE>* delegate) {
        if(mDelegate != NULL)
            delete mDelegate;
        mDelegate = delegate;
    }
    
    
    
} // namespace sora
