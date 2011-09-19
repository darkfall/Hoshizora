//
//  SoraPostEffect.h
//  Sora
//
//  Created by Ruiwei Bu on 9/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPostEffect_h
#define Sora_SoraPostEffect_h

#include "SoraPlatform.h"
#include "SoraShaderEnabled.h"

namespace sora {
    
    class SoraSprite;
    
    /**
     * To enable post effect, you must enable RenderToBuffer feature in SoraCore
     * SoraCore::Instance()->enableCoreFeature(SoraCore::FeatureRenderToBuffer); etc
     **/

    class SORA_API SoraPostEffect: public SoraObject {
    public:
        SoraPostEffect();
        ~SoraPostEffect();
        
        void attach();
        
        void onBufferRender(SoraTextureHandle texture);
        
        SoraSprite* getSprite() const;
        
        SoraShader* attachFragmentShader(const StringType& file, const SoraString& entry);
        SoraShader* attachVertexShader(const StringType& file, const SoraString& entry);
        
        void detachFragmentShader();
        void detachVertexShader();
        
        void attachFragmentShader(SoraShader* shader);
        void attachVertexShader(SoraShader* shader);
        
    private:
        sora::SoraSprite* mSprite;
    };
    
    
} // namespace sora


#endif
