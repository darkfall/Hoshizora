/*
 *  SoraGesture.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GESTURE_H_
#define SORA_GESTURE_H_

#include "SoraPlatform.h"
#include "SoraMath.h"

#include <vector>
#include <list>

namespace sora {
    
    class SoraGestureTypeRecognizer;
    
    typedef enum {
        GESTURE_UNRECOGNIZED = 0,
        
        GESTURE_SLIDE_UP,
        GESTURE_SLIDE_DOWN,
        GESTURE_SLIDE_LEFT,
        GESTURE_SLIDE_RIGHT,
        
        GESTURE_SLIDE_UP_LEFT,
        GESTURE_SLIDE_UP_RIGHT,
        
        GESTURE_SLIDE_DOWN_LEFT,
        GESTURE_SLIDE_DOWN_RIGHT,
        
        GESTURE_SLIDE_LEFT_UP,
        GESTURE_SLIDE_LEFT_DOWN,
        
        GESTURE_SLIDE_RIGHT_UP,
        GESTURE_SLIDE_RIGHT_DOWN,
        
        GESTURE_CUSTOM,
    } SoraGestureType;
    
    typedef enum {
        GESTURE_DIR_INIT = 0,
        
        GESTURE_DIR_UP,
        GESTURE_DIR_DOWN,
        GESTURE_DIR_LEFT,
        GESTURE_DIR_RIGHT,
    } SoraGestureDirection;
	    
	class SoraGesture {
	public:
        friend class SoraGestureRecognizer;
        
        typedef struct {
            SoraGestureDirection mDirection;
            SoraVector mPoint;
        } GestureSigPoint;
        
        SoraGesture();
		~SoraGesture();
        
        SoraGestureType getGestureType() const;
        
        typedef std::vector<GestureSigPoint> GestureSigPointList;
        const GestureSigPointList& getSigPoints() const;
        
        void clear();
        void pushPoint(float32 x, float32 y);
        
        static void addTypeRecoginizer(SoraGestureTypeRecognizer* recognizer);
        static void delTypeRecoginizer(SoraGestureTypeRecognizer* recognizer);
        
    private:
        typedef std::list<SoraGestureTypeRecognizer*> GestureRecognizerList;
        static GestureRecognizerList g_Recognizers;
        
        static SoraGestureType doRecognizeGestureType(const SoraGesture::GestureSigPointList& sigPoints);
        
        GestureSigPointList mSigPoints;
        SoraGestureType mGestureType;
	};
	
	class SoraGestureTypeRecognizer {
    public:
        virtual SoraGestureType recognizeGesture(const SoraGesture::GestureSigPointList& sigPoints) = 0;
    };
} // namespace sora


#endif // SORA_GESTURE_H_