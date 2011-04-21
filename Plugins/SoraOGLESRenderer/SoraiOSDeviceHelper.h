/*
 *  SoraiOSResolutionHelper.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
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
#include "SoraFileUtility.h"
#include "soraiOSFileUtility.h"

namespace sora {

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


static SoraWString iOSGetResourceName(const SoraWString& origName, bool appendRetina=true) {
	if(appendRetina && _IS_RETINA_DISPLAY()) {
		SoraWString fn = origName.substr(0, origName.rfind(L"."));
		SoraWString ext = origName.substr(origName.rfind(L"."), origName.size());
		return sora::SoraFileUtility::getApplicationPath() + fn + L"@2x" + ext;
	}
	return sora::SoraFileUtility::getApplicationPath() + origName;
}
	
	static SoraWString iOSGetDocumentResourceName(const SoraWString& origName, bool appendRetina=true) {
		if(appendRetina && _IS_RETINA_DISPLAY()) {
			SoraWString fn = origName.substr(0, origName.rfind(L"."));
			SoraWString ext = origName.substr(origName.rfind(L"."), origName.size());
			return appDocumentPath() + fn + L"@2x" + ext;
		}
		return appDocumentPath() + origName;
	}
	
	//Objc NSString 和 String 的相互转换
	static std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	static NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
	static NSString* string2NSString(const char* str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str];
		return buffer;
	}
	
	static NSString* wstring2NSString(const std::wstring& ws) {
		char* data = (char*)ws.data();
		unsigned size = ws.size() * sizeof(wchar_t);
		
#if TARGET_RT_BIG_ENDIAN
		const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
		const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif
		
		NSString* result = [[[NSString alloc] initWithBytes:data length:size encoding:kEncoding_wchar_t] autorelease];
		return result;
	}
	/*
	 Family: Hiragino Kaku Gothic ProN W3 
	 Font: HiraKakuProN-W3 
	 Family: Courier 
	 Font: Courier 
	 Font: Courier-BoldOblique 
	 Font: Courier-Oblique 
	 Font: Courier-Bold 
	 Family: Arial 
	 Font: ArialMT 
	 Font: Arial-BoldMT 
	 Font: Arial-BoldItalicMT 
	 Font: Arial-ItalicMT 
	 Family: STHeiti TC 
	 Font: STHeitiTC-Light 
	 Font: STHeitiTC-Medium 
	 Family: AppleGothic 
	 Font: AppleGothic 
	 Family: Courier New 
	 Font: CourierNewPS-BoldMT 
	 Font: CourierNewPS-ItalicMT 
	 Font: CourierNewPS-BoldItalicMT 
	 Font: CourierNewPSMT 
	 Family: Zapfino 
	 Font: Zapfino 
	 Family: Hiragino Kaku Gothic ProN W6 
	 Font: HiraKakuProN-W6 
	 Family: Arial Unicode MS 
	 Font: ArialUnicodeMS 
	 Family: STHeiti SC 
	 Font: STHeitiSC-Medium 
	 Font: STHeitiSC-Light 
	 Family: American Typewriter 
	 Font: AmericanTypewriter 
	 Font: AmericanTypewriter-Bold 
	 Family: Helvetica 
	 Font: Helvetica-Oblique 
	 Font: Helvetica-BoldOblique 
	 Font: Helvetica 
	 Font: Helvetica-Bold 
	 Family: Marker Felt 
	 Font: MarkerFelt-Thin 
	 Family: Helvetica Neue 
	 Font: HelveticaNeue 
	 Font: HelveticaNeue-Bold 
	 Family: DB LCD Temp 
	 Font: DBLCDTempBlack 
	 Family: Verdana 
	 Font: Verdana-Bold 
	 Font: Verdana-BoldItalic 
	 Font: Verdana 
	 Font: Verdana-Italic 
	 Family: Times New Roman 
	 Font: TimesNewRomanPSMT 
	 Font: TimesNewRomanPS-BoldMT 
	 Font: TimesNewRomanPS-BoldItalicMT 
	 Font: TimesNewRomanPS-ItalicMT 
	 Family: Georgia 
	 Font: Georgia-Bold 
	 Font: Georgia 
	 Font: Georgia-BoldItalic 
	 Font: Georgia-Italic 
	 Family: STHeiti J 
	 Font: STHeitiJ-Medium 
	 Font: STHeitiJ-Light 
	 Family: Arial Rounded MT Bold 
	 Font: ArialRoundedMTBold 
	 Family: Trebuchet MS 
	 Font: TrebuchetMS-Italic 
	 Font: TrebuchetMS 
	 Font: Trebuchet-BoldItalic 
	 Font: TrebuchetMS-Bold 
	 Family: STHeiti K 
	 Font: STHeitiK-Medium 
	 Font: STHeitiK-Light
	*/

} // namespace sora

#endif // OS_IOS

#endif // SORA_IOS_RESOLUTION_HELPER_H_