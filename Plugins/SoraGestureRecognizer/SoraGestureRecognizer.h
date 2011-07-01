/*
 *  SoraGestureRecognizer.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GESTURE_RECOGNIZER_H_
#define SORA_GESTURE_RECOGNIZER_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraPlugin.h"
#include "SoraEvent.h"
#include "SoraHotkey.h"

#include "SoraBaseGestureTypeRecognizer.h"

namespace sora {
	
    class SoraTimerEvent;
	
	class SoraGestureEvent: public SoraEvent {
	public:
		SoraGestureEvent(): mGesture(NULL) {}
		SoraGestureEvent(SoraGesture* gesture): mGesture(gesture) {}
		
		SoraGesture* getGesture() const {
			return mGesture;
		}
		
        SORA_EVENT_IDENTIFIER(str2id("SoraGestureEvent"));
        
	private:
		SoraGesture* mGesture;
	};
	
	class SoraGestureRecognizer: public SoraSingleton<SoraGestureRecognizer>, public SoraPlugin {
	protected:
		friend class SoraSingleton<SoraGestureRecognizer>;
		
		SoraGestureRecognizer();
		~SoraGestureRecognizer();
		
	public:
        void registerGestureHandler(SoraEventHandler* handler);
        void unregisterGestureHandler(SoraEventHandler* handler);
        
        void addGestureTypeRecognizer(SoraGestureTypeRecognizer* recognizer);
        void delGestureTypeRecognizer(SoraGestureTypeRecognizer* recognizer);
        
        void setTriggerHotkey(const SoraHotkey& hotkey);
        SoraHotkey getTriggerHotkey() const;
        
        void setDetectionInterval(float32 inteval);
        float32 getDetectionInterval() const;
        
        void update();
        const SoraString getName() const;
                
    private:
        typedef std::list<SoraEventHandler*> GestureHandlerList;
        GestureHandlerList mHandlers;
        
        bool mTriggered;
        SoraHotkey mTriggerHotkey;
        
        SoraGesture mCurrentGesture;
        
        float32 mDetectionInterval;
        float32 mCurrDetectionInterval;
	};
	
} // namespace sora

#endif // SORA_GESTURE_RECOGNIZER_H_