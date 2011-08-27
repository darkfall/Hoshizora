//
//  SoraShaderEnabled.h
//  Sora
//
//  Created by Ruiwei Bu on 8/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderEnabled_h
#define Sora_SoraShaderEnabled_h

#include "SoraPlatform.h"
#include "SoraShader.h"
#include "SoraObject.h"
#include "uncopyable.h"

namespace sora {
    
    /**
     * Base class from objects that can render with shader
     **/
    
    class SORA_API SoraShaderEnabledObject: public SoraObject {
    public:
        SoraShaderEnabledObject();
        virtual ~SoraShaderEnabledObject();
        
        SoraShader* attachShader(const SoraWString& file, const SoraString& entry, int32 type);
        SoraShader* attachFragmentShader(const SoraWString& file, const SoraString& entry);
        SoraShader* attachVertexShader(const SoraWString& file, const SoraString& entry);
        
        void detachShader(SoraShader* shader);
        void detachFragmentShader();
        void detachVertexShader();
        
        void attachShader(SoraShader* shader);
        void attachFragmentShader(SoraShader* shader);
        void attachVertexShader(SoraShader* shader);
        
        void attachShaderToRender();
        void detachShaderFromRender();
        
        bool hasShader() const;
        void clearShader();
        
    private:
        inline void checkShaderContext();
        
        SoraShaderContext* mShaderContext;
    };
    
}


#endif
