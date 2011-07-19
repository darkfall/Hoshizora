//
//  SoraExternalRenderObject.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraExternalRenderObject.h"

namespace sora {
    
    SoraExternalRenderObject::SoraExternalRenderObject(const RenderDelegate& delegate) {
        mDelegate = delegate.clone();
    }
    
    SoraExternalRenderObject::~SoraExternalRenderObject() {
        if(mDelegate) {
            delete mDelegate;
        }
    }
    
    void SoraExternalRenderObject::setDelegate(const RenderDelegate& delegate) {
        if(mDelegate) {
            delete mDelegate;
        }
        mDelegate = delegate.clone();
    }
    
    void SoraExternalRenderObject::render() {
        if(mDelegate && this->getParent()) {
            mDelegate->notify(this, *this->getParent());
        }
    }
    
} // namespace sora
