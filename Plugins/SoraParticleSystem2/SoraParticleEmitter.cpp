//
//  ParticleEmitter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraParticleEmitter.h"
#include "SoraCore.h"

namespace sora {
    
    namespace particle {
        ParticleEmitter::ParticleEmitter():
        SoraComponent(getName()), 
        mSprite(0),
        mInternalSprite(false),
        mUpdateInterval(0.f),
        mLifeTime(0.f),
        mCurrLifeTime(0.f), 
        mActive(false),
        mPaused(false) {
            
        }
        
        void ParticleEmitter::onUpdate(float dt) {
            // cache the interval
            // we put node update in render for faster traversal
            mUpdateInterval = dt;
            
            if(isActive()) {
                if(mLifeTime != 0.f) {
                    mCurrLifeTime += dt;
                    if(mCurrLifeTime >= mLifeTime) 
                        stop();
                }
            
                if(mSprite)
                    mSprite->update(dt);
                emitterUpdate(dt);
            }
        }
        
        void ParticleEmitter::onRender() {
            if(mParticles.size() == 0)
                return;
            
            ParticleNodeList::iterator it = mParticles.begin();
            ParticleNodeList::iterator end = mParticles.end();
            
            for(; it != end; ++it) {
                ParticleNode* node = (*it);
                node->transform(mUpdateInterval);
                node->render();
                
                if(node->isDead()) {
                   // delete node;
                    it = mParticles.erase(it);
                    end = mParticles.end();
                }
            }
        }
        
        void ParticleEmitter::setTextureRect(float x, float y, float w, float h) {
            if(mSprite) {
                mSprite->setTextureRect(x, y, w, h);
                mSprite->setCenter(w/2, h/2);
            }
        }
        
        void ParticleEmitter::emit(ParticleNode* node) {
            sora_assert(node);
            
            node->mSprite = mSprite;
            
            if(mModifiers.size() != 0) {
                ParticleModifierList::iterator it = mModifiers.begin();
                for(; it != mModifiers.end(); ++it) {
                    (*it)->modify(node);
                }
            }  
            mParticles.push_back(node);
        }
        
        void ParticleEmitter::setSprite(const SoraWString& path) {
            if(mInternalSprite && mSprite)
                delete mSprite;
            mSprite = SoraCore::Instance()->createSprite(path);
            if(mSprite) {
                mSprite->setCenter(mSprite->getSpriteWidth()/2,
                                   mSprite->getSpriteHeight()/2);
                mInternalSprite = true;
            }
        }
        
        void ParticleEmitter::setSprite(SoraSprite* spr) {
            if(mInternalSprite && mSprite)
                delete mSprite;
            mSprite = spr;
            if(mSprite) {
                mSprite->setCenter(mSprite->getSpriteWidth()/2,
                                   mSprite->getSpriteHeight()/2);
            }
            mInternalSprite = false;
        }
        
        void ParticleEmitter::setLifeTime(float dt) {
            mLifeTime = dt;
        }
        
        float ParticleEmitter::getLifeTime() const {
            return mLifeTime;
        }
        
        void ParticleEmitter::fire(bool isclear) {
            if(mActive && isclear)
                clear();
            mActive = true;
        }
        
        void ParticleEmitter::fireAt(float x, float y, bool clear) {
            setPosition(x, y);
            fire(clear);
        }
        
        void ParticleEmitter::stop() {
            mActive = false;
        }
        
        void ParticleEmitter::pause() {
            if(mActive)
                mPaused = true;
        }
        
        void ParticleEmitter::resume() {
            mPaused = false;
        }
        
        SoraRect ParticleEmitter::getBoundingBox() const {
            return mBoundingBox;
        }
        
        SoraSprite* ParticleEmitter::getSprite() const {
            return mSprite;
        }
        
        bool ParticleEmitter::isActive() const {
            return mActive && !mPaused;
        }
        
        void ParticleEmitter::clear() {
            mParticles.clear();
        }
        
        void ParticleEmitter::setPosition(float x, float y) {
            mPosition.x = x;
            mPosition.y = y;
        }
        
        SoraVector ParticleEmitter::getPosition() const {
            return mPosition;
        }
        
        void ParticleEmitter::onMessage(SoraMessageEvent* evt) {
            MessageIdType message = evt->getMessage();
            if(message == "fire")
                fire();
            else if(message == "fireat") {
                try {
                    SoraVector pos = evt->getData<SoraVector>();
                    setPosition(pos.x, pos.y);
                    fire();
                } catch(...) {
                    log_error("ParticleEmitter: invalid fireat message 'error data type'");
                }
            } else if(message == "stop") {
                stop();
            } else if(message == "pause") {
                pause();
            } else if(message == "resume") {
                resume();
            } else if(message == "setlifetime") {
                try {
                    float time = evt->getData<float>();
                    setLifeTime(time);
                } catch(...) {
                    log_error("ParticleEmitter: invalid setlifetime message 'error data type'");
                }
            } else if(message == "setsprite") {
                try {
                    SoraWString str = evt->getData<SoraWString>();
                    setSprite(str);
                } catch(...) {
                    log_error("ParticleEmitter: invalid setsprite message 'error data type'");
                }
            } else if(message == "setposition") {
                try {
                    SoraVector pos = evt->getData<SoraVector>();
                    setPosition(pos.x, pos.y);
                } catch(...) {
                    log_error("ParticleEmitter: invalid setposition message 'error data type'");
                }
            }
        }
        
        void ParticleEmitter::addModifier(ParticleModifier* mod) {
            mModifiers.push_back(mod);
        }
        
        ParticleModifier* ParticleEmitter::delModifier(ParticleModifier* mod, bool release) {
            ParticleModifierList::iterator it = mModifiers.begin();
            for(; it != mModifiers.end(); ++it) {
                if((*it) == mod) {
                    ParticleModifier* mod = *it;
                    if(release) {
                        delete mod;
                    }
                    mModifiers.erase(it);
                    return mod;
                }
            }
            return 0; 
        }
    } // namespace particle
    
} // namespace sora
