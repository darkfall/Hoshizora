//
//  SoraLight.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/14/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraLight.h"

namespace sora {
    
    SoraLightSource::SoraLightSource(SoraLightSource::LightType type):
    mType(type),
    mAttribute(0),
    mEnabled(false),
    mColor(SoraColorRGBA()) {
        
    }
    
    SoraLightSource::~SoraLightSource() {
        
    }
    
    SoraLightSource::LightType SoraLightSource::getType() const {
        return mType;
    }
    
    int32 SoraLightSource::getAttribute() const {
        return mAttribute;
    }
    
    void SoraLightSource::setAttribute(int32 attr) {
        mAttribute = attr;
    }
    
    bool SoraLightSource::isEnabled() const {
        return mEnabled;
    }
    
    void SoraLightSource::setEnabled(bool flag) {
        mEnabled = flag;
    }
    
    void SoraLightSource::bindUpdateFunc(const sora::SoraFunction<void(SoraLightSource&)>& updateFunc) {
        mUpdateFunc = updateFunc;
    }
    
    void SoraLightSource::update() {
        if(mUpdateFunc) {
            mUpdateFunc(*this);
        }
    }
    
    SoraColorRGBA SoraLightSource::getColor() const {
        return mColor;
    }
    
    void SoraLightSource::setColor(const SoraVector3& color) {
        mColor = SoraColorRGBA(color.x, color.y, color.z, SoraVector3(color) * SoraVector3(0.2126f, 0.7152f, 0.0722f));
    }
    
    SoraVector3 SoraLightSource::getPosition() const {
        return SoraVector3();
    }
    
    void SoraLightSource::setPosition(const SoraVector3&) {
        
    }
    
    SoraVector3 SoraLightSource::getDirection() const {
        return SoraVector3();
    }
    
    void SoraLightSource::setDirection(const SoraVector3&) {
        
    }
    
    SoraQuaternion SoraLightSource::getRotation() const {
        return SoraQuaternion();
    }
    
    void SoraLightSource::setRotation(const SoraQuaternion&) {
        
    }
    
    void SoraLightSource::setModelMatrix(const SoraMatrix4& mat) {
        
    }
    
    SoraVector3 SoraLightSource::getFalloff() const {
        return SoraVector3();
    }
    
    void SoraLightSource::setFalloff(const SoraVector3& falloff) {
        
    }
    
    float SoraLightSource::getCosInnerAngle() const {
        return 0.f;
    }
    
    void SoraLightSource::setInnerAngle(float) {
        
    }
    
    void SoraLightSource::setOuterAngle(float) {
        
    }
    
    float SoraLightSource::getCosOuterAngle() const {
        return 0.f;
    }
    
    SoraTextureHandle SoraLightSource::getProjectiveTexture() const {
        return 0;
    }
    
    void SoraLightSource::setProjectiveTexture(SoraTextureHandle) {
        
    }
    
    SoraAmbientLightSource::SoraAmbientLightSource():
    SoraLightSource(Ambient) {
        mAttribute = NoShadow;
        mColor = SoraColorRGBA(0.f, 0.f, 0.f, 0.f);
    }
    
    SoraAmbientLightSource::~SoraAmbientLightSource() {
        
    }
    
    SoraPointLightSource::SoraPointLightSource():
    SoraLightSource(Point),
    mQuat(SoraQuaternion()),
    mPos(SoraVector3()) {
        
    }
    
    SoraPointLightSource::~SoraPointLightSource() {
        
    }
    
    SoraVector3 SoraPointLightSource::getPosition() const {
        return mPos;
    }
    
    void SoraPointLightSource::setPosition(const SoraVector3& pos) {
        mPos = pos;
    }
    
    SoraQuaternion SoraPointLightSource::getRotation() const {
        return mQuat;
    }
    
    void SoraPointLightSource::setRotation(const SoraQuaternion& rot) {
        mQuat = rot;
    }
    
    void SoraPointLightSource::setModelMatrix(const SoraMatrix4& model) {
        SoraVector3 scale;
        SoraVector3 rot;
        model.decompose(mPos, rot, scale);
        mQuat.makeRotate(rot.x, rot.y, rot.z);
    }
    
    void SoraPointLightSource::updateCameras() {
        
    }
    
    SoraVector3 SoraPointLightSource::getDirection() const {
        return mQuat.rotate(SoraVector3(0, 0, 1));
    }
    
    void SoraPointLightSource::setDirection(const SoraVector3& dir) {
        mQuat.makeRotate(dir.x, dir.y, dir.z);
    }
    
    SoraVector3 SoraPointLightSource::getFalloff() const {
        return mFalloff;
    }
    
    void SoraPointLightSource::setFalloff(const SoraVector3& off) {
        mFalloff = off;
    }
    
    SoraTextureHandle SoraPointLightSource::getProjectiveTexture() const {
        return mProjectiveTex;
    }
    
    void SoraPointLightSource::setProjectiveTexture(SoraTextureHandle tex) {
        mProjectiveTex = tex;
    }
    
    SoraSpotLightSource::SoraSpotLightSource():
    SoraLightSource(Spot),
    mQuat(SoraQuaternion()),
    mPos(SoraVector3()) {
        
    }
    
    SoraSpotLightSource::~SoraSpotLightSource() {
        
    }
    
    SoraVector3 SoraSpotLightSource::getPosition() const {
        return mPos;
    }
    
    void SoraSpotLightSource::setPosition(const SoraVector3& pos) {
        mPos = pos;
    }
    
    SoraVector3 SoraSpotLightSource::getDirection() const {
        return mQuat.rotate(SoraVector3(0, 0, 1));
    }
    
    void SoraSpotLightSource::setDirection(const SoraVector3& dir) {
        mQuat.makeRotate(dir.x, dir.y, dir.z);
    }
    
    SoraQuaternion SoraSpotLightSource::getRotation() const {
        return mQuat;
    }
    
    void SoraSpotLightSource::setRotation(const SoraQuaternion& rot) {
        mQuat = rot;
    }
    
    void SoraSpotLightSource::setModelMatrix(const SoraMatrix4& mat) {
        SoraVector3 scale;
        SoraVector3 rot;
        mat.decompose(mPos, rot, scale);
        mQuat.makeRotate(rot.x, rot.y, rot.z);
    }
    
    SoraVector3 SoraSpotLightSource::getFalloff() const {
        return mFalloff;
    }
    
    void SoraSpotLightSource::setFalloff(const SoraVector3& vec) {
        mFalloff = vec;
    }
    
    float SoraSpotLightSource::getCosInnerAngle() const {
        return mCosOuterInner.y;
    }
    
    float SoraSpotLightSource::getCosOuterAngle() const {
        return mCosOuterInner.x;
    }
    
    void SoraSpotLightSource::setInnerAngle(float angle) {
        mCosOuterInner.y = cosf(angle);
    }
    
    void SoraSpotLightSource::setOuterAngle(float angle) {
        mCosOuterInner.x = cosf(angle);
        mCosOuterInner.z = angle * 2;
        mCosOuterInner.y = tan(angle);
    }
    
    SoraTextureHandle SoraSpotLightSource::getProjectiveTexture() const {
        return mProjectiveTex;
    }
    
    void SoraSpotLightSource::setProjectiveTexture(SoraTextureHandle tex) {
        mProjectiveTex = tex;
    }
    
    SoraDirectionalLightSource::SoraDirectionalLightSource():
    SoraLightSource(Directional),
    mQuat(SoraQuaternion()) {
        mAttribute = NoShadow;
    }
    
    SoraDirectionalLightSource::~SoraDirectionalLightSource() {

    }
    
    SoraQuaternion SoraDirectionalLightSource::getRotation() const {
        return mQuat;
    }
    
    void SoraDirectionalLightSource::setRotation(const SoraQuaternion& rot) {
        mQuat = rot;
    }
    
    SoraVector3 SoraDirectionalLightSource::getDirection() const {
        return mQuat.rotate(SoraVector3(0, 0, 1));
    }
    
    void SoraDirectionalLightSource::setDirection(const SoraVector3& dir) {
        mQuat.makeRotate(dir.x, dir.y, dir.z);
    }
    
    void SoraDirectionalLightSource::setModelMatrix(const SoraMatrix4& model) {
        SoraVector3 scale, pos, rot;
        model.decompose(pos, rot, scale);
        mQuat.makeRotate(rot.x, rot.y, rot.z);
    }
    
    SoraVector3 SoraDirectionalLightSource::getFalloff() const {
        return mFalloff;
    }
    
    void SoraDirectionalLightSource::setFalloff(const SoraVector3& off) {
        mFalloff = off;
    }
    
    
} // namespace sora
