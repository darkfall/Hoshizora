/*
 *  SoraiOSResolutionHelper.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio Symphonie. All rights reserved.
 *
 */
#ifndef SORA_IOS_RESOLUTION_HELPER_H_
#define SORA_IOS_RESOLUTION_HELPER_H_

#include "SoraPlatform.h"

// THIS MUST BE INCLUDED BY .MM FILE

// iphone resolution
// retina display for iPod Touch 4gen or iPhone 4 gen and up
// OS_IPAD for 1024*768
#ifdef OS_IOS

#import <UIKit/UIDevice.h>

static bool _IS_IPAD() {
	return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
}

static bool _IS_RETINA_DISPLAY() {
	if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0)) { 
		return true; 
	}	
	return false; 
}

static int32 iOSGetScreenWidth(int rot=0) {
	if(_IS_IPAD()) {
		return rot==1?1024:768;
	} else {
	//	if(_IS_RETINA_DISPLAY())
	//		return rot==1?960:640;
	//	else
			return rot==1?480:320;
	}
	return 0;
}

static int32 iOSGetScreenHeight(int rot=0) {
	if(_IS_IPAD()) {
		return rot==1?768:1024;
	} else {
	//	if(_IS_RETINA_DISPLAY())
	//		return rot==1?640:960;
	//	else
			return rot==1?320:480;
	}
	return 0;
}


static SoraWString iOSGetResourceName(const SoraWString& origName) {
	if(_IS_RETINA_DISPLAY()) {
		SoraWString fn = origName.substr(0, origName.rfind(L"."));
		SoraWString ext = origName.substr(origName.rfind(L"."), origName.size());
		return fn + L"@2x" + ext;
	}
	return origName;
}

#endif // OS_IOS

#endif // SORA_IOS_RESOLUTION_HELPER_H_