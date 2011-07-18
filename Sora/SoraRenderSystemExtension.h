//
//  SoraRenderSystemExtension.h
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraRenderSystemExtension_h
#define Sora_SoraRenderSystemExtension_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include <map>

namespace sora {
    
    /**
     *  Defines the Extension part of SoraRenderSystem
     *  Implemention of RenderSystems could choose to implement these features or not
     *  RenderSystems must tell the class whether to support specific features
     *  Such as SoraRenderSystemExtension::Instance()->registerExtension(SORA_EXTENSION_FSAA, 4);
     *  Extension feature list maybe extended during the development process
     *  As I'm learning new things
     **/
    enum ExtensionFeature {
        SORA_EXTENSION_FSAA = 0x0001,
    };
        
    class SORA_API SoraRenderSystemExtension {
    protected:        
        SoraRenderSystemExtension();
        ~SoraRenderSystemExtension();
        
    public:
        static SoraRenderSystemExtension* Instance();
        static void Destroy();
        
        void    registerExtension(ExtensionFeature extension);
        int32   getExtensionParam(ExtensionFeature extension);
        void    setExtensionParam(ExtensionFeature extension, int32 param);

        // notify the RenderSystem to change the extension state
        void    enableExtension(ExtensionFeature extension);
        void    disableExtension(ExtensionFeature extension);
        bool    isExtensionEnabled(ExtensionFeature extension);
    
    private:
        static SoraRenderSystemExtension* mInstance;
        
        void setEnvValue(ExtensionFeature extension, int32 param);
        
        typedef std::map<ExtensionFeature, int32> ExtensionFeatureMap;
        ExtensionFeatureMap m_Extensions;
    };
    
    static void EnableRenderSystemExtension(ExtensionFeature extension) {
        SoraRenderSystemExtension::Instance()->enableExtension(extension);
    }
    
    static void DisableRenderSystemExtension(ExtensionFeature extension) {
        SoraRenderSystemExtension::Instance()->disableExtension(extension);
    }
    
    static int32 GetRenderSystemExtensionParam(ExtensionFeature extension) {
        return SoraRenderSystemExtension::Instance()->getExtensionParam(extension);
    }
    
    static void SetRenderSystemExtensionParam(ExtensionFeature extension, int32 param) {
        return SoraRenderSystemExtension::Instance()->setExtensionParam(extension, param);
    }
    
    static bool IsRenderSystemExtensionEnabled(ExtensionFeature extension) {
        return SoraRenderSystemExtension::Instance()->isExtensionEnabled(extension);
    }

} // namespace sora


#endif
