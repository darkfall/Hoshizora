//
//  SoraParticleFRenderer.h
//  SoraF
//
//  Created by Ruiwei Bu on 2/26/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef SoraF_SoraParticleFRenderer_h
#define SoraF_SoraParticleFRenderer_h

#include "SoraPlatform.h"
#include "SoraSprite.h"

#include "SoraParticleFNode.h"
#include <vector>

namespace sora {
    
    class SoraParticleFEmitter;
    
    class SoraParticleFRenderer {
    public:
        SoraParticleFRenderer();
        virtual ~SoraParticleFRenderer();
        
        virtual const SoraString& getName() const;
        
    public:
        virtual void doRender(const std::vector<SoraParticleFNode>& particles); 
        
    public:
        
        SoraTextureHandle getTexture() const;
        void              setTeture(SoraTextureHandle texture);
        
        SoraRect          getTextureRect() const;
        void              setTextureRect(const SoraRect& rect);
        
    private:
        SoraSprite mSprite;
    };
    
}

#endif
