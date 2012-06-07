//
//  SoraShaderParamModifier.cpp
//  Sora
//
//  Created by Robert Bu on 10/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShaderParamAction.h"
#include "SoraShader.h"
#include "math/SoraMathCommon.h"

namespace sora {
    
    SoraShaderParamAction::SoraShaderParamAction(const std::string& name, float* from, float* to, uint32 size, float inTime) {
        sora_assert(from);
        sora_assert(to);
        sora_assert(size > 0);
        
        mSize = size;
        mName = name;
        
        mFrom = sora_malloc_t(float, size);
        mTo = sora_malloc_t(float, size);
        mCurr = sora_malloc_t(float, size);
        
        memcpy(mFrom, from, sizeof(float)*size);
        memcpy(mTo, to, sizeof(float)*size);
    }
    
    SoraShaderParamAction::~SoraShaderParamAction() {
        if(mFrom)
            sora_free(mFrom);
        if(mTo)
            sora_free(mTo);
        if(mCurr)
            sora_free(mCurr);
    }
    
    SoraAction::Ptr SoraShaderParamAction::ActionWithParam(const std::string& name, float* from, float* to, uint32 size, float inTime) {
        return new SoraShaderParamAction(name, from, to, size, inTime);
    }
    
    bool SoraShaderParamAction::isDone() const {
        return !mShader || SoraFiniteAction::isDone();
    }
    
    void SoraShaderParamAction::step(float dt) {
        SoraFiniteAction::step(dt);

        if(mShader) {
            for(uint32 i=0;i <mSize; ++i) {
                mCurr[i] = lerp(mFrom[i], mTo[i], getCurrentTime() / getDuration());
            }
            mShader->setParameterfv(mName.c_str(), mCurr, mSize);
        }
    }
    
    void SoraShaderParamAction::startWithTarget(SoraObject* target) {
        memcpy(mCurr, mFrom, sizeof(float)*mSize);
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    SoraAction::Ptr SoraShaderParamAction::reverse() const {
        return new SoraShaderParamAction(mName, mTo, mFrom, mSize, getDuration());
    }
    
} // namespace sora
