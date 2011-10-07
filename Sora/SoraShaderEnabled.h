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
#include "SoraObject.h"
#include "SoraUncopyable.h"
#include "SoraString.h"

namespace sora {
    
    class SoraShader;
    class SoraShaderContext;
    
    /**
     * Base class from objects that can render with shader
     **/
    
    class SORA_API SoraShaderEnabledObject: public SoraObject {
    public:
        SoraShaderEnabledObject();
        virtual ~SoraShaderEnabledObject();
        
        void attachShaderContext(const StringType& tag);
        void attachShaderContext(SoraShaderContext* context);
        SoraShaderContext* getShaderContext() const;
        
        SoraShader* attachShader(const StringType& file, const SoraString& entry, int32 type);
        SoraShader* attachFragmentShader(const StringType& file, const SoraString& entry);
        SoraShader* attachVertexShader(const StringType& file, const SoraString& entry);
        
        void detachFragmentShader();
        void detachVertexShader();
        
        void attachShaderToRender();
        void detachShaderFromRender();
        
        SoraShader* getFragmentShader() const;
        SoraShader* getVertexShader() const;
        
        bool hasShader() const;
        void clearShader();
        
    private:
        void detachShader(SoraShader* shader);
        void attachShader(SoraShader* shader);
        void attachFragmentShader(SoraShader* shader);
        void attachVertexShader(SoraShader* shader);
        
    private:
        SoraShaderEnabledObject(const SoraShaderEnabledObject&);
        SoraShaderEnabledObject& operator=(const SoraShaderEnabledObject&);
        
        inline void checkShaderContext();
        
        bool mInternalContext;
        SoraShaderContext* mShaderContext;
    };
    
}


#endif
