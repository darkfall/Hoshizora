/*
 *  SoraCamera.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CAMERA_H_
#define SORA_CAMERA_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraModifierAdapter.h"

namespace sora {
	
	class SORA_API SoraCamera: public SoraObject {
	public:
        SoraCamera();
		SoraCamera(float32 x, float32 y, float32 viewWidth, float32 viewHeight);
		~SoraCamera();
		
        void    setZoom(float32 h, float32 v);
        void    setRotation(float32 rot);
        
        void    setViewRect(float32 width, float32 height);
        float32 getViewWidth() const;
        float32 getViewHeight() const;
        
        void    getZoom(float32* h, float32* v);
        float32 getRotation() const;
        float32 getHZoom() const;
        float32 getVZoom() const;
        
        void    setPosition(float32 x, float32 y);
        /**
         *  Modifier helper functions
         *  Use SoraModifierAdapter as applier
         **/
        void moveTo(float32 x, float32 y, float32 inTime);
        void rotateTo(float32 newRot, float32 inTime);
        void zoomTo(float32 hz, float32 vz, float32 inTime);
        
        /**
         *  Transform to another camera
         *  Position, rotation and zoom applies
         **/
        void transformTo(SoraCamera* r, float32 inTime);
        
        SoraCamera& operator=(const SoraCamera& rhs);
		
        virtual uint32 update(float32 dt);
        virtual void render();
        
        void apply();
        
	private:
        float32 mHZoom;
        float32 mVZoom;
        float32 mRotation;
        
        SoraRect mViewRect;
        
        bool mPropChanged;
        
        typedef SoraModifierAdapter<SoraCamera> CameraModifier;
	};
	
} // namespace sora

#endif // SORA_CAMERA_H_