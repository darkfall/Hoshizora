/*
 *  meadBulletManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadBulletManager.h"
#include "json/json.h"
#include "llexer/llexer.h"
#include "SoraCore.h"

namespace mead {

	meadBulletManager::meadBulletManager() {
		mScreenRect.Set(-64.f, -64.f, sora::SORA->getScreenWidth()+64.f, sora::SORA->getScreenHeight()+64.f);
	}
	
	meadBulletManager::~meadBulletManager() {
		clearBulletList(true);
		
		BULLET_SPRITE_MAP::iterator itSpr = mBulletSprites.begin();
		for(itSpr; itSpr != mBulletSprites.end(); ++itSpr) {
			delete itSpr->second;
			itSpr->second = NULL;
		}
	}
	
	void meadBulletManager::clearBulletList(bool bForce) {
		BULLET_LIST::iterator itBullet = mBullets.begin();
		while(itBullet != mBullets.end()) {
			if(bForce) {
				delete *itBullet;
				*itBullet = 0;
			} else {
				if(!(*itBullet)->isImmortal) {
					delete *itBullet;
					*itBullet = 0;
				}
			}
			++itBullet;
		}
		mBullets.clear();
		mBulletBuffer.clear();
	}
	
	void meadBulletManager::loadBulletConfig(const std::wstring& file) {
		ulong32 size;
		void* data = sora::SORA->getResourceFile(file, size);
		if(data) {
			llexer* lexer = new llexer;
			if(lexer->loadFileMem(data, size) == LEX_NO_ERROR) {
				Token tkNextToken = lexer->getNextToken();
				while(tkNextToken != TOKEN_TYPE_END_OF_STREAM) {
					if(tkNextToken == TOKEN_TYPE_IDENT) {
						if(strcmpnocase(lexer->getCurrLexeme(), "BulletData") == 0) {
							Token ctoken = lexer->getNextToken();
							if(ctoken == TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE) {
								BulletConfigNode node;
								uint32 bulletId;
								
								tkNextToken = lexer->getNextToken();
								while(tkNextToken != TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE) {
									const char* lexeme = lexer->getCurrLexeme();
									if(strcmpnocase(lexeme, "TexId") == 0) {
										lexer->getNextToken();
										lexer->getNextToken();
										
										node.mTextureId = atoi(lexer->getCurrLexeme());
									} else if(strcmpnocase(lexeme, "TexRect") == 0) {
										lexer->getNextToken();
										if(lexer->getNextToken() == TOKEN_TYPE_DELIM_OPEN_PAREN) {
											hgeRect rect;
											lexer->getNextToken(); rect.x1 = atoi(lexer->getCurrLexeme());
											lexer->getNextToken(); rect.y1 = atoi(lexer->getCurrLexeme());
											lexer->getNextToken(); rect.x2 = atoi(lexer->getCurrLexeme());
											lexer->getNextToken(); rect.y2 = atoi(lexer->getCurrLexeme());
											
											node.mTextureRect = rect;
											
											lexer->getNextToken();
										}
									} else if(strcmpnocase(lexeme, "ShootColor") == 0) {
										lexer->getNextToken();
										if(lexer->getNextToken() == TOKEN_TYPE_DELIM_OPEN_PAREN) {
											sora::SoraColorRGBA color;
											lexer->getNextToken(); color.r = atoi(lexer->getCurrLexeme())/255.f;
											lexer->getNextToken(); color.g = atoi(lexer->getCurrLexeme())/255.f;
											lexer->getNextToken(); color.b = atoi(lexer->getCurrLexeme())/255.f;
											color.a = 1.f;
											
											node.mShootColor = color;
											
											lexer->getNextToken();
										}
									} else if(strcmpnocase(lexeme, "BulletId") == 0) {
										lexer->getNextToken();
										lexer->getNextToken();
										
										bulletId = atoi(lexer->getCurrLexeme());
									}
									tkNextToken = lexer->getNextToken();
									if(tkNextToken == TOKEN_TYPE_END_OF_STREAM)
										break;
								}
								
								mBulletConfigs[bulletId] = node;
							}
						}
					}
					tkNextToken = lexer->getNextToken();
				}
				
				delete lexer;
			}
		}
	}
	
	void meadBulletManager::loadBulletSprite(uint32 texId, const std::wstring& sprFile) {
		sora::SoraSprite* pSpr = sora::SORA->createSprite(sprFile);
		if(pSpr) {
			mBulletSprites[texId] = pSpr;
		}
	}
	
	void meadBulletManager::setScreenRect(const hgeRect& rect) {
		mScreenRect = rect;
	}

	ulong32 meadBulletManager::shootBullet(float32 posx, float32 posy, float32 speed, float32 direction, uint32 bulletId, uint32 delay) {
		BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bulletId);
		if(itConfig != mBulletConfigs.end()) {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
			if(itSprite != mBulletSprites.end()) {
				meadBullet* bullet = new meadBullet(posx, posy, speed, direction, itConfig->second.mTextureId, delay, itConfig->second.mTextureRect);
				bullet->screenRect = mScreenRect;
				bullet->bulletId = bulletId;
				
				mBullets.push_back(bullet);
				mBulletBuffer[(ulong32)bullet] = bullet;
				return (ulong32)bullet;
			}
		}
		return 0;
	}
	
	ulong32 meadBulletManager::shootBulletEx(float32 posx, float32 posy, float32 speed, float32 direction, float32 directionIncremental, uint32 bulletId, uint32 shootTime) {
		BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bulletId);
		if(itConfig != mBulletConfigs.end()) {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
			if(itSprite != mBulletSprites.end()) {
				meadBullet* bullet = new meadBullet(posx, posy, speed, direction, itConfig->second.mTextureId, shootTime, itConfig->second.mTextureRect);
				bullet->screenRect = mScreenRect;
				bullet->incrementalDirection = directionIncremental;
				bullet->bulletId = bulletId;

				mBullets.push_back(bullet);
				mBulletBuffer[(ulong32)bullet] = bullet;
				return (ulong32)bullet;
			}
		}
		return 0;		
	}
	
	ulong32 meadBulletManager::shootBulletDirect(void* bullet, uint32 bulletId, uint32 shootTime) {
		if(bullet == NULL)
			return 0;
		
		BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bulletId);
		if(itConfig != mBulletConfigs.end()) {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
			if(itSprite != mBulletSprites.end()) {
				meadBullet* nbullet = new meadBullet;
				memcpy(nbullet, bullet, sizeof(meadBullet));
				
				nbullet->texId = itConfig->second.mTextureId;
				nbullet->texRect = itConfig->second.mTextureRect;
				nbullet->shootTime = shootTime;
				
				mBullets.push_back(nbullet);
				mBulletBuffer[(ulong32)nbullet] = nbullet;
				return (ulong32)nbullet;
			}
		}
		return 0;
	}
	
	ulong32 meadBulletManager::shootLaser(float32 posx, float32 posy, float32 speed, float32 direction, float32 length, uint32 bulletId, uint32 shootTime) {
		BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bulletId);
		if(itConfig != mBulletConfigs.end()) {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
			if(itSprite != mBulletSprites.end()) {
				meadBullet* bullet = new meadBullet(posx, posy, speed, direction, itConfig->second.mTextureId, shootTime, itConfig->second.mTextureRect);
				bullet->screenRect = mScreenRect;
				bullet->bulletId = bulletId;
				
				bullet->setLaser(length);
				
				mBullets.push_back(bullet);
				mBulletBuffer[(ulong32)bullet] = bullet;
				return (ulong32)bullet;
			}
		}
		return 0;
	}
	
	ulong32 meadBulletManager::shootLaserEx(float32 x, float32 y, float32 width, float32 direction, uint32 bulletId, uint32 shootTime, uint32 lastTime) {
		BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bulletId);
		if(itConfig != mBulletConfigs.end()) {
			BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
			if(itSprite != mBulletSprites.end()) {
				meadBullet* bullet = new meadBullet(x, y, 0.f, direction, itConfig->second.mTextureId, shootTime, itConfig->second.mTextureRect);
				bullet->screenRect = mScreenRect;
				bullet->bulletId = bulletId;
				
				bullet->setLaser(width, lastTime);
				
				mBullets.push_back(bullet);
				mBulletBuffer[(ulong32)bullet] = bullet;
				return (ulong32)bullet;
			}
		}
		return 0;
	}
	
	void meadBulletManager::update() {
		BULLET_LIST::iterator itBullet = mBullets.begin();
		while(itBullet != mBullets.end()) {
			meadBullet* bullet = (*itBullet);
			if(bullet) {
				bullet->update();
				if(bullet->isNeedDestroy()) {
					delete (*itBullet);
					(*itBullet) = 0;
					mBulletBuffer.erase((ulong32)(*itBullet));

					itBullet = mBullets.erase(itBullet);
					continue;
				}
				
				sora::SoraSprite* spr = mBulletSprites[bullet->texId];
				bullet->render(spr);
			}
			++itBullet;
		}

	}
	
	uint32 meadBulletManager::getSize() {
		return mBullets.size();
	}
	
	void meadBulletManager::setScreenRectAABB(float32 x, float32 y, float32 x2, float32 y2) {
		mScreenRect.Set(x, y, x2, y2);
	}
	
	meadBullet* meadBulletManager::getBullet(ulong32 handle) {
		return mBulletBuffer[handle];
	}
	
	void* meadBulletManager::getBulletDirect(ulong32 handle) {
		return (void*)mBulletBuffer[handle];
	}
	
	bool meadBulletManager::isBulletValid(ulong32 handle) {
		if(mBulletBuffer.find(handle) != mBulletBuffer.end())
			return true;
		return false;
	}
	
	void meadBulletManager::setBulletId(ulong32 handle, uint32 bid) {
		if(isBulletValid(handle)) {
			meadBullet* bullet = (meadBullet*)handle;
			BULLET_CONFIG_MAP::iterator itConfig = mBulletConfigs.find(bid);
			if(itConfig != mBulletConfigs.end()) {
				BULLET_SPRITE_MAP::iterator itSprite = mBulletSprites.find(itConfig->second.mTextureId);
				if(itSprite != mBulletSprites.end()) {
					bullet->texId = itConfig->second.mTextureId;
					bullet->texRect = itConfig->second.mTextureRect;
				}
			}
		}
	}

} // namespace mead