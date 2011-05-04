/*
 *  bulletLuaHelper.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "bulletLuaHelper.h"

namespace mead {

	meadBulletManager* g_bulletManager = NULL;
	
	void globalBulletManagerInit() {
		if(g_bulletManager == NULL) {
			g_bulletManager = new meadBulletManager;
		}
	}
	
	void globalBulletManagerUpdate() {
		if(g_bulletManager)
			g_bulletManager->update();
	}
	
	ulong32 shootBullet(float32 posx, float32 posy, float32 speed, float32 direction, uint32 bulletId, uint32 shootTime) {
		return g_bulletManager->shootBullet(posx, posy, speed, direction, bulletId, shootTime);
	}
	
	ulong32 shootBulletEx(float32 posx, float32 posy, float32 speed, float32 direction, float32 directionIncremental, uint32 bulletId, uint32 shootTime) {
		return g_bulletManager->shootBulletEx(posx, posy, speed, direction, directionIncremental, bulletId, shootTime);
	}
	
	ulong32 shootBulletDirect(void* bullet, uint32 bulletId, uint32 shootTime) {
		return g_bulletManager->shootBulletDirect(bullet, bulletId, shootTime);
	}
	
	ulong32 shootLaser(float32 posx, float32 posy, float32 speed, float32 direction, float32 length, uint32 bulletId, uint32 shootTime) {
		return g_bulletManager->shootLaser(posx, posy, speed, direction, length, bulletId, shootTime);
	}
	
	ulong32 shootLaserEx(float32 x, float32 y, float32 dx, float32 dy, uint32 bulletId, uint32 shootTime, uint32 lastTime) {
		return g_bulletManager->shootLaserEx(x, y, dx, dy, bulletId, shootTime, lastTime);
	}
	
	void loadBulletConfig(const std::wstring& config) {
		g_bulletManager->loadBulletConfig(config);
	}
	
	void loadBulletSprite(uint32 texId, const std::wstring& sprFile) {
		g_bulletManager->loadBulletSprite(texId, sprFile);
	}
	
	void clearBulletList(bool force) {
		g_bulletManager->clearBulletList(force);
	}
	
	void setScreenRect(float32 x, float32 y, float32 x1, float32 y1) {
		g_bulletManager->setScreenRectAABB(x, y, x1, y1);
	}
	
	bool isBulletValid(ulong32 handle) {
		return g_bulletManager->isBulletValid(handle);
	}
	
	void setBulletId(ulong32 handle, uint32 bid) {
		g_bulletManager->setBulletId(handle, bid);
	}
	
	uint32 getBulletSize() {
		return g_bulletManager->getSize();
	}
	
	void* getBulletDirect(ulong32 handle) {
		return g_bulletManager->getBulletDirect(handle);
	}
} // namespace mead