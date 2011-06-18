/*
 *  rftdSteer.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_STEER_H_
#define RFTD_STEER_H_

#include "SoraPlatform.h"
#include "SoraSpriteAnimation/SoraSpriteAnimation.h"

namespace rftd {
	
	class rftdSteerBehaviour {
	public:
		rftdSteerBehaviour(): pAnimSpr(NULL) {}
		
		void start() {
			bFinished = false;
			toNext();
		}
		
		void setSpeed(float32 s) {
			speed = s;
		}
		void setRotateSpeed(float32 rs) {
			rspeed = rs;
		}
		
		void setPosition(float32 x, float32 y) {
			posx = x;
			posy = y;
		}
		
		float32 getSpeed() const {
			return speed;
		}
		float32 getDirection() const {
			return direction;
		}
		
		float32 getPosX() const {
			return posx;
		}
		float32 getPosY() const {
			return posy;
		}
		void update(float32 dt) {
			if(bFinished)
				return;
			
			currTime += dt;
			if(currTime >= time) {
				toNext();
			}
			
			posx += speedX * dt;
			posy += speedY * dt;
		}
		
		void toNext() {
			if(route.size() == 0) {
				bFinished = true;
				return;
			}
			
			POINT nextPos = route.front();
			float32 distx = nextPos.x - posx;
			float32 disty = nextPos.y - posy;
			
			float32 dist = distx*distx + disty*disty;
			time = sqrt(dist / (speed*speed));
			currTime = 0;
			
			direction = atan2f(disty, distx);
			
			speedX = speed * cosf(direction);
			speedY = speed * sinf(direction);
			
			if(pAnimSpr != NULL) {
				if(speedY < 0.f)
					pAnimSpr->playEx("moveup", true, false);
				else if(speedY < 0.f)
					pAnimSpr->playEx("movedown", true, false);
				else if(speedX > 0.f)
					pAnimSpr->playEx("moveright", true, false);
				else if(speedX < 0.f)
					pAnimSpr->playEx("moveleft", true, false);
				else
					pAnimSpr->playEx("stand", true, false);
			}
			
			route.erase(route.begin());
		}
		
		void addDestPoint(float32 x, float32 y) {
			route.push_back(POINT(x, y));
		}
		void clearRoute() {
			route.clear();
		}
		
		bool isFinished() {
			return bFinished;
		}
		
		void setAnimationSprite(sora::SoraSpriteAnimation* pAnim) {
			pAnimSpr = pAnim;
		}
		
		sora::SoraSpriteAnimation* getAnimationSprite() const {
			return pAnimSpr;
		}
		
	private:
		float32 speed, speedX, speedY;
		float32 direction, nextDirection, rspeed;
		
		float32 time, currTime;
		
		float32 posx, posy;
		typedef struct tagPoint {
			float32 x, y;
			
			tagPoint(float32 _x, float32 _y): x(_x), y(_y) {}
		} POINT;
		
		bool bFinished;
		
		typedef std::vector<POINT> ROUTE_VEC;
		ROUTE_VEC route;
		
		sora::SoraSpriteAnimation* pAnimSpr;
	};
	
}

#endif // RFTD_STEER_H_