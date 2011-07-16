//
//  SoraDelegate.h
//  Sora
//
//  Created by Ruiwei Bu on 7/8/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraDelegate_h
#define Sora_SoraDelegate_h

#include <cassert>

#include "SoraTimestamp.h"

#include "SoraAutoPtr.h"

namespace sora {
    
    /**
     *  Base class for delegate
     */
    template<typename TARG>
    class SORA_API SoraAbstractDelegate {
    public:
        SoraAbstractDelegate(void* target): mTarget(target) {
        //    assert(target != NULL);
        }
        
        SoraAbstractDelegate(const SoraAbstractDelegate& del): mTarget(del.mTarget) {
            assert(mTarget != NULL);
        }
        
        virtual ~SoraAbstractDelegate() {}
        
        virtual bool notify(const void* sender, TARG& arguments) = 0;
        virtual SoraAbstractDelegate* clone() const = 0;
        
        bool operator <(const SoraAbstractDelegate<TARG>& rhs) const {
            return mTarget < rhs.mTarget;
        }
    
        void* target() const {
            return mTarget;
        }
        
    protected:
        void* mTarget;
    };
    
    template<class TOBJ, class TARGS, bool withSenter=true>
    class SORA_API SoraDelegate: public SoraAbstractDelegate<TARGS> {
    public:
        typedef void (TOBJ::*NotifyMethod)(const void*, TARGS&);
        
        SoraDelegate(TOBJ* obj, NotifyMethod method):
        SoraAbstractDelegate<TARGS>(obj),
        mReceiver(obj),
        mReceiverMethod(method) {
        }
        
        SoraDelegate(const SoraDelegate& delegate):
        SoraAbstractDelegate<TARGS>(delegate),
        mReceiver(delegate.mReceiver),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraDelegate() {
            
        }
        
        SoraDelegate& operator = (const SoraDelegate& delegate) {
            if(&delegate != this) {
                this->mTarget = delegate.mTarget;
                this->mReceiver = delegate.mReceiver;
                this->mReceiverMethod = delegate.mReceiverMethod;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            (mReceiver->*mReceiverMethod)(sender, args);
            return true;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraDelegate(*this);
        }
        
    protected:
        TOBJ* mReceiver;
        NotifyMethod mReceiverMethod;
        
    private:
        SoraDelegate();
    };
    
    template<class TOBJ, class TARGS>
    class SORA_API SoraDelegate<TOBJ, TARGS, false>: public SoraAbstractDelegate<TARGS> {
    public:
        typedef void (TOBJ::*NotifyMethod)(TARGS&);
        
        SoraDelegate(TOBJ* obj, NotifyMethod method):
        SoraAbstractDelegate<TARGS>(obj),
        mReceiver(obj),
        mReceiverMethod(method) {
        }
        
        SoraDelegate(const SoraDelegate& delegate):
        SoraAbstractDelegate<TARGS>(delegate),
        mReceiver(delegate.mReceiver),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraDelegate() {
            
        }
        
        SoraDelegate& operator = (const SoraDelegate& delegate) {
            if(&delegate != this) {
                this->mTarget = delegate.mTarget;
                this->mReceiver = delegate.mReceiver;
                this->mReceiverMethod = delegate.mReceiverMethod;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            (mReceiver->*mReceiverMethod)(args);
            return true;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraDelegate(*this);
        }
        
    protected:
        TOBJ* mReceiver;
        NotifyMethod mReceiverMethod;
        
    private:
        SoraDelegate();
    };
    
    // enables to use a C Function as Delegate
    template<class TARGS, bool hasSender=true, bool senderIsConst=false>
    class SORA_API SoraFunctionDelegate: public SoraAbstractDelegate<TARGS> {
    public:
        typedef void (*NotifyMethod)(const void*, TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
        
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            (*mReceiverMethod)(sender, args);
            return true;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };
    
    template<class TARGS>
    class SoraFunctionDelegate<TARGS, true, false>: public SoraAbstractDelegate<TARGS> {
    public:
        typedef void (*NotifyMethod)(void* sender, TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
            
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            void* ncsender = const_cast<void*>(sender);
            (*mReceiverMethod)(ncsender, args);
            return true;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };

    template<class TARGS, bool senderIsConst>
    class SORA_API SoraFunctionDelegate<TARGS, false, senderIsConst>: public SoraAbstractDelegate<TARGS> {
    public:
        typedef void (*NotifyMethod)(TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
            
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            (*mReceiverMethod)(args);
            return true;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };
    
    template<class TARGS>
    class SORA_API SoraExpireDelegate: public SoraAbstractDelegate<TARGS> {
    public:
        SoraExpireDelegate():
        SoraAbstractDelegate<TARGS>(NULL),
        mDelegate(NULL),
        mExpire(0) {}
        
        SoraExpireDelegate(const SoraAbstractDelegate<TARGS>& p, SoraTimestamp::TimeDiff expireMilliseconds):
        SoraAbstractDelegate<TARGS>(p),
        mDelegate(p.clone()),
        mExpire(expireMilliseconds*1000) {
        
        }
        
        SoraExpireDelegate(const SoraExpireDelegate& exp):
        SoraAbstractDelegate<TARGS>(exp),
        mDelegate(exp.mDelegate->clone()),
        mExpire(exp.mExpire),
        mCreationTime(exp.mCreationTime) {
        }
        
        ~SoraExpireDelegate() {
            destroy();
        }
        
        SoraExpireDelegate& operator=(const SoraExpireDelegate& exp) {
            if(&exp != this) {
                delete this->delegate;
                this->mDelegate = exp.mDelegate->clone();
                this->mExpire = exp.mExpire;
                this->mCreationTime = exp.mCreationTime;
                this->mTarget = exp.mTarget;
            }
            return *this;
        }
        
        bool notify(const void* sender, TARGS& args) {
            if(!expired() && mDelegate) {
                return this->mDelegate->notify(sender, args);
            } else
                return false;
        }
        
        SoraAbstractDelegate<TARGS>* clone() const {
            return new SoraExpireDelegate(this);
        }
        
        void destroy() {
            if(this->mDelegate) {
                delete this->mDelegate;
                this->mDelegate = NULL;
            }
        }
        
        const SoraAbstractDelegate<TARGS>& getDelegate() const {
            return this->mDelegate;
        }
        
    protected:
        bool expired() const {
            if(mExpire == 0)
                return false;
            return mCreationTime.isElapsed(mExpire);
        }
        
        SoraAbstractDelegate<TARGS>* mDelegate;
        SoraTimestamp::TimeDiff mExpire;
        SoraTimestamp mCreationTime;
        
    private:
    };
    
    template<class TOBJ, class TARGS>
    static SoraDelegate<TOBJ, TARGS, true> Delegate(TOBJ* obj, void (TOBJ::*NotifyMethod)(void*, TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, true>(obj, NotifyMethod);
    }
                            
    template<class TOBJ, class TARGS>
    static SoraDelegate<TOBJ, TARGS, false> Delegate(TOBJ* obj, void (TOBJ::*NotifyMethod)(TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, false>(obj, NotifyMethod);
    }
    
    template<class TARGS>
    static SoraFunctionDelegate<TARGS, true, true> Delegate(void (*NotifyMethod)(const void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, true, true>(NotifyMethod);
    }
    
    template<class TARGS>
    static SoraFunctionDelegate<TARGS, true, false> Delegate(void (*NotifyMethod)(void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, true, false>(NotifyMethod);
    }
    
    template<class TARGS>
    static SoraFunctionDelegate<TARGS, false> Delegate(void (*NotifyMethod)(TARGS&)) {
        return SoraFunctionDelegate<TARGS, false>(NotifyMethod);
    }
    
    template<class TOBJ, class TARGS>
    static SoraExpireDelegate<TARGS> Delegate(TOBJ* obj, void (TOBJ::*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TOBJ, TARGS, true>(obj, NotifyMethod), expireMs);
    }
    
    template<class TOBJ, class TARGS>
    static SoraExpireDelegate<TARGS> Delegate(TOBJ* obj, void (TOBJ::*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TOBJ, TARGS, false>(obj, NotifyMethod), expireMs);
    }
    
    template<class TARGS>
    static SoraExpireDelegate<TARGS> Delegate(void (*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, true, true>(NotifyMethod), expireMs);
    }
    
    template<class TARGS>
    static SoraExpireDelegate<TARGS> Delegate(void (*NotifyMethod)(void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, true, false>(NotifyMethod), expireMs);
    }
    
    template<class TARGS>
    static SoraExpireDelegate<TARGS> Delegate(void (*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, false>(NotifyMethod), expireMs);
    }
    
    
    
    template<class TOBJ, class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(TOBJ* obj, void (TOBJ::*NotifyMethod)(void*, TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, true>(obj, NotifyMethod).clone();
    }
    
    template<class TOBJ, class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(TOBJ* obj, void (TOBJ::*NotifyMethod)(TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, false>(obj, NotifyMethod).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(const void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, true, true>(NotifyMethod).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, true, false>(NotifyMethod).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(TARGS&)) {
        return SoraFunctionDelegate<TARGS, false>(NotifyMethod).clone();
    }
    
    template<class TOBJ, class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(TOBJ* obj, void (TOBJ::*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TOBJ, TARGS, true>(obj, NotifyMethod), expireMs).clone();
    }
    
    template<class TOBJ, class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(TOBJ* obj, void (TOBJ::*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TOBJ, TARGS, false>(obj, NotifyMethod), expireMs).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, true, true>(NotifyMethod), expireMs).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, true, false>(NotifyMethod), expireMs).clone();
    }
    
    template<class TARGS>
    static SoraAbstractDelegate<TARGS>* DelegatePtr(void (*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS>(Delegate<TARGS, false>(NotifyMethod), expireMs).clone();
    }
                            
} // namespace sora


#endif
