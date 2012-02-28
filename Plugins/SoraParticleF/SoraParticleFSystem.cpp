//
//  File.cpp
//  SoraF
//
//  Created by Ruiwei Bu on 2/26/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "SoraParticleFSystem.h"
#include "SoraParticleFEmitter.h"
#include "SoraParticleFRenderer.h"

#include "SoraForEach.h"

namespace sora {
        
    SoraParticleFSystem::SoraParticleFSystem() {
    }
    
    SoraParticleFSystem::~SoraParticleFSystem() {
        
    }
    
    int32 SoraParticleFSystem::update(float dt) {
        return SoraObject::update(dt);
    }
    
    void SoraParticleFSystem::render() {
        EmitterMap::const_iterator it = mEmitters.begin();
        for(; it != mEmitters.end(); ++it) {
            RendererMap::iterator renderer = mRenderers.find(it->second->getRendererName());
            
            if(renderer != mRenderers.end()) {
                renderer->second->doRender(it->second->getParticles());
            }
        }
    }
        
    SoraParticleFSystem::EmitterPtr SoraParticleFSystem::getEmitter(const SoraString& name) {
        EmitterMap::const_iterator it = mEmitters.find(name);
        return it != mEmitters.end() ? it->second : EmitterPtr();
    }
    
    SoraParticleFSystem::RendererPtr SoraParticleFSystem::getRenderer(const SoraString& name) {
        RendererMap::const_iterator it = mRenderers.find(name);
        return it != mRenderers.end() ? it->second: RendererPtr();
    }
    
    SoraParticleFSystem::EmitterPtr SoraParticleFSystem::removeEmitter(const SoraString& name) {
        EmitterMap::iterator it = mEmitters.find(name);
        if(it != mEmitters.end()) {
            EmitterPtr ptr = it->second;
            mEmitters.erase(it);
            return ptr;
        }
        return EmitterPtr();
    }
    
    SoraParticleFSystem::RendererPtr SoraParticleFSystem::removeRenderer(const SoraString& name) {
        RendererMap::iterator it = mRenderers.find(name);
        if(it != mRenderers.end()) {
            RendererPtr ptr = it->second;
            mRenderers.erase(it);
            return ptr;
        }
        return RendererPtr();
    }
    
    void SoraParticleFSystem::addRenderer(const RendererPtr& renderer) {
        mRenderers.insert(std::make_pair(renderer->getName(), renderer));
    }
    
    void SoraParticleFSystem::addEmitter(const EmitterPtr& emitter) {
        mEmitters.insert(std::make_pair(emitter->getName(), emitter));
        
        this->runAction(emitter);
    }

    
}