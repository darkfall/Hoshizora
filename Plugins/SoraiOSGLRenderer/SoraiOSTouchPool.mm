/*
 *  SoraiOSTouchPool.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#include "SoraiOSTouchPool.h"
#include "SoraiOSTouchInfo.h"

namespace sora {
	
	static SoraiOSTouch* g_touch = NULL;

	void SoraiOSInputDelegate::iOSTouchBegin(NSSet* touches, UIEvent* ev, UIView* view) {
		// currently only support touch event
		// ignore others
		// see UIEvent document for other event types, such as remote controller
		if([ev type] != UIEventTypeTouches) return;
		
		NSUInteger count = [touches count];
		NSArray* touchArray = [touches allObjects];
		UITouch* touch;
		
		for(uint32 i=0; i<count; ++i) {
			touch = [touchArray objectAtIndex:i];
			CGPoint p = [touch locationInView: view];
			
			SoraiOSTouchInfo info;
			info.x = p.x; info.y = p.y;
			info.fingerId = i;
			info.phase = TOUCHPHASE_BEGAN;
			info.tapCount = [touch tapCount];
			info.delta = [touch timestamp];

			g_touch->mTouchInfos.push_back(info);
		}
	}
	
	void SoraiOSInputDelegate::iOSTouchMoved(NSSet* touches, UIEvent* ev, UIView* view) {
		NSUInteger count = [touches count];
		NSArray* touchArray = [touches allObjects];
		UITouch* touch;
		
		for(uint32 i=0; i<count; ++i) {
			touch = [touchArray objectAtIndex:i];
			CGPoint p = [touch locationInView: view];
			
			SoraiOSTouchInfo info = g_touch->getTouch(i);
			
			info.dx = p.x - info.x; info.dy = p.y - info.y;
			info.x = p.x; info.y = p.y;
			info.phase = TOUCHPHASE_MOVED;
			info.tapCount = [touch tapCount];
			info.delta = [touch timestamp];
			g_touch->mTouchInfos[i] = info;
		}
	}
	
	void SoraiOSInputDelegate::iOSTouchEnded(NSSet* touches, UIEvent* ev, UIView* view) {
		NSUInteger count = [touches count];
		NSArray* touchArray = [touches allObjects];
		UITouch* touch;
		
		for(uint32 i=0; i<count; ++i) {
			touch = [touchArray objectAtIndex:i];
			CGPoint p = [touch locationInView: view];
			
			SoraiOSTouchInfo info = g_touch->getTouch(i);
			
			info.dx = p.x - info.x; info.dy = p.y - info.y;
			info.x = p.x; info.y = p.y;
			info.phase = TOUCHPHASE_ENDED;
			info.tapCount = [touch tapCount];
			info.delta = [touch timestamp];
			
			g_touch->mTouchInfos[i] = info;
		}
	}
	
	void SoraiOSInputDelegate::iOSTouchCancled(NSSet* touches, UIEvent* ev, UIView* view) {
		NSUInteger count = [touches count];
		NSArray* touchArray = [touches allObjects];
		UITouch* touch;
		
		for(uint32 i=0; i<count; ++i) {
			touch = [touchArray objectAtIndex:i];
			CGPoint p = [touch locationInView: view];
			
			SoraiOSTouchInfo info = g_touch->getTouch(i);
			
			info.dx = p.x - info.x; info.dy = p.y - info.y;
			info.x = p.x; info.y = p.y;
			info.phase = TOUCHPHASE_CANCLED;
			info.tapCount = [touch tapCount];
			info.delta = [touch timestamp];
			
			g_touch->mTouchInfos[i] = info;
		}
	}
	
	void SoraiOSInputDelegate::updateTouchInfo(NSSet* touches, UIView* view) {
		if(!g_touch) g_touch = SoraiOSTouch::Instance();
		
		g_touch->flush();
		
		NSUInteger count = [touches count];
		NSArray* touchArray = [touches allObjects];
		UITouch* touch;
		
		for(uint32 i=0; i<count; ++i) {
			touch = [touchArray objectAtIndex:i];
			CGPoint p = [touch locationInView: view];
			
            SoraiOSTouchInfo info;
            info = g_touch->getTouch(i);
			
                        
			if(info.x != 0.f || info.y != 0.f)
				info.dx = p.x - info.x; info.dy = p.y - info.y;
			info.x = p.x; info.y = p.y;
			switch ([touch phase]) {
				case UITouchPhaseBegan:         info.phase = TOUCHPHASE_BEGAN; break;
				case UITouchPhaseStationary:    info.phase = TOUCHPHASE_STATIONARY; break;
				case UITouchPhaseMoved:         info.phase = TOUCHPHASE_MOVED; break;
				case UITouchPhaseEnded:         info.phase = TOUCHPHASE_ENDED; break;
				case UITouchPhaseCancelled:     info.phase = TOUCHPHASE_CANCLED; break;
			}
			info.fingerId = i;
			info.tapCount = [touch tapCount];
			if(info.timestamp != 0.f)
				info.delta = [touch timestamp] - info.timestamp;
			info.timestamp = [touch timestamp];

		//	printf("building ios input date, fingerid=%d, posx,y=%f,%f, deltax,y=%f, %f, timestamp=%f, tapcount=%d, phase=%d\n",
		//		   info.fingerId, info.x, info.y, info.dx, info.dy, info.delta, info.tapCount, info.phase);
			
			g_touch->touchCount = count;
			g_touch->mTouchInfos[i] = info;
		}
	}
	
} // namespace sora