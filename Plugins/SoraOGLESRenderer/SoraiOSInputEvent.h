/*
 *  SoraiOSInputEvent.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 2/13/11.
 *  Copyright 2011 Studio Symphonie. All rights reserved.
 *
 */

#ifndef _SORA_IOS_INPUT_EVENT_H_
#define _SORA_IOS_INPUT_EVENT_H_

#import <Foundation/Foundation.h>

/* import this in your view and call corresponding functions to get input event in SoraCore */

namespace sora {

	static void iOSTouchesBegan(NSSet* touches, UIEvent* ev) {
	}
	
	static void iOSTouchesMoved(NSSet* touches, UIEvent* ev) {
	}
	
	static void iOSTouchesEnded(NSSet* touches, UIEvent* ev) {
	}
	
} // namespace sora

#endif // _SORA_IOS_INPUT_EVENT_H_