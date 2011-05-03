/*
 *  bulletLuaHelper.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef BULLET_LUA_HELPER_H_
#define BULLET_LUA_HELPER_H_

#include "SoraPlatform.h"
#include "meadBulletManager.h"

namespace mead {
	
	/*
	 global bullet functions
	 */	
	void globalBulletManagerInit();
	void globalBulletManagerUpdate();
	
	ulong32 shootBullet(float32 posx, float32 posy, float32 speed, float32 direction, uint32 bulletId, uint32 shootTime);	
	ulong32 shootBulletEx(float32 posx, float32 posy, float32 speed, float32 direction, float32 directionIncremental, uint32 bulletId, uint32 shootTime);	
	ulong32 shootBulletDirect(void* bullet, uint32 bulletId, uint32 shootTime);	
	ulong32 shootLaser(float32 posx, float32 posy, float32 speed, float32 direction, float32 length, uint32 bulletId, uint32 shootTime);
	ulong32 shootLaserEx(float32 x, float32 y, float32 dx, float32 dy, uint32 bulletId, uint32 shootTime, uint32 lastTime);
	
	void loadBulletConfig(const std::wstring& config);	
	void loadBulletSprite(uint32 texId, const std::wstring& sprFile);	
	void clearBulletList(bool force);	
	void setScreenRect(float32 x, float32 y, float32 x1, float32 y1);	
	bool isBulletValid(ulong32 handle);	
	void setBulletId(ulong32 handle, uint32 bid);	
	uint32 getBulletSize();	
	void* getBulletDirect(ulong32 handle);	
	
	static void setBulletSpeed(ulong32 handle, float32 speed) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setSpeed(speed);
	}
	
	static void setBulletPosition(ulong32 handle, float32 x, float32 y) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setPosition(x, y);
	}
	
	static void setBulletOriginPoint(ulong32 handle, float32 x, float32 y) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setOriginPoint(x, y);
	}
	
	static void setBulletIncrementalSpeed(ulong32 handle, float32 inc) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setIncrementalSpeed(inc);
	}
	
	static void setBulletIncrementalSpeedEx(ulong32 handle, float32 incx, float32 incy) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setIncrementalSpeedEx(incx, incy);
	}
	
	static void setBulletIncrementalDirection(ulong32 handle, float32 dir) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setIncrementalDirection(dir);
	}
	
	static void setBulletCustomValue1(ulong32 handle, float32 cval1) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setCustomValue1(cval1);
	}
	
	static void setBulletCustomValue2(ulong32 handle, float32 cval2) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setCustomValue2(cval2);
	}
	
	static void setBulletReflectionTimes(ulong32 handle, uint32 times) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setReflectionTimes(times);
	}
	
	static void setBulletDirection(ulong32 handle, float32 dir) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setDirection(dir);
	}
	
	static float32 getBulletSpeed(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->speed;
		return 0.f;
	}
	
	static float32 getBulletSpeedX(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->speedX;
		return 0.f;
	}
	
	static float32 getBulletSpeedY(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->speedY;
		return 0.f;
	}
	
	static float32 getBulletDirection(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->direction;
		return 0.f;
	}
	
	static float32 getBulletPositionX(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->positionX;
		return 0.f;
	}
	
	static float32 getBulletPositionY(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->positionY;
		return 0.f;
	}
	
	static float32 getBulletOriginPointX(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->originPointX;
		return 0.f;
	}
	
	static float32 getBulletOriginPointY(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->originPointY;
		return 0.f;
	}
	
	static uint32 getBulletReflectionTimes(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->reflectionTimes;
		return 0;
	}
	
	static float32 getBulletIncrementalSpeed(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->incrementalSpeed;
		return 0.f;
	}
	
	static float32 getBulletIncrementalSpeedX(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->incrementalSpeedX;
		return 0.f;
	}
	
	static float32 getBulletIncrementalSpeedY(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->incrementalSpeedY;
		return 0.f;
	}
	
	static float32 getBulletIncrementalDirection(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->incrementalDirection;
		return 0.f;
	}
	
	static float32 getBulletCustomValue1(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->customValue1;
		return 0.f;
	}
	
	static float32 getBulletCustomValue2(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->customValue2;
		return 0.f;
	}
	
	static void removeBullet(ulong32 handle, bool effect) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) {
			bullet->die(effect);
		}
	}
	
	static uint32 getBulletId(ulong32 handle) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) return bullet->bulletId;
		return 0;
	}
	
	static void setBulletDownReflection(ulong32 handle, bool flag) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setDownReflection(flag);
	}
	
	static void setBulletAlphaBlend(ulong32 handle, bool flag) {
		meadBullet* bullet = (meadBullet*)handle;
		if(bullet) bullet->setAlphaBlend(flag);
	}
	
} // namespace mead

#endif // BULLET_LUA_HELPER_H_
