//
//  SoraShaderManager.h
//  Sora
//
//  Created by Robert Bu on 7/14/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderManager_h
#define Sora_SoraShaderManager_h

#include "SoraPlatform.h"
#include "SoraShader.h"
#include "SoraStringId.h"
#include <map>

namespace sora {
    
    class SORA_API SoraShaderManager {
    protected:
        SoraShaderManager();
        ~SoraShaderManager();
        
    public:
		static SoraShaderManager* Instance();
        
        bool init();

        SoraShader* createShader(const StringType& file, const SoraString& entry, int32 type);
        SoraShader* createShaderFromMem(const char* data, const SoraString& entry, int32 type);
      
        void freeShader(SoraShader* shader);
        
    private:
        SoraShaderContext* mShaderContext;
    };

    
} // namespace sora



#endif
