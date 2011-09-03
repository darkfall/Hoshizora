/*
 *  SoraGenericObject.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraActor.h"
#include "SoraModifierAdapter.h"
#include "SoraCore.h"

namespace sora {

    SoraActor::SoraActor(ActorType type, const SoraWString& filePath, float32 posx, float32 posy, bool isInSpriteParser):
    m_Sprite(NULL),
    m_VertexSprite(NULL),
    m_ActorType(type) {
        switch(m_ActorType) {
            case ACTOR_SPRITE:
                if(isInSpriteParser) {
                    m_Sprite = GetSpriteFromParser(ws2sfast(filePath));
                    if(m_Sprite == NULL) {
                        DebugPtr->log(vamssg("Error loading sprite with path = %s", ws2sfast(filePath).c_str()));
                        m_ActorType = ACTOR_NULL;
                    }
                } else {
                    m_Sprite = SoraCore::Instance()->createSprite(filePath);
                    if(m_Sprite == NULL) {
                        DebugPtr->log(vamssg("Error loading sprite with path = %s", ws2sfast(filePath).c_str()));
                        m_ActorType = ACTOR_NULL;
                    }
                }
                break;
                
            case ACTOR_ANIMATED_SPRITE:
                if(isInSpriteParser) {
                    m_Sprite = GetAnimationFromParser(ws2sfast(filePath));
                    if(m_Sprite == NULL) {
                        DebugPtr->log(vamssg("Error loading sprite with path = %s", ws2sfast(filePath).c_str()));
                        m_ActorType = ACTOR_NULL;
                    }
                } else {
                    m_Sprite = new SoraSpriteAnimation(filePath);
                    if(m_Sprite == NULL) {
                        DebugPtr->log(vamssg("Error loading sprite with path = %s", ws2sfast(filePath).c_str()));
                        m_ActorType = ACTOR_NULL;
                    }
                }
                break;
                
            case ACTOR_GIF:
                m_Sprite = new SoraGifSprite(filePath);
                if(m_Sprite == NULL) {
                    DebugPtr->log(vamssg("Error loading sprite with path = %s", ws2sfast(filePath).c_str()));
                    m_ActorType = ACTOR_NULL;
                }
                break;
                
            default:
                THROW_SORA_EXCEPTION(InvalidArgumentException, "Missing Actor Type");
        }
    }
    
    SoraActor::~SoraActor() {
        if(m_Sprite) 
            delete m_Sprite;
    }
    
    void SoraActor::render() {
        if(!m_VertexSprite) {
            if(m_Sprite)
                m_Sprite->render(getPositionX(), getPositionY());
        }
        if(m_PropLua)
            m_PropLua->render();
    }
    
    void SoraActor::render(float32 x, float32 y) {
        if(m_Sprite)
            m_Sprite->render(x, y);
        if(m_PropLua)
            m_PropLua->render();
    }
    
    uint32 SoraActor::update(float32 dt) {
        if(m_Sprite) {
            m_Sprite->update(dt);
            if(m_PropLua)
                m_PropLua->update(dt);
            if(m_PropPhysics) {
                m_PropPhysics->update(dt);
                setPosition(m_PropPhysics->getPositionX(), 
                            m_PropPhysics->getPositionY());
                m_Sprite->setRotation(m_PropPhysics->getAngle());
            }
        }
        return 0;
    }
    
    bool SoraActor::loadVerticesFromFile(const SoraWString& file, int32 renderMode) {
        if(!m_VertexSprite) {
            m_VertexSprite = new SoraCustomShapeSprite(m_Sprite, renderMode);
        }
        assert(m_VertexSprite);
        bool result = m_VertexSprite->loadVerticesFromFile(file);
        return result;
    }
    
    SoraSprite* SoraActor::getSprite() const {
        return m_Sprite;
    }
    
    SoraSpriteAnimation* SoraActor::getAnimation() const {
        if(m_ActorType == ACTOR_ANIMATED_SPRITE) {
            return dynamic_cast<SoraSpriteAnimation*>(m_Sprite);
        }
        return NULL;
    }
    
    void SoraActor::attachShader(SoraShader* shader) {
        if(m_Sprite)
            m_Sprite->attachShader(shader);
    }
    
    SoraShader* SoraActor::attachShader(const SoraWString& file, const SoraString& entry, int32 type) {
        if(m_Sprite)
            return m_Sprite->attachShader(file, entry, type);
        return NULL;
    }
    
    void SoraActor::detachShader(SoraShader* shader) {
        if(m_Sprite)
            m_Sprite->detachShader(shader);
    }
    
    SoraObjectHandle SoraActor::getHandle() {
        return SoraObjectHandle(this);
    }
    
    SoraActor::ActorType SoraActor::getType() const {
        return m_ActorType;
    }

    void SoraActor::moveTo(float32 px, float32 py, float32 inTime) {
        /*new SoraModifierAdapter<SoraObject>(this, 
                                           CreateEffectTransitions(getPositionX(), 
                                                                   getPositionY(),
                                                                   px, 
                                                                   py,
                                                                   inTime, 
                                                                   IMAGE_EFFECT_ONCE));*/
        
    }
    
    void SoraActor::playAnim(const SoraString& name, bool repeat, bool queue) {
        if(m_ActorType == ACTOR_ANIMATED_SPRITE) {
            SoraSpriteAnimation* anm = static_cast<SoraSpriteAnimation*>(m_Sprite);
            if(anm != NULL)
                anm->playEx(name.c_str(), repeat, queue);
        }
    }
    
    void SoraActor::bindPhysicalObject(SoraPhysicalObject* phyObj) {
        m_PropPhysics = phyObj;
    }
    
    void SoraActor::bindLuaObject(SoraLuaObject* luaObj) {
        m_PropLua = luaObj;
    }
    
    SoraPhysicalObject* SoraActor::getPhysicalObject() const {
        return m_PropPhysics;
    }
    
    SoraLuaObject* SoraActor::getLuaObject() const {
        return m_PropLua;
    }
    
} // namespace sora