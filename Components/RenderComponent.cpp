//
//  RenderComponent.cpp
//  Sora
//
//  Created by Robert Bu on 8/27/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "RenderComponent.h"
#include "SoraCore.h"
#include "factory/SoraSpriteFactory.h"
#include "entity/SoraEntity.h"

#include "debug/SoraAutoProfile.h"
#include "debug/SoraGlobalProfiler.h"

namespace sora {
    
    namespace component {
        
        RenderComponent::RenderComponent(SoraLightWeightEntity* ent):
        SoraComponent(ent, true),
        mInternalSprite(false),
        mSprite(0) {
        }
        
        RenderComponent::~RenderComponent() {
            if(mInternalSprite && mSprite)
                delete mSprite;
        }
        
        void RenderComponent::setSprite(const SoraWString& path, const SoraString& type) {
            if(type.empty())
                mSprite = sora::SoraCore::Instance()->createSprite(path);
            else
                mSprite = sora::CreateSpriteType(type, path);
        }
        
        void RenderComponent::setSprite(SoraSprite* spr) {
            if(mInternalSprite&& mSprite)
                delete mSprite;
            mSprite = spr;
            mInternalSprite = false;
        }
        
        SoraSprite* RenderComponent::getSprite() const {
            return mSprite;
        }
        
        void RenderComponent::onMessage(SoraMessageEvent* message) {
            
        }
        
        void RenderComponent::onComponentAdd(SoraComponent* comp) {
    //        if(comp->getName() == CID_Position)
    //            mPosition = static_cast<PositionComponent>(comp);
        }
        
        void RenderComponent::onComponentRemove(SoraComponent* comp) { 
    //        if(comp == mPosition)
    //            mPosition = NULL;
        }
        
        void RenderComponent::onUpdate(float dt) {
            mSprite->update(dt);
        }
        
        void RenderComponent::onRender() {
            PROFILE("render");
            mSprite->render(getOwner()->getProperty("Position.x", 0.f),
                            getOwner()->getProperty("Position.y", 0.f));
            printf("%llu\n", SoraGlobalProfiler::Instance()->getProfile("render").elapsedTime);
        }
        
    } // namespace component
    
} // namespace sora