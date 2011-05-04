/*
 *  meadBulletManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_BULLET_MANAGER_H_
#define MEAD_BULLET_MANAGER_H_

#include "meadBullet.h"

#include "QuickList.h"
#include "SoraColor.h"
#include "SoraSprite.h"

#include "AutoContainer.h"

namespace mead {
	
	typedef void (*CustomBulletCollisionFunc)(meadBullet*, void* userData1, void* userData2);
	
	class meadBulletManager: public sora::AutoListElement<meadBulletManager>  {
	public:
		meadBulletManager();
		~meadBulletManager();
		
		/*
		 texId see texfile
		 delay in frame
		 */
		ulong32 shootBullet(float32 posx, float32 posy, float32 speed, float32 direction, uint32 bulletId, uint32 shootTime); 
		ulong32 shootBulletEx(float32 posx, float32 posy, float32 speed, float32 direction, float32 directionIncremental, uint32 bulletId, uint32 shootTime);
		
		ulong32 shootLaser(float32 posx, float32 posy, float32 speed, float32 direction, float32 length, uint32 bulletId, uint32 shootTime);
		ulong32 shootLaserEx(float32 x, float32 y, float32 width, float32 direction, uint32 bulletId, uint32 shootTime, uint32 lastTime);
		
		// lua user data export
		ulong32 shootBulletDirect(void* bullet, uint32 bulletId, uint32 shootTime); 
		
		void loadBulletConfig(const std::wstring& file);
		void loadBulletSprite(uint32 texId, const std::wstring& sprFile);
		
		void update();
		
		// if bForce == true, then even immortal bullet would be cleared
		void clearBulletList(bool bForce=false);
		
		void setScreenRect(const hgeRect& rect);
		void setScreenRectAABB(float32 x, float32 y, float32 x2, float32 y2);
		
		bool isBulletValid(ulong32 handle);
		void setBulletId(ulong32 handle, uint32 bid);
		
		uint32 getSize();
		
		meadBullet* getBullet(ulong32 handle);
		
		// lua user data export
		void* getBulletDirect(ulong32 handle);
		
	private:
		typedef QuickList<meadBullet*> BULLET_LIST;
		typedef sora::hash_map<ulong32, meadBullet*> BULLET_MAP;
		
		// bullet list 
		BULLET_LIST mBullets;
		// bullet buffer for fast get bullet 
		BULLET_MAP mBulletBuffer;
		
		typedef sora::hash_map<uint32/*texid*/, sora::SoraSprite*> BULLET_SPRITE_MAP;
		
		// bullet sprite map
		BULLET_SPRITE_MAP mBulletSprites;
		
		struct BulletConfigNode {
			hgeRect mTextureRect;
			sora::SoraColorRGBA mShootColor;
			
			uint32 mTextureId;
			
			BulletConfigNode(const hgeRect& rect, const sora::SoraColorRGBA& color, uint32 texid):
			mTextureRect(rect), mShootColor(color), mTextureId(texid) {}
			
			BulletConfigNode() {}
		};
		
		typedef sora::hash_map<uint32/*bulletid*/, BulletConfigNode> BULLET_CONFIG_MAP;
		
		// bullet config map
		BULLET_CONFIG_MAP mBulletConfigs;

		
		hgeRect mScreenRect;
	};
	
} // namespace mead;



#endif // MEAD_BULLET_MANAGER_H_