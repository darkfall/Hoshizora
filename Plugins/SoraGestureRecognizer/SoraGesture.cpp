/*
 *  SoraGesture.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGesture.h"

#include "Debug/SoraInternalLogger.h"

namespace sora {
    
    static int32 DISTANCE_TOLERANCE = 50;
    
    SoraGesture::GestureRecognizerList SoraGesture::g_Recognizers;

    SoraGesture::SoraGesture():
    mGestureType(GESTURE_UNRECOGNIZED) {
    
    }
 
    SoraGesture::~SoraGesture() {
        
    }
    
    SoraGestureType SoraGesture::getGestureType() const {
        return mGestureType;
    }
    
    const SoraGesture::GestureSigPointList& SoraGesture::getSigPoints() const {
        return mSigPoints;
    }
    
    void SoraGesture::pushPoint(float32 x, float32 y) {
        if(mSigPoints.size() != 0) {
            SoraVector previousPoint = mSigPoints.back().mPoint;
            
            float32 dx = x - previousPoint.x;
            float32 dy = y - previousPoint.y;
            
            if(dx * dx + dy * dy < DISTANCE_TOLERANCE)
                return;
                        
            SoraGestureDirection gesDir;
            if(dx > 0 && abs(dy) < dx)
                gesDir = GESTURE_DIR_RIGHT;
            else if(dy > 0 && dy > abs(dx))
                gesDir = GESTURE_DIR_UP;
            else if(dx < 0 && abs(dy) < abs(dx))
                gesDir = GESTURE_DIR_LEFT;
            else if(dy < 0 && abs(dy) > abs(dx))
                gesDir = GESTURE_DIR_DOWN;
            
            if(gesDir != mSigPoints.back().mDirection) {
                mSigPoints.back().mDirection = gesDir;
                
                GestureSigPoint newSigPoint;
                newSigPoint.mDirection = gesDir;
                newSigPoint.mPoint = SoraVector(x, y);
                
                mSigPoints.push_back(newSigPoint);
                
                mGestureType = doRecognizeGestureType(mSigPoints);
            }
        } else {
            GestureSigPoint newSigPoint;
            newSigPoint.mDirection = GESTURE_DIR_INIT;
            newSigPoint.mPoint = SoraVector(x, y);
                        
            mSigPoints.push_back(newSigPoint);
        }
    }
    
    void SoraGesture::addTypeRecoginizer(SoraGestureTypeRecognizer* recognizer) {
        g_Recognizers.push_back(recognizer);
    }
    
    void SoraGesture::clear() {
        mSigPoints.clear();
        mGestureType = GESTURE_UNRECOGNIZED;
    }
    
    void SoraGesture::delTypeRecoginizer(SoraGestureTypeRecognizer* recognizer) {
        g_Recognizers.remove(recognizer);
    }
    
    SoraGestureType SoraGesture::doRecognizeGestureType(const SoraGesture::GestureSigPointList& sigPoints) {
        SoraGestureType type = GESTURE_UNRECOGNIZED;
        
        GestureRecognizerList::iterator itRecognizer = g_Recognizers.begin();
        while(itRecognizer != g_Recognizers.end()) {
            if((type = (*itRecognizer)->recognizeGesture(sigPoints)) != GESTURE_UNRECOGNIZED)
                return type;
            ++itRecognizer;
        }
        return type;
    }
    
} // namespace sora