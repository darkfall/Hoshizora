/*
 *  SoraiOSTouchPool.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_IOS_TOUCH_POOL_H_
#define SORA_IOS_TOUCH_POOL_H_

#include "SoraPlatform.h"

#ifdef OS_IOS

#include "SoraiOSInput.h"

namespace sora {
	
	/*
	 call these in your EAGLView delegate to transfer input info to SORA
	 */
	
	class SoraiOSInputDelegate {
	protected:
		static void iOSTouchBegin(NSSet* touches, UIEvent* ev, UIView* view);
		static void iOSTouchMoved(NSSet* touches, UIEvent* ev, UIView* view);
		static void iOSTouchEnded(NSSet* touches, UIEvent* ev, UIView* view);
		static void iOSTouchCancled(NSSet* touches, UIEvent* ev, UIView* view);	
		
	public:		
		// for update
		static void updateTouchInfo(NSSet* touches, UIView* view);
	};
	
	
} // namespace sora

#endif // OS_IOS


#endif // SORA_IOS_TOUCH_POOL_H_
