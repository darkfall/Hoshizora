/*
 *  rfMapRenderer.h
 *  Sora
 *
 *  Created by GriffinBu on 12/14/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef RF_MAP_RENDERER_H_
#define RF_MAP_RENDERER_H_

#include "../Reflection/rfMap.h"

#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraCore.h"

namespace rf {
		
	class rfMapRenderer {
	public:
		rfMapRenderer();
		~rfMapRenderer();
		
		int loadMap(const wchar_t* pstrMapPath);
		int loadMapMem(void* data, uint32 size);
		
		void setBoxSprite(const wchar_t* pstrBoxSpr, const wchar_t* pstrBoxSprHL);
		void setMirrorSprite(const wchar_t* pstrMirrorSpr);
		void setLightSourceSprite(const wchar_t* pstrLightSourceSpr, const wchar_t* pstrLightSpr);
		
		void update(float32 dt);
		void render();
		
		void setPosition(float32 x, float32 y);
		
		uint32 getWidth();
		uint32 getHeight();
		uint16 getMapH();
		uint16 getMapV();
		uint16 getBoxSize();
		
		void setMaxMirrorNum(uint16 maxMirror);
		void setMaxTime(uint16 maxTime);
		
		uint16 getMaxMirrorNum() const;
		uint16 getMaxTime() const;
		uint16 getUsedMirrorNum() const;
		uint16 getUsedTime() const;
		uint16 getBoxAim() const;
		uint16 getBoxGot() const;
		
		bool isWin() const;
		bool isLose() const;
		
		std::wstring getCurrentMap() const;
		
	private:
		void renderLight(uint16 v, uint16 h, float32 dir);
		inline void renderLightInRect(float32 x, float32 y, float32 x1, float32 y2, float32 w);
		
		rfMap* pMap;
		
		float32 cx, cy;
		uint16 mapH, mapV;
		
		sora::SoraSprite* pBoxSpr;
		sora::SoraSprite* pBoxSprHL;
		sora::SoraSprite* pMirrorSpr;
		sora::SoraSprite* pLightSpr;
		sora::SoraSprite* pLightSourceSpr;
		
		sora::SoraCore* CORE;
		
		uint8 mouseDelay;
		
		uint16 maxMirror;
		uint16 maxTime;
		
		uint16 usedMirror;
		float32 usedTime;
		
		uint16 boxAimNum;
		uint16 boxGetNum;
		
		std::wstring currentMap;
	};
} // namespace rf


#endif