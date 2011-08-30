//
//  SoraParticleEmitter.h
//  Sora
//
//  Created by Robert Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraParticleEmitter_2_h
#define Sora_SoraParticleEmitter_2_h

#include "entity/SoraComponent.h"
#include "SoraSprite.h"
#include "SoraParticleNode.h"

namespace sora {

    /**
     * Base class for emitters
     **/
    namespace particle {
        
        using namespace sora;
        
        struct ParticleEmitter: SoraComponent {
            ParticleEmitter();
            
            void setSprite(const SoraWString& path);
            void setSprite(SoraSprite* spr);
            void setTextureRect(float x, float y, float w, float h);
            SoraSprite* getSprite() const;
            
            void setLifeTime(float dt);
            float getLifeTime() const;
            
            void setPosition(float x, float y);
            SoraVector getPosition() const;
            
            bool isActive() const;
            
            void fire(bool clearPrev=true);
            void fireAt(float x, float y, bool clearPrev=true);
            
            void stop();
            void pause();
            void resume();
            
            void clear();
            
            SoraRect getBoundingBox() const;
            
            void onUpdate(float dt);
            void onRender();
            
            virtual void onMessage(SoraMessageEvent* evt);
            
            
            // emit a particle and insert into list
            // you can override it to add atrributes to it
            virtual void emit(ParticleNode* node);
            
            // for subclasses update theirselves
            // would be called every update frame
            virtual void emitterUpdate(float dt) = 0;
            
            SORA_DEF_COMPONENT(ParticleEmitter)
            
        protected:
            SoraSprite* mSprite;
            bool mInternalSprite;
            
            float mUpdateInterval;
            float mLifeTime;
            float mCurrLifeTime;
            
            typedef std::vector<ParticleNode*> ParticleNodeList;
            ParticleNodeList mParticles;
            
            bool mActive;
            bool mPaused;
            SoraVector mPosition;
            SoraRect mBoundingBox;
        };
        
    } // namepace particle

} // namespace sora


#endif
