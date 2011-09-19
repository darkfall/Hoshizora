//
//  SoraModifier.h
//  Sora
//
//  Created by Robert Bu on 7/1/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraModifier_h
#define Sora_SoraModifier_h

#include "SoraAutoPtr.h"
#include "SoraFunction.h"

namespace sora {
    
    /**
     *  Base class for Modifiers
     **/
        
    template<class MT>
    class SORA_API SoraModifier {
    public:
        typedef SoraAutoPtr<SoraModifier> PtrType;
        typedef SoraFunction<void(SoraModifier*)> Delegate;
        
        SoraModifier(bool autoRelease=true):
        mAutoRelease(autoRelease) {
        }
        
        virtual ~SoraModifier() {
        }
        
        virtual bool update(float dt) = 0;
        virtual void modify(MT* object) = 0;
        
        virtual SoraModifier<MT>* clone() = 0;
        
        virtual void reset() {
        }
        
        void setFinishDelegate(const Delegate& del) {
            mDelegate = del;
        }
        
        const Delegate& getFinishDelegate() const {
            return mDelegate;
        }
        
        virtual void release() {
            if(isAutoRelease())
                delete this;
        }
        
        void setAutoRelease(bool flag) {
            mAutoRelease = flag;
        }
        
        bool isAutoRelease() const {
            return mAutoRelease;
        }
        
    protected:
        Delegate mDelegate;
        bool mAutoRelease;
    };
    
    template<class MT>
    class SoraModifierList: public SoraModifier<MT> {
    public:
        typedef SoraModifier<MT>* ModifierPtr;
        
        SoraModifierList(bool repeat=false);
        ~SoraModifierList();
        
        void    add(SoraModifier<MT>* modi);
        bool    update(float dt);
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
    bool SoraModifierList<MT>::update(float dt) {
        if(mModifiers.size() != 0) {
            if(mModifiers[mCurrModifier]->update(dt)) {
                ++mCurrModifier;
            
                if(!mRepeat) {
                    if(mCurrModifier >= mModifiers.size()) {
                        clear();

                        if(SoraModifier<MT>::mDelegate) {
                            SoraModifier<MT>::mDelegate(this);
                        }
                        return true;
                    } else 
                        mModifiers[mCurrModifier]->reset();
                } else {
                    if(mCurrModifier >= mModifiers.size())
                        mCurrModifier = 0;
                  
                    mModifiers[mCurrModifier]->reset();
                }
            }
        }
        return false;
    }
    
    template<typename MT>
    void SoraModifierList<MT>::modify(MT* obj) {
        if(mModifiers.size() != 0 && mCurrModifier < mModifiers.size()) {
            mModifiers[mCurrModifier]->modify(obj);
        }
    }
    
    template<typename MT>
    void SoraModifierList<MT>::clear() {
        typename ModifierList::iterator itModifier = mModifiers.begin();
        while(itModifier != mModifiers.end()) {
            if((*itModifier)->isAutoRelease()) {
                (*itModifier)->release();
            }
            ++itModifier;
        }
        mModifiers.clear();
    }
    
    template<typename MT>
    SoraModifier<MT>* SoraModifierList<MT>::clone() {
        SoraModifierList<MT>* c = new SoraModifierList<MT>(mRepeat);
        typename ModifierList::iterator itModifier = mModifiers.begin();
        while(itModifier != mModifiers.end()) {
            SoraModifier<MT>* mod = (*itModifier)->clone();
            if(mod != NULL) {
                c->add(mod);
            }
            ++itModifier;
        }
        c->mDelegate = SoraModifier<MT>::mDelegate;
        c->mCurrModifier = mCurrModifier;
        return c;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, SoraModifier<MT>* eff2, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, 
                                                    SoraModifier<MT>* eff2, 
                                                    SoraModifier<MT>* eff3, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1, 
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
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
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
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
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
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
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
    inline SoraModifierList<MT>* CreateModifierList(SoraModifier<MT>* eff1,  
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
    
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1, 
                                                                const typename SoraModifier<MT>::Delegate& del, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1, 
                                                                SoraModifier<MT>* eff2, 
                                                                const typename SoraModifier<MT>::Delegate& del, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1, 
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                const typename SoraModifier<MT>::Delegate& del, bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1, 
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                SoraModifier<MT>* eff4, 
                                                                const typename SoraModifier<MT>::Delegate& del,  bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1,  
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                SoraModifier<MT>* eff4, 
                                                                SoraModifier<MT>* eff5, 
                                                                const typename SoraModifier<MT>::Delegate& del,  bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1,  
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                SoraModifier<MT>* eff4, 
                                                                SoraModifier<MT>* eff5,
                                                                SoraModifier<MT>* eff6, 
                                                                const typename SoraModifier<MT>::Delegate& del,  bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1,  
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                SoraModifier<MT>* eff4, 
                                                                SoraModifier<MT>* eff5,
                                                                SoraModifier<MT>* eff6, 
                                                                SoraModifier<MT>* eff7, 
                                                                const typename SoraModifier<MT>::Delegate& del,  bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->setFinishDelegate(del);
        return list;
    }
    
    template<class MT>
    inline SoraModifierList<MT>* CreateModifierListWithDelegate(SoraModifier<MT>* eff1,  
                                                                SoraModifier<MT>* eff2, 
                                                                SoraModifier<MT>* eff3, 
                                                                SoraModifier<MT>* eff4, 
                                                                SoraModifier<MT>* eff5,
                                                                SoraModifier<MT>* eff6, 
                                                                SoraModifier<MT>* eff7,
                                                                SoraModifier<MT>* eff8, 
                                                                const typename SoraModifier<MT>::Delegate& del,  bool repeat = false) {
        SoraModifierList<MT>* list = new SoraModifierList<MT>(repeat);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->add(eff8);
        list->setFinishDelegate(del);
        return list;
    }
    
} // namespace sora

#endif
