/*
 *  rfMapRenderer.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/14/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "rfMapRenderer.h"
#include "SoraMath.h"
#include "Rect4V.h"
#include <cmath>

namespace rf {
	
#define RF_MOUSE_DELAY 10
	
	rfMapRenderer::rfMapRenderer() {
		cx = cy = 0.f;
		pMap = 0;
		
		pBoxSpr = 0;
		pBoxSprHL = 0;
		pMirrorSpr = 0;
		pLightSpr = 0;
		pLightSourceSpr = 0;
		
		mapH = mapV = 0;
		
		mouseDelay = 0;
		
		maxMirror = maxTime = 0;
		
		CORE = sora::SoraCore::Instance();
	}
	
	rfMapRenderer::~rfMapRenderer() {
		if(pMap) {
			delete pMap;
			pMap = 0;
		}
		if(pBoxSpr) 
			delete pBoxSpr;
		if(pBoxSprHL)
			delete pBoxSprHL;
		if(pMirrorSpr)
			delete pMirrorSpr;
		if(pLightSpr)
			delete pLightSpr;
		if(pLightSourceSpr)
			delete pLightSourceSpr;
	}
	
	void rfMapRenderer::setPosition(float32 x, float32 y) {
		cx = x;
		cy = y;
	}
	
	void rfMapRenderer::setMaxMirrorNum(uint16 num) {
		maxMirror = num;
	}
	
	void rfMapRenderer::setMaxTime(uint16 time) {
		maxTime = time;
	}
	
	uint16 rfMapRenderer::getMaxMirrorNum() const {
		return maxMirror;
	}
	
	uint16 rfMapRenderer::getMaxTime() const {
		return maxTime;
	}
	
	uint16 rfMapRenderer::getUsedMirrorNum() const {
		return usedMirror;
	}
	
	uint16 rfMapRenderer::getUsedTime() const {
		return (uint16)usedTime;
	}
	
	bool rfMapRenderer::isWin() const {
		return (boxAimNum == boxGetNum && boxAimNum != 0);
	}
	
	bool rfMapRenderer::isLose() const {
		return (maxTime != 0 && usedTime >= maxTime);
	}
	
	uint16 rfMapRenderer::getBoxAim() const {
		return boxAimNum;
	}
	
	uint16 rfMapRenderer::getBoxGot() const {
		return boxGetNum;
	}
	
	void rfMapRenderer::render() {
		if(!pMap) return;
		if(!pBoxSpr || !pBoxSprHL || !pMirrorSpr || !pLightSourceSpr) return;
		
		boxAimNum = boxGetNum = 0;
		uint16 bsize = pMap->getGlobalBoxSize();
		for(uint16 i=0; i<mapV; ++i) {
			for(uint16 j=0; j<mapH; ++j) {
				rfBox box = pMap->getBox(i, j);
				if(box.uAvailable) {
					if(box.uIsLightPath == 1) {
						++boxAimNum;
						pBoxSprHL->setColor(0xFFFFFFFF);
						if(box.uLightPassed == 1) {
							++boxGetNum;
							pBoxSprHL->setColor(0xFF808080);
							pBoxSprHL->render(cx+box.BoxPosX+bsize/2, cy+box.BoxPosY+bsize/2);
						}
						else {
							pBoxSprHL->render(cx+box.BoxPosX+bsize/2, cy+box.BoxPosY+bsize/2);
						}
					}
					else {
						pBoxSpr->render(cx+box.BoxPosX+bsize/2, cy+box.BoxPosY+bsize/2);
					}
				}
				box.uLightPassed = 0;
				box.uMirrorReflected = 0;
				pMap->setBox(i, j, box);
			}
		}
		for(uint16 i=0; i<mapV; ++i) {
			for(uint16 j=0; j<mapH; ++j) {
				rfBox box = pMap->getBox(i, j);
				if(box.uAvailable) {
					if(box.uHasLight) {
						renderLight(i, j, box.light.fDirection);
					}
				}
			}
		}
		for(uint16 i=0; i<mapV; ++i) {
			for(uint16 j=0; j<mapH; ++j) {
				rfBox box = pMap->getBox(i, j);
				if(box.uHasMirror) {
					pMirrorSpr->setRotation(sora::DGR_RAD(box.MirrorDirection));
					pMirrorSpr->render(cx+box.BoxPosX+bsize/2, cy+box.BoxPosY+bsize/2);
				}
				if(box.uAvailable) {
					if(box.uHasLight) {
						pLightSourceSpr->render(cx+box.BoxPosX+bsize/2, cy+box.BoxPosY+bsize/2);
					}
				}
			}
		}
	}
	
	inline bool isInBox(int32 mx, int32 my, int32 x, int32 y, int32 w, int32 h) {
		if(mx > x && mx < x+w && my > y && y < y+h)
			return true;
		return false;
	}
	
	void rfMapRenderer::update(float32 dt) {
		if(!pMap) return;
		if(!pBoxSpr || !pBoxSprHL || !pMirrorSpr || !pLightSourceSpr) return;
		if(isWin() || isLose()) return;
		
		if(mouseDelay == 0) {
			if(CORE->keyDown(SORA_KEY_LBUTTON)) {
				float32 mx, my;
				CORE->getMousePos(&mx, &my);
							
				if(isInBox(mx, my, cx, cy, pMap->getMapWidth(), pMap->getMapHeight())) {
					float32 distx = mx-cx;
					float32 disty = my-cy;
				
					int32 wx = distx / pMap->getGlobalBoxSize();
					int32 wy = disty / pMap->getGlobalBoxSize();
								
					rfBox box = pMap->getBox(wy, wx);
					if(box.uHasLight) {
						box.light.fDirection += 360.f/pMap->getGlobalBoxEdgeNum();	
						box.light.fDirection = (int32)(box.light.fDirection) % 360;
						pMap->setBox(wy, wx, box);
						mouseDelay = 1;
						return;
					}
					
					if(!box.uHasMirror && (usedMirror < maxMirror || maxMirror == 0)) {
						box.uHasMirror = 1;
						usedMirror += 1;
					}
					else {
						box.MirrorDirection += 360.f/pMap->getGlobalBoxEdgeNum();
						box.MirrorDirection = (int32)(box.MirrorDirection) % 360;
					}
					pMap->setBox(wy, wx, box);
					
					mouseDelay = 1;
				}
			}
			else if(CORE->keyDown(SORA_KEY_RBUTTON)) {
				float32 mx, my;
				CORE->getMousePos(&mx, &my);
							
				if(isInBox(mx, my, cx, cy, pMap->getMapWidth(), pMap->getMapHeight())) {
					float32 distx = mx-cx;
					float32 disty = my-cy;
					
					int32 wx = distx / pMap->getGlobalBoxSize();
					int32 wy = disty / pMap->getGlobalBoxSize();
									
					rfBox box = pMap->getBox(wy, wx);
					if(box.uHasLight) {
						box.light.fDirection -= 360.f/pMap->getGlobalBoxEdgeNum();	
						box.light.fDirection = (int32)(box.light.fDirection) % 360;
						pMap->setBox(wy, wx, box);
						mouseDelay = 1;
						return;
					}
					
					if(box.uHasMirror == 1) {
						box.uHasMirror = 0;
						box.MirrorDirection = 360.f/pMap->getGlobalBoxEdgeNum()/2;
						if(maxMirror != 0)
							usedMirror -= 1;
						pMap->setBox(wy, wx, box);
					}
					mouseDelay = 1;
				}				
			}
		} else {
			if(mouseDelay < RF_MOUSE_DELAY)
				++mouseDelay;
			else {
				mouseDelay = 0;
			}
		}
		
		if(usedTime < maxTime) 
			usedTime += dt;
	}
	
	int rfMapRenderer::loadMap(const wchar_t* pstrMap) {		
		ulong32 size;
		void* pData = CORE->getResourceFile(pstrMap, size);
		if(!pData)
			return 0;

		int r = loadMapMem(pData, size);
		currentMap = pstrMap;
		free(pData);
		return r;
	}
	
	int rfMapRenderer::loadMapMem(void* pData, uint32 size) {
		pMap = new rfMap;
		if(!pMap->loadMapMem(pData, size)) {
			delete pMap;
			pMap = 0;
			return 0;
		}
		
		mapH = pMap->getMapBoxCountH();
		mapV = pMap->getMapBoxCountV();
		
		uint16 bsize = pMap->getGlobalBoxSize();
		
		if(pBoxSpr) pBoxSpr->setScale((float)bsize/pBoxSpr->getTextureWidth(), (float)bsize/pBoxSpr->getTextureHeight());
		if(pBoxSprHL) pBoxSprHL->setScale((float)bsize/pBoxSprHL->getTextureWidth(), (float)bsize/pBoxSprHL->getTextureHeight());
	
		usedTime = usedMirror = 0;

		return 1;
	}
	
	void rfMapRenderer::setBoxSprite(const wchar_t* pb, const wchar_t* pbhl) {
		pBoxSpr = CORE->createSprite(pb);
		pBoxSprHL = CORE->createSprite(pbhl);
		
		if(pBoxSpr) {
			pBoxSpr->setCenter(pBoxSpr->getTextureWidth()/2, pBoxSpr->getTextureHeight()/2);
			pBoxSpr->setBlendMode(BLEND_ZWRITE | BLEND_ALPHABLEND | BLEND_COLORMUL);
		}
		if(pBoxSprHL) {
			pBoxSprHL->setCenter(pBoxSprHL->getTextureWidth()/2, pBoxSprHL->getTextureHeight()/2);
			pBoxSprHL->setBlendMode(BLEND_ZWRITE | BLEND_ALPHABLEND | BLEND_COLORMUL);
		}
	}
	
	void rfMapRenderer::setMirrorSprite(const wchar_t* pm) {
		pMirrorSpr = CORE->createSprite(pm);
		if(pMirrorSpr)
			pMirrorSpr->setCenter(pMirrorSpr->getTextureWidth()/2, pMirrorSpr->getTextureHeight()/2);
	}
	
	void rfMapRenderer::setLightSourceSprite(const wchar_t* pstrLightSource, const wchar_t* pstrLight) {
		pLightSourceSpr = CORE->createSprite(pstrLightSource);
		pLightSpr = CORE->createSprite(pstrLight);
		
		if(pLightSpr) {
	//		pLightSpr->setCenter(pLightSpr->getTextureWidth()/2, pLightSpr->getTextureHeight()/2);
			pLightSpr->setBlendMode(BLEND_ZWRITE | BLEND_ALPHABLEND | BLEND_COLORMUL);
		}
		if(pLightSourceSpr) {
			pLightSourceSpr->setCenter(pLightSourceSpr->getTextureWidth()/2, pLightSourceSpr->getTextureHeight()/2);
			pLightSourceSpr->setBlendMode(BLEND_ZWRITE | BLEND_ALPHABLEND | BLEND_COLORMUL);
		}
		
	}
	
	uint32 rfMapRenderer::getWidth() {
		if(pMap)
			return pMap->getMapWidth();
		return 0;
	}
	
	uint32 rfMapRenderer::getHeight() {
		if(pMap)
			return pMap->getMapHeight();
		return 0;
	}
	
	uint16 rfMapRenderer::getMapH() {
		return mapH;
	}
	
	uint16 rfMapRenderer::getMapV() {
		return mapV;
	}
	
	void rfMapRenderer::renderLightInRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 lw) {
		sora::Rect4V rect;
		
		float32 fWidth = lw;
		float rotAng = (float)atan2f(y2-y1, x2-x1)-sora::D_PI_4;
		
		rect.x1 = x1; rect.y1 = y1;
		rect.x2 = x1+fWidth*cosf(rotAng); rect.y2 = y1+fWidth*sinf(rotAng);
		rect.x4 = x2; rect.y4 = y2;
		rect.x3 = x2+fWidth*cosf(rotAng); rect.y3 = y2+fWidth*sinf(rotAng);
				
		pLightSpr->render4V(rect.x1-fWidth/2*cosf(rotAng), rect.y1-fWidth/2*sinf(rotAng),
							rect.x2-fWidth/2*cosf(rotAng), rect.y2-fWidth/2*sinf(rotAng),
							rect.x3-fWidth/2*cosf(rotAng), rect.y3-fWidth/2*sinf(rotAng),
							rect.x4-fWidth/2*cosf(rotAng), rect.y4-fWidth/2*sinf(rotAng));
	}
	
	void rfMapRenderer::renderLight(uint16 v, uint16 h, float32 dir) {
		uint16 sv = v;
		uint16 sh = h;
		float32 sdir = dir;
	
		rfBox currBox = pMap->getBox(sv, sh);
		float32 length = pMap->getGlobalBoxSize();
		float32 currx = currBox.BoxPosX+length/2;
		float32 curry = currBox.BoxPosY+length/2;
				
		currx += length * cosf(sora::DGR_RAD(sdir));
		curry += length * sinf(sora::DGR_RAD(sdir));
		
		int32 wx = currx / length;
		int32 wy = curry / length;
		
		if(wx == sv && wy == sh) {
			currx += length * cosf(sora::DGR_RAD(sdir));
			curry += length * sinf(sora::DGR_RAD(sdir));
			
			wx = currx / length;
			wy = curry / length;
		}			
	
		while(wx < mapH && wy < mapV && wx >=0 && wy >= 0) {
			rfBox nextBox = pMap->getBox(wy, wx);
			
			if(nextBox.uMirrorReflected == 1)
				break;
			
			
			if(nextBox.uHasMirror == 1) {
				if(nextBox.uMirrorReflected == 0) {
					float32 mrf = sdir - nextBox.MirrorDirection;
					sdir += mrf*2;
					sdir = (int32)sdir % 360;
				
					nextBox.uMirrorReflected = 1;
				}
			} 
			renderLightInRect(currBox.BoxPosX+cx+length/2, currBox.BoxPosY+cy+length/2,
							  nextBox.BoxPosX+cx+length/2, nextBox.BoxPosY+cy+length/2, 6.f);
			
			float32 rl = sqrt((float)(nextBox.BoxPosY-currBox.BoxPosY)*(nextBox.BoxPosY-currBox.BoxPosY)+
							  (nextBox.BoxPosX-currBox.BoxPosX)*(nextBox.BoxPosX-currBox.BoxPosX));
			
			currBox = nextBox;
			currBox.uLightPassed = 1;
			pMap->setBox(wy, wx, currBox);
			
			currx += rl * cosf(sora::DGR_RAD(sdir));
			curry += rl * sinf(sora::DGR_RAD(sdir));
			
			wx = currx / pMap->getGlobalBoxSize();
			wy = curry / pMap->getGlobalBoxSize();
			
			if(currx < 0 || curry < 0 || currx > mapH*length || curry > mapV*length)
				break;
		}
		renderLightInRect(currBox.BoxPosX+cx+length/2, currBox.BoxPosY+cy+length/2,
						  currx+cx+200*cosf(sora::DGR_RAD(sdir)), curry+cy+200*sinf(sora::DGR_RAD(sdir)), 6.f);
	}
	
	std::wstring rfMapRenderer::getCurrentMap() const {
		return currentMap;
	}
	
	
} // namespace rf