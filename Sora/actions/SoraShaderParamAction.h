//
//  SoraShaderParamModifier.h
//  Sora
//
//  Created by Robert Bu on 10/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderParamModifier_h
#define Sora_SoraShaderParamModifier_h

#include "SoraPlatform.h"
#include "SoraAction.h"

namespace sora {
    
    class SoraShader;
    
    class SoraShaderParamAction: public SoraFiniteAction {
    public:
        SoraShaderParamAction(const std::string& name, float* from, float* to, uint32 size, float inTime);
        virtual ~SoraShaderParamAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual bool isDone() const;
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithParam(const std::string& name, float* from, float* to, uint32 size, float inTime);
        
    private:
        float* mFrom;
        float* mTo;
        float* mCurr;
        uint32 mSize;
        std::string mName;
        SoraShader* mShader;
    };
        
} // namespace sora


#endif
