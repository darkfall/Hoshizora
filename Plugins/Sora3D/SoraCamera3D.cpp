//
//  SoraCamera3D.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/14/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraCamera3D.h"

#include "SoraCore.h"
#include "SoraRenderSystem.h"

namespace sora {
    
    
    Sora3DCamera::Sora3DCamera(CameraMode mode):
    mCameraMode(mode) {
        
    }
    
    Sora3DCamera::Sora3DCamera(CameraMode mode, float l, float r, float b, float t, float n, float f):
    mCameraMode(mode) {
        if(mode == Ortho)
            mViewMatrix = SoraMatrix4::OrthoMat(l, r, b, t, n, f);
        else if(mode == Perspective) 
            mViewMatrix = SoraMatrix4::FrustumMat(l, r, b, t, n, f);
    }
    
    Sora3DCamera::Sora3DCamera(float fov, float aspect, float nearx, float farx):
    mCameraMode(Perspective) {
        mViewMatrix = SoraMatrix4::PerspectiveMat(fov, aspect, nearx, farx);
    }
    
    Sora3DCamera::CameraMode Sora3DCamera::getCameraMode() const {
        return mCameraMode;
    }
    
    void Sora3DCamera::setProjectionMatrix(const SoraMatrix4& mat) {
        mViewMatrix = mat;
    }
    
    SoraMatrix4& Sora3DCamera::getProjectionMatrix() {
        return mViewMatrix;
    }
    
    const SoraMatrix4& Sora3DCamera::getProjectionMatrix() const {
        return mViewMatrix;
    }
    
    void Sora3DCamera::setRotation(float x, float y, float z) {
        mTransform.setRotation(x, y, z);
    }
    
    void Sora3DCamera::setScale(float x, float y, float z) {
        mTransform.setScale(x, y, z);
    }
    
    void Sora3DCamera::setPosition(float x, float y, float z) {
        mTransform.setPosition(x, y, z);
    }
    
    void Sora3DCamera::setRotation(const SoraVector3& v) {
        mTransform.setRotation(SoraQuaternion(v.x, v.y, v.z));
    }
    
    void Sora3DCamera::setScale(const SoraVector3& v) {
        mTransform.setScale(v);
    }
    
    void Sora3DCamera::setPosition(const SoraVector3& v) {
        mTransform.setPosition(v);
    }
    
    void Sora3DCamera::rotate(float x, float y, float z) {     
        SoraVector3 newRotation = mTransform.getRotation().get() + SoraVector3(x, y, z);
        mTransform.setRotation(SoraQuaternion(newRotation.x,
                                              newRotation.y,
                                              newRotation.z));
    }
    
    void Sora3DCamera::scale(float x, float y, float z) {
        mTransform.setScale(mTransform.getScale() + SoraVector3(x, y, z));
    }
    
    void Sora3DCamera::transform(float x, float y, float z) {
        mTransform.setPosition(mTransform.getPosition() + SoraVector3(x, y, z));
    }
    
    void Sora3DCamera::rotate(const SoraVector3& v) {
        SoraVector3 newRotation = mTransform.getRotation().get() + v;
        mTransform.setRotation(SoraQuaternion(newRotation.x,
                                              newRotation.y,
                                              newRotation.z));
    }
    
    void Sora3DCamera::scale(const SoraVector3& v) {
        mTransform.setScale(mTransform.getScale() + v);
    }
    
    void Sora3DCamera::transform(const SoraVector3& v) {
        mTransform.setPosition(mTransform.getPosition() + v);
    }
    
    void Sora3DCamera::apply() {
        SoraMatrix4 finalMat = mViewMatrix * mTransform.getTransformMatrix();
        
        SoraCore::Ptr->getRenderSystem()->setProjectionMatrix(finalMat);
    }
    
    void Sora3DCamera::lookAt(const SoraVector3& eye, const SoraVector3& pos, const SoraVector3& up) {
        SoraVector3 forward = pos - eye;
        forward.normalize();
        
        SoraVector3 side = forward.cross(up);
        side.normalize();
        
        SoraVector3 newup = side.cross(forward);
        
        SoraMatrix4 mat;
        mat.x[0] = side.x;
        mat.x[4] = side.y;
        mat.x[8] = side.z;
        mat.x[12] = 0.0f;
        
        mat.x[1] = up.x;
        mat.x[5] = up.y;
        mat.x[9] = up.z;
        mat.x[13] = 0.0f;
        
        mat.x[2] = -forward.x;
        mat.x[6] = -forward.y;
        mat.x[10] = -forward.z;
        mat.x[14] = 0.0f;
        
        mat.x[3] = mat.x[7] = mat.x[11] = 0.0f;
        mat.x[15] = 1.0f;
        
        mat = mViewMatrix * mat;
        mat.translate(-eye.x, -eye.y, -eye.z);
        
        mViewMatrix = mat;
    }
    
    void Sora3DCamera::lookAt(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz) {
        this->lookAt(SoraVector3(eyex, eyey, eyez),
                     SoraVector3(atx, aty, atz),
                     SoraVector3(upx, upy, upz));
    }
    
} // namespace sora
