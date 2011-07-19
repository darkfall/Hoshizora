//
//  SoraRenderSystemExtension.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraRenderSystemExtension.h"
#include "SoraCore.h"

namespace sora {
    
    SoraRenderSystemExtension* SoraRenderSystemExtension::mInstance = NULL;
    
    SoraRenderSystemExtension* SoraRenderSystemExtension::Instance() {
        if(!mInstance)
            mInstance = new SoraRenderSystemExtension;
        return mInstance;
    }
    
    void SoraRenderSystemExtension::Destroy() {
        if(mInstance) {
            delete mInstance;
			mInstance = NULL;
		}
    }
    
    SoraRenderSystemExtension::SoraRenderSystemExtension() {
        
    }
    
    SoraRenderSystemExtension::~SoraRenderSystemExtension() {
        
    }
    
    void SoraRenderSystemExtension::registerExtension(ExtensionFeature extension) {
        m_Extensions[extension] = 0;
    }
    
    int32 SoraRenderSystemExtension::getExtensionParam(ExtensionFeature extension) {
        ExtensionFeatureMap::iterator ext = m_Extensions.find(extension);
        if(ext != m_Extensions.end())
            return ext->second;
        return -1;
    }
    
    void SoraRenderSystemExtension::setExtensionParam(ExtensionFeature extension, int32 param) {
        ExtensionFeatureMap::iterator ext = m_Extensions.find(extension);
        if(ext != m_Extensions.end()) {
            SoraCore::Instance()->getRenderSystem()->onExtensionStateChanged(extension, true, param);
            ext->second = param;
        }
    }
    
    void SoraRenderSystemExtension::enableExtension(ExtensionFeature extension) {
        ExtensionFeatureMap::iterator ext = m_Extensions.find(extension);
        if(ext != m_Extensions.end())
            SoraCore::Instance()->getRenderSystem()->onExtensionStateChanged(extension, true, ext->second);
    }
    
    void SoraRenderSystemExtension::disableExtension(ExtensionFeature extension) {
        ExtensionFeatureMap::iterator ext = m_Extensions.find(extension);
        if(ext != m_Extensions.end())
            SoraCore::Instance()->getRenderSystem()->onExtensionStateChanged(extension, false, ext->second);
    }
    
    bool SoraRenderSystemExtension::isExtensionEnabled(ExtensionFeature extension) {
        ExtensionFeatureMap::iterator ext = m_Extensions.find(extension);
        return ext != m_Extensions.end();
    }
    
    void SoraRenderSystemExtension::setEnvValue(ExtensionFeature extension, int32 param) {
        switch(extension) {
            case SORA_EXTENSION_FSAA:
                SET_ENV_INT("FSAA", param);
                break;
        }
    }
    
} // namespace sora