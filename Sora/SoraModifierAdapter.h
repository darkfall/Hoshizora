//
//  SoraImageEffectAutomator.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraImageEffectAutomator_h
#define Sora_SoraImageEffectAutomator_h

#include "SoraPlatform.h"
#include "SoraModifier.h"
#include "SoraFunction.h"
#include <list>

namespace sora {
    
    /**
     *  Adapter that helps apply modifiers to any class that can manage a modifier
     *  Such as SoraObject
     **/
    
    class SORA_API SoraAbstractModifierAdapter {
    public:
        virtual ~SoraAbstractModifierAdapter() {}

        virtual bool update(float dt) = 0;
    };
    
    template<typename T>
    class SORA_API SoraModifierAdapter: public SoraAbstractModifierAdapter {
    public:
        SoraModifierAdapter(T* obj, SoraModifier<T>* modi, bool insert=false) {
            mObj = obj;
            mModifier = modi;
            
            mInsert = insert;
            if(mInsert)
                obj->addModifierAdapter(this);
        }
        
        virtual ~SoraModifierAdapter() {
            if(mInsert)
                mObj->removeModifierAdapter(this);
        }
        
        typedef SoraFunction<void(T*)> FinishNotification;
        void setFinishNotification(const FinishNotification& func) {
            mOnFinish = func;
        }
        
        bool update(float dt) {
            if(mModifier) {
                bool result = mModifier->update(dt);
                mModifier->modify(mObj);
                
                if(result) { 
                    if(mOnFinish) {
                        mOnFinish(mObj);
                    }
                    
                    if(mModifier->isAutoRelease()) {
                        mModifier->release();
                        mModifier = 0;
                        return true;
                    }
                }
            }
            return false;
        }
        
    protected:
        SoraModifierAdapter();
        
        T* mObj;
        SoraModifier<T>* mModifier;
        
        FinishNotification mOnFinish;
    
        bool mInsert;
    };
    
    template<typename MT>
    inline SoraModifierAdapter<MT>* CreateModifierAdapter(MT* obj, SoraModifier<MT>* modifier, bool insert=false) {
        return new SoraModifierAdapter<MT>(obj, modifier, insert);
    }
    
    template<typename MT>
    inline SoraModifierAdapter<MT>* CreateModifierAdapterWithNotification(MT* obj, 
                                                                          SoraModifier<MT>* modifier, 
                                                                          const SoraFunction<void(MT*)>& notify,
                                                                          bool insert=false) {
        SoraModifierAdapter<MT>* adapter = new SoraModifierAdapter<MT>(obj, modifier, insert);
        adapter->setFinishNotification(notify);
        return adapter;
    }
} // namespace sora



#endif
