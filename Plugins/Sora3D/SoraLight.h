//
//  SoraLight.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLight_h
#define Sora_SoraLight_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraPreDeclare.h"
#include "SoraColor.h"
#include "SoraMath.h"

#include "function/SoraFunction.h"

namespace sora {
    
    class SORA_API SoraLightSource: public SoraSharedClass<SoraLightSource> {
    public:
        enum LightType {
            Ambient = 0,
            Point,
            Directional,
            Spot
        };
        
        enum LightSourceAttrib {
            NoShadow            = 1UL << 0,
            NoDiffuse           = 1UL << 1,
            NoSpecular          = 1UL << 2,
            IndirectLighting    = 1UL << 3,
        };
        
        explicit SoraLightSource(LightType type);
        virtual ~SoraLightSource();
        
        LightType getType() const;
        
        int32 getAttribute() const;
        void setAttribute(int32 attrib);
        
        bool isEnabled() const;
        void setEnabled(bool enabled);
        
        void bindUpdateFunc(const sora::SoraFunction<void(SoraLightSource&)>& updateFunc);
        
        virtual void update();
        
        SoraColorRGBA getColor() const;
        void setColor(const SoraVector3& color);
        
        virtual SoraVector3 getPosition() const;
        virtual void setPosition(const SoraVector3& pos);
        
        virtual SoraVector3 getDirection() const;
        virtual void setDirection(const SoraVector3& dir);
        
        virtual SoraQuaternion getRotation() const;
        virtual void setRotation(const SoraQuaternion& rot);
        
        virtual void setModelMatrix(const SoraMatrix4& mat);
        
        virtual SoraVector3 getFalloff() const;
        virtual void setFalloff(const SoraVector3& falloff);
        
        virtual float getCosInnerAngle() const;
        virtual float getCosOuterAngle() const;
        virtual void setInnerAngle(float angle);
        virtual void setOuterAngle(float angle);
        
        virtual SoraTextureHandle getProjectiveTexture() const;
        virtual void setProjectiveTexture(SoraTextureHandle tex);
        
    protected:
        LightType mType;
        int32 mAttribute;
        bool mEnabled;
        SoraColorRGBA mColor;
        
        sora::SoraFunction<void(SoraLightSource&)> mUpdateFunc;
    };
                                
    class SORA_API SoraAmbientLightSource: public SoraLightSource {
    public:
        SoraAmbientLightSource();
        virtual ~SoraAmbientLightSource();
    };
    
    class SORA_API SoraPointLightSource: public SoraLightSource {
    public:
        SoraPointLightSource();
        virtual ~SoraPointLightSource();
        
        SoraVector3 getPosition() const;
        void setPosition(const SoraVector3& pos);
        
        SoraVector3 getDirection() const;
        void setDirection(const SoraVector3& dir);
        
        SoraQuaternion getRotation() const;
        void setRotation(const SoraQuaternion& rot);
        
        void setModelMatrix(const SoraMatrix4& mat);
        
        SoraVector3 getFalloff() const;
        void setFalloff(const SoraVector3& falloff);
        
        SoraTextureHandle getProjectiveTexture() const;
        void setProjectiveTexture(SoraTextureHandle tex);
        
    protected:
        void updateCameras();
        
    protected:
        SoraQuaternion mQuat;
        SoraVector3 mPos;
        SoraVector3 mFalloff;
        
        SoraTextureHandle mProjectiveTex;
    };
    
    class SORA_API SoraSpotLightSource: public SoraLightSource {
    public:
        SoraSpotLightSource();
        virtual ~SoraSpotLightSource();
        
        SoraVector3 getPosition() const;
        void setPosition(const SoraVector3& pos);
        
        SoraVector3 getDirection() const;
        void setDirection(const SoraVector3& dir);
        
        SoraQuaternion getRotation() const;
        void setRotation(const SoraQuaternion& rot);
        
        void setModelMatrix(const SoraMatrix4& mat);
        
        SoraVector3 getFalloff() const;
        void setFalloff(const SoraVector3& falloff);
        
        float getCosInnerAngle() const;
        float getCosOuterAngle() const;
        void setInnerAngle(float angle);
        void setOuterAngle(float angle);
        
        SoraTextureHandle getProjectiveTexture() const;
        void setProjectiveTexture(SoraTextureHandle tex);
        
    protected:
        SoraQuaternion mQuat;
        SoraVector3 mPos;
        SoraVector3 mFalloff;
        SoraVector4 mCosOuterInner;
        
        SoraTextureHandle mProjectiveTex;
    };
    
    class SORA_API SoraDirectionalLightSource: public SoraLightSource {
    public:
        SoraDirectionalLightSource();
        virtual ~SoraDirectionalLightSource();
        
        SoraVector3 getDirection() const;
        void setDirection(const SoraVector3& dir);
        
        SoraQuaternion getRotation() const;
        void setRotation(const SoraQuaternion& rot);
        
        void setModelMatrix(const SoraMatrix4& mat);
        
        SoraVector3 getFalloff() const;
        void setFalloff(const SoraVector3& falloff);
        
    protected:
        SoraQuaternion mQuat;
        SoraVector3 mFalloff;
    };
        
    
    
} // namespace sora


#endif
