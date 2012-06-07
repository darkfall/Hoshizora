//
//  SoraCamera3D.h
//  Sora
//
//  Created by Robert Bu on 11/14/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCamera3D_h
#define Sora_SoraCamera3D_h

#include "SoraCamera.h"

namespace sora {
    
    
    /**
     * Simple 3d camera implementation
     * Since we won't go far in 3d in Hoshizora
     **/
    class Sora3DCamera: public SoraCamera {
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

#endif
