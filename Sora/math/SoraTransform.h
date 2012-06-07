//
//  SoraTransform.h
//  Sora
//
//  Created by Robert Bu on 10/30/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTransform_h
#define Sora_SoraTransform_h

#include "SoraPlatform.h"

#include "SoraQuaternion.h"
#include "SoraVector3.h"
#include "SoraMatrix4.h"

namespace sora {
    
    /**
     * Transform for 3d objects
     **/
    
    struct SoraTransform {
        
        inline SoraTransform():
        mScale(SoraVector3(1.f, 1.f, 1.f)),
        mBuild(false) {
            
        }
        
        inline void buildTransformMatrix() {
            mTransformMatrix = SoraMatrix4::RotMat(mRotation.x, 
                                                    mRotation.y, 
                                                    mRotation.z).scale(mScale.x, 
                                                                        mScale.y, 
                                                                        mScale.z).translate(mPosition.x, 
                                                                                            mPosition.y, 
                                                                                            mPosition.z);;
        }
        
        inline void setTransformMatrix(const SoraMatrix4& mat) {
            SoraVector3 rot;
            mat.decompose(mPosition, rot, mScale);
            mRotation.makeRotate(rot.x, rot.y, rot.z);
            
            mTransformMatrix = mat;
        }
        
        inline SoraMatrix4& getTransformMatrix() {
            if(!mBuild)
                buildTransformMatrix();
            return mTransformMatrix;
        }

        /*
         set
         */
        inline void setPosition(const SoraVector3& pos) {
            mPosition = pos;
            
            mBuild = false;
        }
        
        inline void setScale(const SoraVector3& scale) {
            mScale = scale;
            
            mBuild = false;
        }
        
        inline void setRotation(const SoraQuaternion& rot) {
            mRotation = rot;
            
            mBuild = false;
        }
        
        inline void setRotation(const SoraVector3& axis, real angle) {
            mRotation = SoraQuaternion(axis.x, axis.y, axis.z, angle);
            
            mBuild = false;
        }
        
        inline void setPosition(real x, real y, real z) {
            mPosition.set(x, y, z);
            
            mBuild = false;
        }
        
        inline void setScale(real x, real y, real z) {
            mScale.set(x, y, z);
            
            mBuild = false;
        }
        
        inline void setRotation(real x, real y, real z) {
            mRotation = SoraQuaternion(x, y, z, 0);
            
            mBuild = false;
        }
        
        /*
         get
         */
        inline const SoraVector3& getPosition() const {
            return mPosition;
        }
        
        inline real getPositionX() const {
            return mPosition.x;
        }
        
        inline real getPositionY() const {
            return mPosition.y;
        }
        
        inline real getPositionZ() const {
            return mPosition.z;
        }
        
        inline const SoraVector3& getScale() const {
            return mScale;
        }
        
        inline real getScaleX() const {
            return mScale.x;
        }
        
        inline real getScaleY() const {
            return mScale.y;
        }
        
        inline real getScaleZ() const {
            return mScale.z;
        }
        
        inline const SoraQuaternion& getRotation() const {
            return mRotation;
        }
        
        inline real getRotationX() const {
            return mRotation.x;
        }
        
        inline real getRotationY() const {
            return mRotation.y;
        }
        
        inline real getRotationZ() const {
            return mRotation.z;
        }
        
    protected:
        SoraVector3 mPosition;
        SoraVector3 mScale;
        SoraQuaternion mRotation;
        
    private:
        SoraMatrix4 mTransformMatrix;
        bool        mBuild;
    };
    
} // namespace sora



#endif
