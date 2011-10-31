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
    
    class SORA_API SoraCameraBase: public SoraObject {
    public:
        virtual void apply() = 0;
    };
	
	class SORA_API Sora2DCamera: public SoraCameraBase {
	public:
        Sora2DCamera();
		Sora2DCamera(float x, float y, float viewWidth, float viewHeight);
		~Sora2DCamera();
		
        void    setZoom(float h, float v);
        void    setRotation(float rot);
        
        void    setViewRect(float width, float height);
        float   getViewWidth() const;
        float   getViewHeight() const;
            
        void    getZoom(float* h, float* v);
        float   getRotation() const;
        float   getHZoom() const;
        float   getVZoom() const;
        
        void    setPosition(float x, float y);
        /**
         *  Modifier helper functions
         *  Use SoraModifierAdapter as applier
         **/
        void moveTo(float x, float y, float inTime);
        void rotateTo(float newRot, float inTime);
        void zoomTo(float hz, float vz, float inTime);
        
        /**
         *  Transform to another camera
         *  Position, rotation and zoom applies
         **/
        void transformTo(Sora2DCamera* r, float inTime);
        		
        virtual int32 update(float dt);
        virtual void render();
        
        void apply();
        
	private:
        float mHZoom;
        float mVZoom;
        float mRotation;
        
        SoraRect mViewRect;
        
        bool mPropChanged;
        
        typedef SoraModifierAdapter<Sora2DCamera> CameraModifier;
	};
    
    /**
     * Simple 3d camera implementation
     * Since we won't go far in 3d in Hoshizora
     **/
    class Sora3DCamera: public SoraCameraBase {
    public:
        enum CameraMode {
            // ortho camera
            Ortho, 
            // perspective camera
            Perspective,
        };
        
        /**
         * Ortho camera default
         * since this is still a 2d engine
         **/
        explicit Sora3DCamera(CameraMode mode = Ortho);
        // build using ortho or frustum
        Sora3DCamera(CameraMode mode, float l, float r, float b, float t, float n, float f);
        // build using perspective
        Sora3DCamera(float fov, float aspect, float near, float far);
        
        CameraMode getCameraMode() const;
        
        void setProjectionMatrix(const SoraMatrix4& mat);
        
        SoraMatrix4& getProjectionMatrix();
        const SoraMatrix4& getProjectionMatrix() const;
        
    public:
        /**
         * rotation, scale, transform
         **/
        void setRotation(float x, float y, float z);
        void setScale(float x, float y, float z);
        void setPosition(float x, float y, float z);
        
        void setRotation(const SoraVector3&);
        void setScale(const SoraVector3&);
        void setPosition(const SoraVector3&);
        
        void rotate(float x, float y, float z);
        void scale(float x, float y, float z);
        void transform(float x, float y, float z);
        
        void rotate(const SoraVector3&);
        void scale(const SoraVector3&);
        void transform(const SoraVector3&);
        
        
        // glutLookAt
        void lookAt(float eyex, float eyey, float eyez,
                    float atx, float aty, float atz,
                    float upx, float upy, float upz);
        void lookAt(const SoraVector3& eye,
                    const SoraVector3& at,
                    const SoraVector3& up);
        
    public:
        // inheritated from SoraCameraBase
        void apply();
        
    private:
        CameraMode mCameraMode;
        SoraMatrix4 mViewMatrix;
    };
	
} // namespace sora

#endif // SORA_CAMERA_H_