//
//  SoraModifier.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraModifier_h
#define Sora_SoraModifier_h

#include "SoraAutoPtr.h"

namespace sora {
    
    /**
     *  Base class for Modifiers
     **/
    enum {
        ModifierUpdateEnd = -1
    };
        
    template<class MT>
    class SORA_API SoraModifier {
    public:
        typedef SoraAutoPtr<SoraModifier> Ptr;
        
        virtual int32   update(float32 dt) = 0;
        virtual void    modify(MT* object) = 0;
        
        virtual SoraModifier<MT>* clone() = 0;
        
        virtual void reset() {}
    };
    
    template<class MT>
    class SORA_API SoraModifierList: public SoraModifier<MT> {
    public:
        typedef SoraModifier<MT>* ModifierPtr;
        
        SoraModifierList(bool repeat=false);
        ~SoraModifierList();
        
        void    add(SoraModifier<MT>* modi);
        int32   update(float32 dt);
        void    modify(MT* obj);
        void    clear();
        
        SoraModifier<MT>* clone();
        
    private:
        typedef std::vector<ModifierPtr> ModifierList;
        ModifierList mModifiers;
        int32 mCurrModifier;
        bool mRepeat;
    };
    
    template<typename MT>
    SoraModifierList<MT>::SoraModifierList(bool repeat):
    mCurrModifier(0),
    mRepeat(repeat) {
        
    }
    
    template<typename MT>
    SoraModifierList<MT>::~SoraModifierList() {
        clear();
    }
    
    template<typename MT>
    void SoraModifierList<MT>::add(SoraModifier<MT>* modi) {
        mModifiers.push_back(modi);
    }
    
    template<typename MT>
    int32 SoraModifierList<MT>::update(float32 dt) {
        if(mModifiers.size() != 0) {
            int32 result = mModifiers[mCurrModifier]->update(dt);
            if(result == ModifierUpdateEnd) {
                ++mCurrModifier;
            
                if(!mRepeat) {
                    if(mCurrModifier >= mModifiers.size()) {
                        clear();
                    } else 
                        mModifiers[mCurrModifier]->reset();
                } else {
                    if(mCurrModifier >= mModifiers.size())
                        mCurrModifier = 0;
                  
                    mModifiers[mCurrModifier]->reset();
                }
            }
        }
        return 0;
    }
    
    template<typename MT>
    void SoraModifierList<MT>::modify(MT* obj) {
        if(mModifiers.size() != 0 && mCurrModifier < mModifiers.size()) {
            mModifiers[mCurrModifier]->modify(obj);
        }
    }
    
    template<typename MT>
    void SoraModifierList<MT>::clear() {
        for(size_t i=0; i<mModifiers.size(); ++i)
            delete mModifiers[i];
        mModifiers.clear();
    }
    
    template<typename MT>
    SoraModifier<MT>* SoraModifierList<MT>::clone() {
        return NULL;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, SoraModifier<MT>* eff2, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, 
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, 
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, 
                                                    SoraModifier<MT>* eff4, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, 
                                                    SoraModifier<MT>* eff4, 
                                                    SoraModifier<MT>* eff5, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, 
                                                    SoraModifier<MT>* eff4, 
                                                    SoraModifier<MT>* eff5,
                                                    SoraModifier<MT>* eff6, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, 
                                                    SoraModifier<MT>* eff4, 
                                                    SoraModifier<MT>* eff5,
                                                    SoraModifier<MT>* eff6, 
                                                    SoraModifier<MT>* eff7, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        return list;
    }
    
    template<class MT>
    static SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, 
                                                    SoraModifier<MT>* eff4, 
                                                    SoraModifier<MT>* eff5,
                                                    SoraModifier<MT>* eff6, 
                                                    SoraModifier<MT>* eff7,
                                                    SoraModifier<MT>* eff8, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->add(eff8);
        return list;
    }
    
} // namespace sora

#endif
