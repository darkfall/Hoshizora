//
//  SoraBasicParticleEmitter.h
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBasicParticleEmitter_h
#define Sora_SoraBasicParticleEmitter_h

#include "SoraParticleModifier.h"
#include "SoraMath.h"

namespace sora {
    
    namespace particle {

        /**
         * Particle Modifier with
         * Speed
         * Acceleration(Force and Friction)
         * Position offset
         * Triangle Acceleration
         * Spin
         * Randomization
         * Support
         **/
        
        class BasicParticleModifier: public ParticleModifier {
        public:
            BasicParticleModifier();
            
            void setParticleLifeTime(float life);
            float getParticleLifeTime() const;
            
            void setSpeed(float speed);
            float getSpeed();
            
            void setSpin(float spin);
            float getSpin() const;
            
            void setScale(float start, float end);
            void setScale(const SoraVector& start, const SoraVector& end);
            SoraVector getScaleStart() const;
            SoraVector getScaleEnd() const;
            
            void setColor(uint32 start, uint32 end);
            uint32 getStartColor() const;
            uint32 getEndColor() const;
                       
            void setFriction(float friction);
            float getFriction() const;
            
            void setTriganleAcceleration(float taccel);
            float getTriangleAcceleration() const;
            
            void setAcceleration(const SoraVector& accel);
            SoraVector getAcceleration() const;
            
            void setGravity(float gravity);
            float getGravity() const;
            
            virtual void modify(ParticleNode* node);
            
        private:
            float mSpeed;
            float mSpin;
            float mFriction;
            float mTrigAcceleration;
            float mGravity;
            float mParticleLifeTime;
            
            SoraVector mStartScale;
            SoraVector mEndScale;
            SoraVector mAcceleration;

            uint32 mStartColor;
            uint32 mEndColor;
        };
        
    } // namespace particle
    
} // namespace sora


#endif
