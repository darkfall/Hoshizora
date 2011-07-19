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
    class SORA_API SoraModifierAdapter: public SoraAbstractModiferAdapter {
    public:
        SoraModifierAdapter(T* obj, bool retain=false) {
            assert(obj != NULL);
            mObj = obj;
            mRetain = retain;
        }
        SoraModifierAdapter(T* obj, SoraModifier<T>* modi, bool retain=false) {
            mObj = obj;
            add(modi);
            mRetain = retain;
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
            } else {
                if(!mRetain)
                    delete this;
            }
        }
        
        void add(SoraModifier<T>* modi) {
            mModifiers.push_back(modi);
        }
        void del(SoraModifier<T>* modi) {
            mModifiers.remove(modi);
        }
        
    protected:
        SoraModifierAdapter();
        
        typedef std::list<SoraModifier<T>*> ModifierList;
		ModifierList mModifiers;
        T* mObj;
    
        bool mRetain;
    };
    
    template<typename MT>
    static SoraModifierAdapter<MT>* CreateModifierAdapter(MT* obj, SoraModifier<MT>* modifier, bool retain=false) {
        return new SoraModifierAdapter<MT>(obj, modifier, retain);
    }
} // namespace sora



#endif
