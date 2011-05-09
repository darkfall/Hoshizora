/*
 *  meadBullet.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_BULLET_H_
#define MEAD_BULLET_H_

#include "hgerect.h"
#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraColor.h"

#include <string>

namespace mead {
	
	struct meadBullet {
		// texId in the bullet manager
		int32 texId;
		// bulletid
		uint32 bulletId;
		// texture rect for the bullet
		hgeRect texRect;
		hgeRect screenRect;
		
		int32 getBulletId() { return bulletId; }
		
		// original point, default (0, 0)
		// button direction would be calculated depend on this
		float32 originPointX, originPointY;
		
		// current position
		float32 positionX, positionY;
		float32 getPositionX() { return positionX; }
		float32 getPositionY() { return positionY; }
		void setPosition(float32 x, float32 y) { 
			positionX = x;
			positionY = y;
		}
		
		// current speed value
		float32 speed;
		// current speed vector, use for faster calculation
		float32 speedX, speedY;
		
		float32 getSpeed() { return speed; }
		
		// the direction that the bullet faces
		float32 direction;
		
		float32 getDirection() { return direction; }
		
		
		void setDirection(float32 direction) {
			this->direction = direction;
			speedX = speed * cosf(direction);
			speedY = speed * sinf(direction);
		}
		
		// change the origin point to xx
		void setOriginPoint(float32 opx, float32 opy) {
			originPointX = opx;
			originPointY = opy;
			
			setDirection(atan2f(positionY-opy, positionX-opx));
			
			if(incrementalSpeed != 0.f) {
				setIncrementalSpeed(speed);
			}
		}
		float32 getOriginPointX() { return originPointX; }
		float32 getOriginPointY() { return originPointY; }
		
		void setSpeed(float32 speed) {
			this->speed = speed;
			speedX = speed * cosf(direction);
			speedY = speed * sinf(direction);
		}
		
		// incremental speed for the bullet
		// speed would increment this value every frame
		float32 incrementalSpeed;
		// incremental speed vector, for faster calculation
		float32 incrementalSpeedX, incrementalSpeedY;
		
		void setIncrementalSpeed(float32 speed) {
			incrementalSpeed = speed;
			incrementalSpeedX = incrementalSpeed * cosf(direction);
			incrementalSpeedY = incrementalSpeed * sinf(direction);
		}
		void setIncrementalSpeedEx(float32 sx, float32 sy) {
			incrementalSpeedX = sx;
			incrementalSpeedY = sy;
			incrementalSpeed = sqrtf(incrementalSpeedX*incrementalSpeedX + incrementalSpeedY*incrementalSpeedY);
		}
		float32 getIncrementalSpeed() { return incrementalSpeed; }
		
		// incremental direction
		// direction would increment this value every frame
		float32 incrementalDirection;
		
		void setIncrementalDirection(float32 direction) { incrementalDirection = direction; }
		float32 getIncrementalDirection() { return incrementalDirection; }
	
		// bullet states;
		// can the bullet collide others
		bool isCollision;
		// can the bullet graze by player
		bool isGraze;
		// is the bullet be drawn on the screen
		bool isShow;
		// is the bullet died or not, died bullet would be removed from the screen every frame
		bool isDied;
		// is the bullet die after clear screen
		bool isImmortal;
		
		// custom value for the bullet, use in the script
		float32 customValue1;
		float32 customValue2;
		
		float32 getCustomValue1() { return customValue1; }
		float32 getCustomValue2() { return customValue2; }
		void setCustomValue1(float32 val) { customValue1 = val; }
		void setCustomValue2(float32 val) { customValue2 = val; }
		
		// reflection times
		uint32 currReflectionTimes;
		uint32 reflectionTimes;
		bool isDownReflection; 
		
		void setReflectionTimes(uint32 times) { reflectionTimes = times; }
		uint32 getReflectionTimes() { return reflectionTimes; }
		
		void setDownReflection(bool flag) { isDownReflection = flag; }
				
		uint32 currShootTime;
		uint32 shootTime;
		
		uint32 currDiedEffectTime;
		uint32 diedEffectTime;
		
		// is Alpha Blending
		bool isAlphaBlend;
		void setAlphaBlend(bool flag) {
			isAlphaBlend = flag;
		}
		
		bool isCustomCollid;
		float32 collisionRadius;
		
		meadBullet(): 
			positionX(0.f), positionY(0.f), speed(0.f), direction(0.f), texId(0), shootTime(0),
			originPointX(0.f), originPointY(0.f), speedX(0.f), speedY(0.f), 
			incrementalSpeed(0.f), incrementalSpeedX(0.f), incrementalSpeedY(0.f),
			incrementalDirection(0.f), 
			customValue1(0.f), customValue2(0.f),
			currReflectionTimes(0), reflectionTimes(0),
			currShootTime(0), currPauseTime(0), pauseTime(0),
			diedEffectTime(10), currDiedEffectTime(0),
			isLaser(false), laserLength(0.f),
			isCustomCollid(false), collisionRadius(0.f), 
			isPlayerBullet(false),
			isCollision(false), isGraze(true), isShow(true), isDied(false), isImmortal(false), isDownReflection(false), isAlphaBlend(true) {
		}
		
		meadBullet(float32 posx, float32 posy, float32 s, float32 d, uint32 tid, uint32 st, const hgeRect& rect): 
			positionX(posx), positionY(posy), speed(s), direction(d), texId(tid), texRect(rect), shootTime(st),
			originPointX(0.f), originPointY(0.f), speedX(0.f), speedY(0.f), 
			incrementalSpeed(0.f), incrementalSpeedX(0.f), incrementalSpeedY(0.f),
			incrementalDirection(0.f), 
			customValue1(0.f), customValue2(0.f),
			currReflectionTimes(0), reflectionTimes(0),
			currShootTime(0), currPauseTime(0), pauseTime(0),
			diedEffectTime(10), currDiedEffectTime(0),
			isLaser(false), laserLength(0.f),
			isCustomCollid(false), collisionRadius(0.f),
			isPlayerBullet(false),
			isCollision(false), isGraze(true), isShow(true), isDied(false), isImmortal(false), isDownReflection(false), isAlphaBlend(true)  {
				setSpeed(speed);
				if(st == 0)
					isCollision = true;
				collisionRadius = (texRect.x2 + texRect.y2) / 4;
		}
				
		inline void update() {
			if(shootTime != 0) {
				++currShootTime;
				if(currShootTime >= shootTime) {
					shootTime = 0;
					currShootTime = 0;
					isCollision = true;
				}
			}
			
			if(pauseTime != 0) {
				++currPauseTime;
				if(currPauseTime >= pauseTime)
					pauseTime = 0;
			}
			
			positionX += speedX;
			positionY -= speedY;
			
			if(incrementalSpeed != 0.f) {
				speedX += cosf(direction) * incrementalSpeed;
				speedY += sinf(direction) * incrementalSpeed;
			}
			
			if(incrementalDirection != 0.f) {
				direction += incrementalDirection;
				setDirection(direction);
			}
			
			if(!isDied) {
				if(isLaser) {
					if(laserLastTime != 0) {
						++currLaserLastTime;
						if(currLaserLastTime >= laserLastTime) {
							isDied = true;
						}
					} else {
						float32 laserPosX = positionX - laserLength * cosf(direction);
						float32 laserPosY = positionY + laserLength * sinf(direction);
						if(!screenRect.TestPoint(laserPosX, laserPosY)) {
							isDied = true;
							currDiedEffectTime = diedEffectTime;
						}
					}
				} else
				if(!screenRect.TestPoint(positionX, positionY)) {
					if(reflectionTimes == 0 || (!isDownReflection && positionY >= screenRect.y2)) {
						isDied = true;
						currDiedEffectTime = diedEffectTime;
					} else {
						if(positionX < screenRect.x1 || positionX > screenRect.x2)
							speedX = -speedX;
						else
							speedY = -speedY;
						direction = atan2f(speedY, speedX);
						
						++currReflectionTimes;
						if(currReflectionTimes >= reflectionTimes)
							reflectionTimes = 0;
					}
				}
			} else {
				++currDiedEffectTime;
			}
		}
		
		inline void die(bool effect) {
			isDied = true;
			if(!effect)
				currDiedEffectTime = diedEffectTime;
		}
		
		inline bool isNeedDestroy() {
			return (isDied && currDiedEffectTime > diedEffectTime);
		}
		
		bool isLaser;
		float32 laserLength, laserScale;
		float32 laserWidthScale;
		uint32 currLaserLastTime, laserLastTime;
		
		inline void setLaser(float32 length) {
			isLaser = true;
			laserLength = length;
			
			currLaserLastTime = laserLastTime = 0;
			
			laserScale = length/texRect.y2;
		}
		
		inline void setLaser(float32 width, uint32 lt) {
			isLaser = true;
			laserLength = 0.f;
			
			laserWidthScale = width / texRect.x2;
			
			currLaserLastTime = 0;
			laserLastTime = lt;

			laserScale = 600 / texRect.y2;
		}
		
		bool isPlayerBullet;
		
		inline bool collisionDetect(float32 x, float32 y) {
			return false;
		}
		
		inline void render(sora::SoraSprite* spr) {
			spr->setTextureRect(texRect.x1, texRect.y1, texRect.x2, texRect.y2);
			if(!isLaser) {
				spr->setCenter(texRect.x2/2, texRect.y2/2);
				spr->setRotation(-direction+sora::DGR_RAD(90));
				spr->setColor(0xFFFFFFFF);
				if(shootTime == 0) {
					if(!isDied) {
						spr->setBlendMode(BLEND_COLORMUL | isAlphaBlend?BLEND_ALPHABLEND:BLEND_ALPHAADD | BLEND_NOZWRITE);
						spr->setScale(1.f, 1.f);
					} else {
						spr->setBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
						sora::SoraColorRGBA color = spr->getColor();
						color.a = 1.f - (float32)currDiedEffectTime/diedEffectTime;
						spr->setColor(color.GetHWColor());
					}
				} else {
					spr->setBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
					float32 scale = 1.f + (2.f - (float32)currShootTime/shootTime * 2);
					spr->setScale(scale, scale);
				}
				spr->render(positionX, positionY);
			} else {
				if(laserLength != 0.f) {
					spr->setBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
					spr->setCenter(texRect.x2/2, texRect.y2/2);
					spr->setRotation(-direction+sora::DGR_RAD(90));
					spr->setColor(0xFFFFFFFF);
					if(shootTime == 0) {
						if(!isDied)
							spr->setScale(1.f, laserScale);
						else {
							spr->setScale(1.f, laserScale*(float32)currDiedEffectTime/diedEffectTime);
						}
					} else {
						spr->setScale(1.f, laserScale*(float32)currShootTime/shootTime);
					}
					spr->render(positionX, positionY);
				} else {
					spr->setBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
					spr->setCenter(texRect.x2/2, 0.f);
					spr->setRotation(-direction+sora::DGR_RAD(90));
					spr->setColor(0xFFFFFFFF);
					if(shootTime != 0) {
						spr->setScale(0.1f, laserScale);
					} else {
						if(!isDied)
							spr->setScale(laserWidthScale, laserScale);
						else {
							spr->setScale(laserWidthScale - (float32)currDiedEffectTime/diedEffectTime * laserWidthScale, laserScale);
						}
					}
					spr->render(positionX, positionY);
				}
			}
		}
		
		uint32 pauseTime;
		uint32 currPauseTime;
		
		void pause(uint32 time) {
			pauseTime = time;
			currPauseTime = 0;
		}
	};
	
} // namespace mead


#endif // MEAD_BULLET_H_
