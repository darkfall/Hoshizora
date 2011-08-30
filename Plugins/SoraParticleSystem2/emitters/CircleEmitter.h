//
//  CircleEmitter.h
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_CircleEmitter_h
#define Sora_CircleEmitter_h

#include "../SoraBasicParticleEmitter.h"

namespace sora {
    
    namespace particle {
        
        class CircleEmitter: public BasicParticleEmitter {
        public:
            CircleEmitter(int mode = MODE_ALL);
            
            enum {
                // emit a circle at a time
                MODE_ALL,
                // emit particle in a circle loop
                MODE_LOOP
            };
            
            void emitterUpdate(float dt);
            
            void setMode(int mode);
            int getMode() const;
            
            void setEmitInterval(float interval);
            float getEmitInterval() const;
            
            void setRadiusInterval(float r);
            float getRadiusInterval() const;
            
            void setEmitNum(int num);
            int getEmitNum() const;
            
            inline void doEmit();
            
        private:
            int mMode;
            
            float mInterval;
            float mEmitNum;
            
            float mCurrInterval;
            float mCurrRadius;
            
            float mRadiusInterval;
        };
        
    } // namespace emitter
    
    
} // namespace sora



#endif
