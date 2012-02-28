//
//  SoraParticleNode.h
//  SoraF
//
//  Created by Ruiwei Bu on 2/26/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef SoraF_SoraParticleFNode_h
#define SoraF_SoraParticleFNode_h

#include "SoraPlatform.h"
#include "SoraMath.h"
#include "SoraColor.h"
#include "SoraVector3.h"
#include "math/SoraTransform.h"

namespace sora {
    
    struct SoraParticleFNode {
        
        // basic information
        float         speed;
        SoraVector    angle;
        SoraTransform transform;
        SoraColorRGBA color;
    
        // variations
        float         speed_var;
        SoraVector   angle_var;
        SoraVector3   spin_var;
        SoraVector3   scale_var;
        SoraColorRGBA color_var;
        
        // lifetime
        float         life_time;
        float         current_life_time;
        
        void update(float dt) {
            if(current_life_time < life_time) {
                speed += speed_var * dt;
                angle += angle_var * dt;
                color += color_var * dt;

          //      SoraVector3 rot = transform.getRotation().get();
            //    rot += spin_var * dt;
             //   transform.setRotation(rot.x, rot.y, rot.z);
                transform.setScale(scale_var * dt + transform.getScale());
                transform.setPosition(transform.getPosition() + SoraVector3(cosf(angle.x) * speed,
                                                                            sinf(angle.x) * speed,
                                                                            0) * dt);
                current_life_time += dt;
            }
        }
        
        bool isDone() const {
            return current_life_time > life_time;
        }
    };
    
}

#endif
