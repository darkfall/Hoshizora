/*
 *  SoraGestureRecognizer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGestureRecognizer.h"

#include "SoraGesture.h"
#include "SoraCore.h"

namespace sora {

    SoraGestureRecognizer::SoraGestureRecognizer():
    mTriggered(false),
    mDetectionInterval(0.01f),
    mCurrDetectionInterval(0.f) {
        SoraGesture::addTypeRecoginizer(new SoraBaseGestureTypeRecognizer);
        SORA->registerPlugin(this);
        
        mTriggerHotkey.set(SORA_KEY_RBUTTON);
    }
    
    SoraGestureRecognizer::~SoraGestureRecognizer() {
        SoraGesture::GestureRecognizerList::iterator itRecognizer = SoraGesture::g_Recognizers.begin();
        while(itRecognizer != SoraGesture::g_Recognizers.end()) {
            delete *itRecognizer;
            ++itRecognizer;
        }
    }
    
    void SoraGestureRecognizer::registerGestureHandler(SoraEventHandler* handler) {
        mHandlers.push_back(handler);
    }
    
    void SoraGestureRecognizer::unregisterGestureHandler(SoraEventHandler* handler) {
        mHandlers.remove(handler);
    }
    
    void SoraGestureRecognizer::addGestureTypeRecognizer(SoraGestureTypeRecognizer* recognizer) {
        SoraGesture::addTypeRecoginizer(recognizer);
    }
    
    void SoraGestureRecognizer::delGestureTypeRecognizer(SoraGestureTypeRecognizer* recognizer) {
        SoraGesture::delTypeRecoginizer(recognizer);
    }
    
    void SoraGestureRecognizer::update() {
        if(mTriggerHotkey.test()) {
            if(!mTriggered) {
                mTriggered = true;
                mCurrentGesture.clear();
            }
        } else {
            if(mTriggered) {
                mTriggered = false;
                
                if(mCurrentGesture.getGestureType() != GESTURE_UNRECOGNIZED) {
                    SoraGestureEvent event(&mCurrentGesture);
                    
                    GestureHandlerList::iterator itHandler = mHandlers.begin();
                    while(itHandler != mHandlers.end()) {
                        (*itHandler)->handleEvent(&event);
                        
                        if(event.isConsumed())
                            break;
                        
                        ++itHandler;
                    }
                }
                return;
            }
        }
        
        if(mTriggered) {
            if(mCurrDetectionInterval >= mDetectionInterval) {
                float32 mousex, mousey;
                SORA->getMousePos(&mousex, &mousey);
            
                mCurrentGesture.pushPoint(mousex, mousey);
                
                mCurrDetectionInterval = 0.f;
            }
            mCurrDetectionInterval += SORA->getDelta();
        }
    }
    
    const SoraString SoraGestureRecognizer::getName() const {
        return "SoraGestureRecognizer";
    }
    
    void SoraGestureRecognizer::setTriggerHotkey(const SoraHotkey& hotkey) {
        mTriggerHotkey = hotkey;
    }
    
    SoraHotkey SoraGestureRecognizer::getTriggerHotkey() const {
        return mTriggerHotkey;
    }
    
    void SoraGestureRecognizer::setDetectionInterval(float32 interval) {
        mDetectionInterval = interval;
    }
    
    float32 SoraGestureRecognizer::getDetectionInterval() const {
        return mDetectionInterval;
    }


} // namespace sora