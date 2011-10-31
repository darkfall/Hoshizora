/*
 *  Sora2DCamera.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCamera.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"
#include "Modifiers/SoraCameraModifiers.h"

namespace sora {

    Sora2DCamera::Sora2DCamera(float x, float y, float viewWidth, float viewHeight):
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true){
        setPosition(x, y);
        mViewRect.set(x, y, viewWidth, viewHeight);
    }
    
    Sora2DCamera::Sora2DCamera():
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true) {
        mViewRect.set(0.f, 0.f, (float)SoraCore::Ptr->getScreenWidth(), (float)SoraCore::Ptr->getScreenHeight());
    }
    
    Sora2DCamera::~Sora2DCamera() {
        
    }
    
    void Sora2DCamera::setViewRect(float width, float height) {
        mViewRect.set(getPositionX(),
                      getPositionY(),
                      width,
                      height);
        mPropChanged = true;
    }
    float Sora2DCamera::getViewWidth() const {
        return mViewRect.x2;
    }
    
    float Sora2DCamera::getViewHeight() const {
        return mViewRect.y2;
    }
    
    void Sora2DCamera::setZoom(float h, float v) {
        if(mHZoom != h || mVZoom != v) {
            mHZoom = h;
            mVZoom = v;
            mPropChanged = true;
        }
    }
    
    void Sora2DCamera::setRotation(float rot) {
        if(mRotation != rot) {
            mRotation = rot;
            mPropChanged = true;
        }
    }
    
    void Sora2DCamera::getZoom(float* h, float* v) {
        *h = mHZoom;
        *v = mVZoom;
    }
    
    void Sora2DCamera::setPosition(float x, float y) {
        if(getPositionX() != x || getPositionY() != y) {
            mPropChanged = true;
            SoraObject::setPosition(x, y);
        }
    }
    
    float Sora2DCamera::getRotation() const {
        return mRotation;
    }
    
    float Sora2DCamera::getHZoom() const {
        return mHZoom;
    }
    
    float Sora2DCamera::getVZoom() const {
        return mVZoom;
    }

    int32 Sora2DCamera::update(float dt) {
        if(mPropChanged) {
            sora::SoraCore::Ptr->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
            sora::SoraCore::Ptr->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
            mPropChanged = false;
        }
		return 0;
    }
    
    void Sora2DCamera::render() {
        if(getObjSize() != 0) {
            sora::SoraCore::Ptr->pushTransformMatrix();
            sora::SoraCore::Ptr->pushClippingMatrix();
            apply();
            SoraObject::render();
            sora::SoraCore::Ptr->popTransformMatrix();
            sora::SoraCore::Ptr->popClippingMatrix();
        }
    }
    
    void Sora2DCamera::apply() {
        sora::SoraCore::Ptr->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
        sora::SoraCore::Ptr->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
    }
    
    void Sora2DCamera::moveTo(float x, float y, float inTime) {
        CreateModifierAdapter(this, 
                              new SoraCameraPositionModifier(getPositionX(),
                                                             getPositionY(),
                                                             x, 
                                                             y,
                                                             inTime));

    }
    
    void Sora2DCamera::rotateTo(float newRot, float inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraRotationModifier(mRotation,
                                                             newRot,
                                                             inTime));
    }

    void Sora2DCamera::zoomTo(float hz, float vz, float inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraZoomModifier(mHZoom,
                                                         mVZoom,
                                                         hz,
                                                         vz,
                                                         inTime));
                                              
    }
    
    void Sora2DCamera::transformTo(Sora2DCamera* r, float inTime) {
        assert(r);
        moveTo(r->getPositionX(), r->getPositionY(), inTime);
        rotateTo(r->getRotation(), inTime);
        zoomTo(r->mHZoom, r->mVZoom, inTime);
    }
    
    
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
    
    Sora3DCamera::Sora3DCamera(float fov, float aspect, float near, float far):
    mCameraMode(Perspective) {
        mViewMatrix = SoraMatrix4::PerspectiveMat(fov, aspect, near, far);
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