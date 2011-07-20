/*
 *  SoraCamera.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCamera.h"
#include "SoraCore.h"
#include "Modifiers/SoraCameraModifiers.h"

namespace sora {

    SoraCamera::SoraCamera(float32 x, float32 y, float32 viewWidth, float32 viewHeight):
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true){
        setPosition(x, y);
        mViewRect.Set(x, y, viewWidth, viewHeight);
    }
    
    SoraCamera::SoraCamera():
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true) {
        mViewRect.Set(0.f, 0.f, (float32)SORA->getScreenWidth(), (float32)SORA->getScreenHeight());
    }
    
    SoraCamera::~SoraCamera() {
        
    }
    
    void SoraCamera::setViewRect(float32 width, float32 height) {
        mViewRect.Set(getPositionX(),
                      getPositionY(),
                      width,
                      height);
        mPropChanged = true;
    }
    float32 SoraCamera::getViewWidth() const {
        return mViewRect.x2;
    }
    
    float32 SoraCamera::getViewHeight() const {
        return mViewRect.y2;
    }
    
    void SoraCamera::setZoom(float32 h, float32 v) {
        if(mHZoom != h || mVZoom != v) {
            mHZoom = h;
            mVZoom = v;
            mPropChanged = true;
        }
    }
    
    void SoraCamera::setRotation(float32 rot) {
        if(mRotation != rot) {
            mRotation = rot;
            mPropChanged = true;
        }
    }
    
    void SoraCamera::getZoom(float32* h, float32* v) {
        *h = mHZoom;
        *v = mVZoom;
    }
    
    void SoraCamera::setPosition(float32 x, float32 y) {
        if(mPosx != x || mPosy != y) {
            mPropChanged = true;
            SoraObject::setPosition(x, y);
        }
    }
    
    float32 SoraCamera::getRotation() const {
        return mRotation;
    }
    
    float32 SoraCamera::getHZoom() const {
        return mHZoom;
    }
    
    float32 SoraCamera::getVZoom() const {
        return mVZoom;
    }

    uint32 SoraCamera::update(float32 dt) {
        if(mPropChanged) {
            sora::SORA->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
            sora::SORA->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
            mPropChanged = false;
        }
		return 0;
    }
    
    void SoraCamera::render() {
        if(getObjSize() != 0) {
            sora::SORA->pushTransformMatrix();
            sora::SORA->pushClippingMatrix();
            apply();
            SoraObject::render();
            sora::SORA->popTransformMatrix();
            sora::SORA->popClippingMatrix();
        }
    }
    
    void SoraCamera::apply() {
        sora::SORA->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
        sora::SORA->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
    }
    
    void SoraCamera::moveTo(float32 x, float32 y, float32 inTime) {
        CreateModifierAdapter(this, 
                              new SoraCameraPositionModifier(getPositionX(),
                                                             getPositionY(),
                                                             x, 
                                                             y,
                                                             inTime));

    }
    
    void SoraCamera::rotateTo(float32 newRot, float32 inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraRotationModifier(mRotation,
                                                             newRot,
                                                             inTime));
    }

    void SoraCamera::zoomTo(float32 hz, float32 vz, float32 inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraZoomModifier(mHZoom,
                                                         mVZoom,
                                                         hz,
                                                         vz,
                                                         inTime));
                                              
    }
    
    void SoraCamera::transformTo(SoraCamera* r, float32 inTime) {
        assert(r);
        moveTo(r->getPositionX(), r->getPositionY(), inTime);
        rotateTo(r->getRotation(), inTime);
        zoomTo(r->mHZoom, r->mVZoom, inTime);
    }
    
    SoraCamera& SoraCamera::operator=(const SoraCamera& rhs) {
        if(&rhs != this) {
            this->mHZoom = rhs.mHZoom;
            this->mVZoom = rhs.mVZoom;
            this->mRotation = rhs.mRotation;
            this->mPosx = rhs.mPosx;
            this->mPosy = rhs.mPosy;
            
            SoraObject::operator=(rhs);
        }
        return *this;
    }
} // namespace sora