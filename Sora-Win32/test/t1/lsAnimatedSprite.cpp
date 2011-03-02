/*
 *  lsAnimatedSprite.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */


#include "lsAnimatedSprite.h"

#include "SoraCore.h"

namespace ls {
	
#define SORACORE sora::SoraCore::Instance() 

	lsAnimatedSprite::lsAnimatedSprite(const SoraWString& path) {
		load(path);
	}
	
	lsAnimatedSprite::lsAnimatedSprite(): pSpr(0) {
	}
	
	bool lsAnimatedSprite::load(const SoraWString& path) {
		ulong32 size;
		void* pData = SORACORE->getResourceFile(path, size);
		
		pAnm = lAnmPacker::unpack(pData, size);
		if(pAnm) {
			sora::HSORATEXTURE tex = SORACORE->createTextureFromMem(pAnm->getTexData(), pAnm->getTexSize());
			if(tex) {
				pSpr = SORACORE->createSpriteTex(tex);
				pAnm->freeTexData();
				if(!pSpr) {
					delete pAnm;
					pAnm = 0;
					return false;
				}
			}
		} else {
			return false;
		}
		return true;
	}
	
	void lsAnimatedSprite::render(float32 x, float32 y) {
		if(valid())
			pSpr->render();
	}
	
	void lsAnimatedSprite::update(float32 dt) {
		if(valid()) {
			pAnm->update(dt);
			LANM_TEX tex = pAnm->getCurrTex();
			pSpr->update(dt);
			pSpr->setTextureRect(tex.tx, tex.ty, tex.tw, tex.th);
		}
	}
	
	bool lsAnimatedSprite::valid() {
		return (pAnm != NULL && pSpr != NULL);
	}
	
	void lsAnimatedSprite::play(const char* name, bool bLoop) {
		if(valid())
			pAnm->play(name, bLoop);
	}
	
	void lsAnimatedSprite::play() {
		if(valid())
			pAnm->play();
	}

} // namespace ls