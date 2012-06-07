//
//  SoraParticleSystem.h
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraParticleFSystem_h
#define SoraF_SoraParticleFSystem_h

#include "SoraParticleFNode.h"
#include "SoraObject.h"
#include "SoraAutoPtr.h"

#include <map>

namespace sora {
    
    class SoraParticleFEmitter;
    class SoraParticleFRenderer;
    
    class SoraParticleFSystem: public SoraObject {
    public:
        SoraParticleFSystem();
        virtual ~SoraParticleFSystem();
        
    public:
        // SoraObject
        int32 update(float dt);
        void  render();
        
    private:
        friend class SoraParticleFEmitter;
        
    public:
        
        typedef SoraAutoPtr<SoraParticleFEmitter> EmitterPtr;
        typedef SoraAutoPtr<SoraParticleFRenderer> RendererPtr;
        
        EmitterPtr getEmitter(const SoraString& name);
        RendererPtr getRenderer(const SoraString& name);
        
        EmitterPtr removeEmitter(const SoraString& name);
        RendererPtr removeRenderer(const SoraString& name);
        
        void addRenderer(const RendererPtr& renderer);
        void addEmitter(const EmitterPtr& emitter);
                
    private:
        typedef std::map<SoraString, EmitterPtr> EmitterMap;
        typedef std::map<SoraString, RendererPtr> RendererMap;
        
        EmitterMap mEmitters;
        RendererMap mRenderers;
    };
    
}


#endif
