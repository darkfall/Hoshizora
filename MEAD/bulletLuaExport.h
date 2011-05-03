/*
 *  luaExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_BULLET_LUA_EXPORT_H_
#define MEAD_BULLET_LUA_EXPORT_H_

#include "LuaPlusHelper.h"
#include "luaclass.h"
#include "bulletLuaHelper.h"
#include "meadBossManager.h"
#include "SoraGlobalValues/GlobalValuesLuaExport.h"

namespace mead {
	
	static void exportBulletManager(LuaPlus::LuaState* state) {
		LuaClass<meadBulletManager>(state)
			.create("BulletManager")
			.destroy("free")
			.def("shootBullet", &meadBulletManager::shootBullet)
			.def("shootBulletEx", &meadBulletManager::shootBulletEx)
			.def("shootLaser", &meadBulletManager::shootLaser)
			.def("shootLaserEx", &meadBulletManager::shootLaserEx)
			.def("shootBulletDirect", &meadBulletManager::shootBulletDirect)
			.def("loadBulletConfig", &meadBulletManager::loadBulletConfig)
			.def("loadBulletSprite", &meadBulletManager::loadBulletSprite)
			.def("update", &meadBulletManager::update)
			.def("clearBulletList", &meadBulletManager::clearBulletList)
			.def("setScreenRect", &meadBulletManager::setScreenRectAABB)
			.def("getSize", &meadBulletManager::getSize)
			.def("isBulletValid", &meadBulletManager::isBulletValid)
			.def("setBulletId", &meadBulletManager::setBulletId)
			.def("getBullet", &meadBulletManager::getBulletDirect);
		
		
		LuaClass<meadBullet>(state)
			.create("MeadBullet")
			.destroy("free")
			.def("getPositionX", &meadBullet::getPositionX)
			.def("getPositionY", &meadBullet::getPositionY)
			.def("setPosition", &meadBullet::setPosition)
			.def("getOriginPointX", &meadBullet::getOriginPointX)
			.def("getOriginPointY", &meadBullet::getOriginPointY)
			.def("setOriginPoint", &meadBullet::setOriginPoint)
			.def("setSpeed", &meadBullet::setSpeed)
			.def("getSpeed", &meadBullet::getSpeed)
			.def("setDirection", &meadBullet::setDirection)
			.def("getDirection", &meadBullet::getDirection)
			.def("getIncrementalSpeed", &meadBullet::getIncrementalSpeed)
			.def("setIncrementalSpeed", &meadBullet::setIncrementalSpeed)
			.def("setIncrementalSpeedEx", &meadBullet::setIncrementalSpeedEx)
			.def("getIncrementalDirection", &meadBullet::getIncrementalDirection)
			.def("setIncrementalDirection", &meadBullet::setIncrementalDirection)
			.def("getCustomValue1", &meadBullet::setCustomValue1)
			.def("getCustomValue2", &meadBullet::setCustomValue2)
			.def("setCustomValue1", &meadBullet::getCustomValue1)
			.def("setCustomValue2", &meadBullet::getCustomValue2)
			.def("setReflectionTimes", &meadBullet::setReflectionTimes)
			.def("getReflectionTimes", &meadBullet::getReflectionTimes)
			.def("getBulletId", &meadBullet::getBulletId);
		
		LuaModule(state)
			.def("setBulletPos", &setBulletPosition)
			.def("setBulletSpeed", &setBulletSpeed)
			.def("setBulletOriginPoint", &setBulletOriginPoint)
			.def("setBulletIncrementalSpeed", &setBulletIncrementalSpeed)
			.def("setBulletIncrementalSpeedEx", &setBulletIncrementalSpeedEx)
			.def("setBulletIncrementalDirection", &setBulletIncrementalDirection)
			.def("setBulletCustomValue1", &setBulletCustomValue1)
			.def("setBulletCustomValue2", &setBulletCustomValue2)
			.def("setBulletReflectionTimes", &setBulletReflectionTimes)
			.def("setBulletDirection", &setBulletDirection)
			.def("getBulletSpeed", &getBulletSpeed)
			.def("getBulletSpeedX", &getBulletSpeedX)
			.def("getBulletSpeedY", &getBulletSpeedY)
			.def("getBulletDirection", &getBulletDirection)
			.def("getBulletOriginPointX", &getBulletOriginPointX)
			.def("getBulletOriginPointY", &getBulletOriginPointY)
			.def("getBulletReflectionTimes", &getBulletReflectionTimes)
			.def("getBulletIncrementalSpeed", &getBulletIncrementalSpeed)
			.def("getBulletIncrementalSpeedX", &getBulletIncrementalSpeedX)
			.def("getBulletIncrementalSpeedY", &getBulletIncrementalSpeedY)
			.def("getBulletCustomValue1", &getBulletCustomValue1)
			.def("getBulletCustomValue2", &getBulletCustomValue2)
			.def("getBulletPosX", &getBulletPositionX)
			.def("getBulletPosY", &getBulletPositionY)
			.def("removeBullet", &removeBullet)
			.def("getBulletId", &getBulletId)
			// global bullet manager function
			.def("globalBulletManagerInit", &globalBulletManagerInit)
			.def("globalBulletManagerUpdate", &globalBulletManagerUpdate)
			.def("shootBullet", &shootBullet)
			.def("shootBulletEx", &shootBulletEx)
			.def("shootLaser", &shootLaser)
			.def("shootLaserEx", &shootLaserEx)
			.def("shootBulletDirect", &shootBulletDirect)
			.def("loadBulletConfig", &loadBulletConfig)
			.def("loadBulletSprite", &loadBulletSprite)
			.def("clearBulletList", &clearBulletList)
			.def("setBulletScreenRect", &setScreenRect)
			.def("isBulletValid", &isBulletValid)
			.def("setBulletId", &setBulletId)
			.def("getBulletSize", &getBulletSize)
			.def("getBulletDirect", &getBulletDirect)
			.def("setBulletDownReflection", &setBulletDownReflection)
			.def("setBulletAlphaBlend", &setBulletAlphaBlend);
		
		gge::LuaClass<meadBossManager>(state, "BossManager", meadBossManager::Instance())
			.def("createBoss", &meadBossManager::createBoss)
			.def("isBossDied", &meadBossManager::isBossDied)
			.def("clearAllBosses", &meadBossManager::clearAllBosses)
			.def("update", &meadBossManager::update)
			.def("render", &meadBossManager::render);
		
		sora::exportGlobalValuesManager(state);
	}
	
} // namespace mead


#endif // MEAD_BULLET_LUA_EXPORT_H_