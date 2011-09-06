//
//  SoraShaderEnabled.h
//  Sora
//
//  Created by Robert Bu on 8/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderEnabled_h
#define Sora_SoraShaderEnabled_h

#include "SoraPlatform.h"
#include "SoraShader.h"
#include "SoraObject.h"
#include "SoraUncopyable.h"

namespace sora {
    
    /**
     * Base class from objects that can render with shader
     **/
    
    class SORA_API SoraShaderEnabledObject: public SoraObject {
    public:
        SoraShaderEnabledObject();
        virtual ~SoraShaderEnabledObject();
        
        SoraShader* attachShader(const util::String& file, const util::String& entry, int32 type);
        SoraShader* attachFragmentShader(const util::String& file, const util::String& entry);
        SoraShader* attachVertexShader(const util::String& file, const util::String& entry);
        
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
