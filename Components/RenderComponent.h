//
//  RenderComponent.h
//  Sora
//
//  Created by Robert Bu on 8/27/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_RenderComponent_h
#define Sora_RenderComponent_h

#include "Entity/SoraComponent.h"
#include "SoraSprite.h"

#include "ComponentIds.h"

namespace sora {
    
    namespace component {
        
      //  class PositionComponent;
        
        class RenderComponent: public SoraComponent {
        public:
            RenderComponent(); 
            RenderComponent(const SoraWString& spritePath, const SoraString& spriteType=SoraString());
            
            ~RenderComponent();
            
            void setSprite(SoraSprite* spr);
            void setSprite(const SoraWString& spritePath, const SoraString& spriteType=SoraString());
            
            SoraSprite* getSprite() const;
            
            // inherited from SoraComponent
            void onMessage(SoraMessageEvent* message);
            void onComponentAdd(SoraComponent* comp);
            void onComponentRemove(SoraComponent* comp);
            
            void onUpdate(float dt);
            void onRender();
            
            SORA_IMPL_COMPONENT(Render);
            
        private:
            bool mInternalSprite;
            sora::SoraSprite* mSprite;
            
    //PositionComponent* mPosition;
        };
        
    } // namespace component
    
    
} // namespace sora



#endif
