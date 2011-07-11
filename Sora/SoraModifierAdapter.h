//
//  SoraImageEffectAutomator.h
//  Sora
//
//  Created by Ruiwei Bu on 7/11/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraImageEffectAutomator_h
#define Sora_SoraImageEffectAutomator_h

#include "SoraPlatform.h"
#include "SoraImageEffect.h"
#include "AutoContainer.h"

namespace sora {
    
    /**
     *  Adapter that helps apply modifiers to any class
     *  Such as SoraObject
     **/
    
    class SORA_API SoraAbstractModiferAdapter: public AutoListElement<SoraAbstractModiferAdapter> {
    public:
        virtual void update(float32 dt) = 0;
    };
    
    template<typename T>
    class SORA_API SoraModiferAdapter: public SoraAbstractModiferAdapter {
    public:
        SoraModiferAdapter(T* obj) {
            assert(obj != NULL);
            mObj = obj;
        }
        void update(float32 dt) {
            if(!mModifiers.empty()) {
                typename ModifierList::iterator itModifier = mModifiers.begin();
                while(itModifier != mModifiers.end()) {
                    int32 result = (*itModifier)->update(dt);
                    (*itModifier)->modify(mObj);
                    
                    if(result == sora::ModifierUpdateEnd) {     
                        delete (*itModifier);
                        (*itModifier) = 0;
                        
                        itModifier = mModifiers.erase(itModifier);
                        continue;
                    }
                    ++itModifier;
                }
            } else 
                delete this;
        }
        
        void add(SoraModifier<T>* modi) {
            mModifiers.push_back(modi);
        }
        void del(SoraModifier<T>* modi) {
            mModifiers.remove(modi);
        }
        
    protected:
        typedef std::list<SoraModifier<T>*> ModifierList;
		ModifierList mModifiers;
        T* mObj;
    };
    
} // namespace sora



#endif
