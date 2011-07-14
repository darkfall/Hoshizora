/*
 *  SoraGenericObject.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ENTITY_H_
#define SORA_ENTITY_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraSprite.h"
#include "SoraObjectHandle.h"
#include "SoraCustomShapeSprite.h"

#include "SoraSpriteParser/SoraSpriteParser.h"
#include "SoraBox2D/SoraPhysicalObject.h"
#include "SoraLua/SoraLuaObject.h"
#include "SoraSpriteAnimation/SoraSpriteAnimation.h"
#include "SoraGifSprite/SoraGifSprite.h"

namespace sora {
	
	/** 
	 *	A Generic Actor for project usage
	 *	Target:
	 *	Support png, jpeg, tga, bmp image format
	 *  Support intergration with other plugins, physical object etc
	 *	Support sub objects
	 *  Support Gif animation
	 *	Support Shader and ImageEffect
	 *	Support text
	 **/
	
	class SoraActor: public SoraObject {
	public:
        enum ActorType {
            // internal value
            ACTOR_NULL = -1,
            
            ACTOR_SPRITE = 0,
            ACTOR_ANIMATED_SPRITE,
            ACTOR_GIF,
        };
        
        SoraActor(ActorType type, const SoraWString& filePath, float32 posx, float32 posy, bool isInSpriteParser=false);
		~SoraActor();

        void render();
        void render(float32 x, float32 y);
        uint32 update(float32 dt);
        
        bool loadVerticesFromFile(const SoraWString& file, int32 renderMode);
        SoraCustomShapeSprite* getCustomShapeSprite() const;
        
        SoraSprite* getSprite() const;
        SoraSpriteAnimation* getAnimation() const;
        
        void attachShader(SoraShader* shader);
        SoraShader* attachShader(const SoraWString& filePath, const SoraString& entry, int32 shaderType);
        void detachShader(SoraShader* shader);
        
        SoraObjectHandle getHandle();
        ActorType getType() const;
        
        void moveTo(float32 px, float32 py, float32 inTime);
        void playAnim(const SoraString& name, bool repeat, bool queue);
        
        void bindPhysicalObject(SoraPhysicalObject* phyObj);
        void bindLuaObject(SoraLuaObject* luaObj);
        
        SoraPhysicalObject* getPhysicalObject() const;
        SoraLuaObject* getLuaObject() const;
        
    private:
        SoraSprite* m_Sprite;
        SoraCustomShapeSprite* m_VertexSprite;
        
        SoraPhysicalObject* m_PropPhysics;
        SoraLuaObject* m_PropLua;
        
        ActorType m_ActorType;
	};
	
} // namespace sora

#endif // SORA_GENERIC_OBJECT_H_