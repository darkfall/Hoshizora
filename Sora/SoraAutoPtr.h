/*
 *  SoraAutoPtr.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_AUTO_PTR_H_
#define SORA_AUTO_PTR_H_

#include "SoraPlatform.h"
#include "SoraFunction.h"
#include "SoraException.h"
#include "SoraUncopyable.h"

/*
 a simple reference counted auto pointer implemention
 notice: behave more like boost::shared_ptr or tr1::sharedptr but not std::auto_ptr
 if you want auto_ptr, use std::auto_ptr instead
 this is not a std::auto_ptr implemention, but reference counted shared ptr
 */

namespace sora {
	
    /**
     * Some ptr object implementation
     * including SoraAutoPtr, SoraWeakPtr and SoraUniquePtr
     **/
    
    template<typename T, class RP>
	class SoraAutoPtr;
    
    template<typename T>
    class SoraUniquePtr;
    
    template<typename T>
    class SoraWeakPtr;
    
    template<typename T, typename Base>
    static T checked_cast(Base* pointer) {
        T derived = dynamic_cast<T>(pointer);
        if(!derived)
            THROW_SORA_EXCEPTION(RuntimeException, "checked_cast: failed to cast from parent to child");
        return derived;
    }
    
    
    class SoraAutoPtrRefCounter {
    public:
        SoraAutoPtrRefCounter(): mRef(1), mWeakRef(1) {}
        
        void incRef() {
            ++this->mRef;
        }
        
        int decRef() {
            return --this->mRef;
        }
        
        int getRef() const {
            return this->mRef;
        }
        
        void incWeakRef() {
            ++this->mWeakRef;
        }
        
        int decWeakRef() {
            return --this->mWeakRef;
        }
        
        int getWeakRef() const {
            return this->mWeakRef;
        }
        
        void destruct() {
            delete this;
        }
        
        void release() {
            int32 newref = --this->mRef;
            if(newref != 0)
                return;
            
            this->weakRelease();
        }
        
        void weakRelease() {
            int32 newweakref = --this->mWeakRef;
            if(newweakref == 0)
                this->destruct();
        }
        
    private:
        int32 mRef;
        int32 mWeakRef;
    };
    
    template<class C>
    struct SoraAutoPtrReleasePolicy {
        static C* Alloc() {
            return new C();
        }
        
        static void Release(C* obj) {
            if(obj)
                delete obj;
        }
    };
    
    template<class C>
    struct SoraAutoPtrReleaseArrayPolicy {
        static void Release(C* obj) {
            if(obj)
                delete []obj;
        }
    };
    
    template<class C>
    struct SoraAutoPtrFreeReleasePolicy {
        static C* Alloc() {
            return sora_malloc_t(C, 1);
        }
        
        static void Release(C* obj) {
            if(obj)
                sora_free(obj);
        }
    };
    
	template<typename T, class RP=SoraAutoPtrReleasePolicy<T> >
	class SoraAutoPtr {
	public:
        typedef T element_type;
        
		SoraAutoPtr(): 
        mPtr(0), 
        mCounter(new SoraAutoPtrRefCounter),
        mAllocator(),
        mUnallocator() {
        }
		
        SoraAutoPtr(T* t): 
        mPtr(t), 
        mCounter(new SoraAutoPtrRefCounter),
        mAllocator(),
        mUnallocator() { 
        }
        
        SoraAutoPtr(const SoraFunction<T*(void)>& allocator, const SoraFunction<void(T*)>& unallocator): 
        mPtr(0), 
        mCounter(new SoraAutoPtrRefCounter),
        mAllocator(allocator),
        mUnallocator(unallocator) {
        }
		
        SoraAutoPtr(T* t, const SoraFunction<T*(void)>& allocator, const SoraFunction<void(T*)>& unallocator): 
        mPtr(t), 
        mCounter(new SoraAutoPtrRefCounter),
        mAllocator(allocator),
        mUnallocator(unallocator) { 
        }
        
        template<class Other, class OtherRP>
		SoraAutoPtr(const SoraAutoPtr<Other, OtherRP>& rhs): 
        mCounter(rhs.mCounter),
        mPtr(const_cast<Other*>(rhs.get())) {
            this->mCounter->incRef();
        }
        
        SoraAutoPtr(const SoraAutoPtr& rhs):
        mCounter(rhs.mCounter),
        mPtr(rhs.mPtr) {
            this->mCounter->incRef();
        }
        
        ~SoraAutoPtr() { 
            this->release();
        }
		
		SoraAutoPtr& assign(T* _ptr) {
            if(get() != _ptr) {
                SoraAutoPtrRefCounter* tmp = new SoraAutoPtrRefCounter;
                release();
                this->mCounter = tmp;
                this->mPtr = _ptr;
            }
            return *this;
        }
        
        SoraAutoPtr& assign(const SoraAutoPtr& ptr) {
            if(&ptr != this) {
                SoraAutoPtr tmp(ptr);
                swap(tmp);
            }
            return *this;
        }
        
        template<class Other, class OtherRP>
        SoraAutoPtr& assign(const SoraAutoPtr<Other, OtherRP>& _ptr) {
            if(_ptr.get() != this->mPtr) {
                SoraAutoPtr tmp(_ptr);
                swap(tmp);
            }
            return *this;
        }
        
        void reset(T* ptr=0) {
            this->assign(ptr);
        }
        
        // must have initialized with allocator
        void alloc() {
            if(mAllocator) {
                this->reset(mAllocator());
            }
        }
        
		SoraAutoPtr& operator=(const SoraAutoPtr& rhs) { 
			return this->assign(rhs);
		}
		SoraAutoPtr& operator=(T* rhs) {
            return this->assign(rhs);
		}
        
        template<class Other, class OtherRP>
        SoraAutoPtr& operator=(const SoraAutoPtr<Other, OtherRP>& _ptr) {
            return this->assign<Other>(_ptr);
        }
        
        void swap(SoraAutoPtr& rhs) {
            std::swap(this->mPtr, rhs.mPtr);
            std::swap(this->mCounter, rhs.mCounter);
        }
        
        template<class Other, class OtherRP>
        SoraAutoPtr<Other, OtherRP> cast() const {
            Other* other = dynamic_cast<Other*>(this->mPtr);
            if(other) {
                return SoraAutoPtr<Other, OtherRP>(this->mCounter, other);
            }
            return SoraAutoPtr<Other, OtherRP>();
        }
        
        template<class Other, class OtherRP>
		SoraAutoPtr<Other, OtherRP> unsafeCast() const {
            Other* other = static_cast<Other*>(this->mPtr);
            return SoraAutoPtr<Other, OtherRP>(this->mCounter, other);
        }
        
        inline T* get() const {
            return this->mPtr;
        }
        
		inline T* operator->() {
			return this->deref();
		}
        inline T* operator->() const {
            return this->deref();
        }
        
		inline T& operator*() {
			return *this->deref();
		}
        inline T& operator*() const {
			return *this->deref();
		}
        
        operator T*() {
            return this->mPtr;
        }
        operator const T*() const {
            return this->mPtr;
        }
        
        operator bool() {
            return this->mPtr != 0;
        }
        
        bool operator !()const {
            return this->mPtr == 0;
        }
        bool operator ==(const SoraAutoPtr& rhs) const {
            return this->get() != rhs.get();
        }
        bool operator -=(const T* rhs) const {
            return this->get() != rhs;
        }
        bool operator ==(T* rhs) const {
            return this->get() != rhs;
        }
        bool operator !=(const SoraAutoPtr& rhs) const {
            return this->get() != rhs.get();
        }
        bool operator !=(const T* rhs) const {
            return this->get() != rhs;
        }
        bool operator !=(T* rhs) const {
            return this->get() != rhs;
        }
        bool operator>(const SoraAutoPtr& rhs) const {
            return this->get() > rhs.get();
        }
        bool operator>(T* rhs) const {
            return this->get() > rhs;
        }
        bool operator<(const T* rhs) const {
            return this->get() < rhs;
        }
        bool operator>(const T* rhs) const {
            return this->get() > rhs;
        }
        bool operator<(T* rhs) const {
            return this->get() < rhs;
        }
        bool operator<=(const SoraAutoPtr& rhs) const {
            return this->get() < rhs.get();
        }
        bool operator>=(const SoraAutoPtr& rhs) const {
            return this->get() > rhs.get();
        }
        bool operator>=(T* rhs) const {
            return this->get() > rhs;
        }
        bool operator<=(const T* rhs) const {
            return this->get() < rhs;
        }
        bool operator>=(const T* rhs) const {
            return this->get() > rhs;
        }
        bool operator<=(T* rhs) const {
            return this->get() < rhs;
        }
        
        bool isValid() const {
            return this->mPtr!=0;
        }
        bool isNull() const {
            return this->mPtr==0;
        }
		
		int32 use_count() const { 
			return this->mCounter->getRef();
		}
        
		bool operator == (const SoraAutoPtr<T>& rhs) {
			return this->mPtr == rhs.mPtr;
		}
		
        inline T* deref() const {
            if(!this->mPtr)
                THROW_SORA_EXCEPTION(RuntimeException, "ukn::SoraAutoPtr: invalid ptr to deref");
            return this->mPtr;
        }
        
	private:
		void release() {
            sora_assert(this->mCounter);
            int i = this->mCounter->decRef();
            if(i == 0) {
                if(mUnallocator)
                    mUnallocator(this->mPtr);
                else
                    RP::Release(this->mPtr);
                this->mPtr = 0;
                
                this->mCounter->release();
            }
        }
        
        SoraAutoPtr(SoraAutoPtrRefCounter* pCounter, T* pPtr):
        mCounter(pCounter), mPtr(pPtr) {
            sora_assert(pCounter);
            pCounter->incRef();
        }
        
    private:
        T* mPtr;
        SoraAutoPtrRefCounter* mCounter;
        
        template<typename OT>
        friend class SoraWeakPtr;
        
        SoraAutoPtr(T* ptr, SoraAutoPtrRefCounter* ref):
        mPtr(ptr),
        mCounter(ref) {
            if(mCounter)
                mCounter->incRef();
        }
        
        SoraFunction<T*(void)> mAllocator;
        SoraFunction<void(T*)> mUnallocator;
        
        template<class OC, class ORP>
        friend class SoraAutoPtr;
	};
    
    template <class C, class RP>
    inline void swap(SoraAutoPtr<C, RP>& p1, SoraAutoPtr<C, RP>& p2) {
        p1.swap(p2);
    }
    
    template<typename T>
    struct COMReleasePolicy {
        static void Release(T* pointer) {
            T::Release();
        }
    };
    
    template<typename T>
    static SoraAutoPtr<T> MakeCOMPtr(T* pointer) {
        return SoraAutoPtr<T, COMReleasePolicy<T> >(pointer);
    }
	
	template<typename T>
	class EnableSharedPtrFromThis: public SoraAutoPtr<T> {
	public:
		typedef SoraAutoPtr<T> Ptr;
	};
    
    template<typename T>
    SoraAutoPtr<T> MakeSharedPtr() {
        return SoraAutoPtr<T>(new T());
    }
    
    template<typename T, typename A1>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1) {
        return SoraAutoPtr<T>(new T(a1));
    }
    
    template<typename T, typename A1, typename A2>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2) {
        return SoraAutoPtr<T>(new T(a1, a2));
    }
    
    template<typename T, typename A1, typename A2, typename A3>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3) {
        return SoraAutoPtr<T>(new T(a1, a2, a3));
    }
    
    template<typename T, typename A1, typename A2, typename A3, typename A4>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3, A4 a4) {
        return SoraAutoPtr<T>(new T(a1, a2, a3, a4));
    }
    
    template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        return SoraAutoPtr<T>(new T(a1, a2, a3, a4, a5));
    }
    
    template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
        return SoraAutoPtr<T>(new T(a1, a2, a3, a4, a5, a6));
    }
    
    template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
        return SoraAutoPtr<T>(new T(a1, a2, a3, a4, a5, a6, a7));
    }
    
    template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    SoraAutoPtr<T> MakeSharedPtr(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
        return SoraAutoPtr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8));
    }
    
    
    template<class T>
    class SoraUniquePtr: SoraUncopyable {
    public:
        typedef T element_type;
        
        explicit SoraUniquePtr(T* _ptr):
        ptr(_ptr) {
        }
        
        SoraUniquePtr():
        ptr(0) {
            
        }
        
        ~SoraUniquePtr() {
            if(this->ptr) {
                delete this->ptr;
                this->ptr = 0;
            }
        }
        
        void reset(T* p = 0) {
            this->ptr = p;
        }
        
        T& operator*() const {
            sora_assert(this->ptr != 0);
            return *this->ptr;
        }
        T* operator->() const {
            return this->ptr;
        }
        
        T* get() const {
            return this->ptr;
        }
        
        operator bool() const {
            return ptr != 0;
        }
        
        void swap(SoraUniquePtr& b);
        
    private:
        T* ptr;
    };
    
    template<class T>
    void swap(SoraUniquePtr<T>& a, SoraUniquePtr<T>& b) {
        std::swap(a.ptr, b.ptr);
    }
    
    template<typename T>
    class SoraWeakPtr {
    public:
        typedef T element_type;
        
        SoraWeakPtr():
        mPtr(0),
        mCounter(0) {
            
        }
        
        SoraWeakPtr(const SoraWeakPtr<T>& ptr):
        mCounter(ptr.mCounter) {
            if(mCounter) {
                mCounter->incWeakRef();
                mPtr = ptr.lock().get();
            }
        }
        
        template<class Y> SoraWeakPtr(const SoraAutoPtr<Y>& ptr):
        mPtr(ptr.get()),
        mCounter(ptr.mCounter) {
            if(mCounter) {
                mCounter->incWeakRef();
            }
        }
        
        template<class Y> SoraWeakPtr(const SoraWeakPtr<Y>& ptr):
        mCounter(ptr.mCounter) {
            if(mCounter) {
                mCounter->incWeakRef();
                mPtr = ptr.lock().get();
            }
        }
        
        ~SoraWeakPtr() {
            if(mCounter) {
                mCounter->decWeakRef();
            }
        }
        
        SoraWeakPtr& operator=(const SoraWeakPtr& r) {
            mPtr = r.lock().get();
            mCounter = r.mCounter;
            if(mCounter)
                mCounter->incWeakRef();
            return *this;
        }
        
        template<class Y> SoraWeakPtr & operator=(const SoraWeakPtr<Y>& r) {
            mPtr = r.lock().get();
            mCounter = r.mCounter;
            if(mCounter)
                mCounter->incWeakRef();
            return *this;
        }
        
        template<class Y> SoraWeakPtr & operator=(const SoraAutoPtr<Y>& r) {
            mPtr = r.get();
            mCounter = r.mCounter;
            
            if(mCounter)
                mCounter->incWeakRef();
            return *this;
        }
        
        int32 use_count() const {
            return mCounter ? mCounter->getRef() : 0;
        }
        
        int32 weak_count() const {
            return mCounter ? mCounter->getWeakRef() : 0;
        }
        
        bool expired() const {
            return (mCounter && mCounter->getRef() == 0) || !mCounter;
        }
        
        SoraAutoPtr<T> lock() const {
            return SoraAutoPtr<T>(mPtr, mCounter);
        }
        
        void reset() {
            mPtr = 0;
            if(mCounter) {
                mCounter->decWeakRef();
            }
            mCounter = 0;
        }
        
        void swap(SoraWeakPtr<T> & b) {
            std::swap(mPtr, b.mPtr);
            std::swap(mCounter, b.mCounter);
        }
        
    private:
        T* mPtr;
        SoraAutoPtrRefCounter* mCounter;
        
        template<class Y>
        friend class SoraWeakPtr;
    };
    
    template<class T>
    void swap(SoraWeakPtr<T>& a, SoraWeakPtr<T>& b) {
        std::swap(a.mPtr, b.mPtr);
        std::swap(a.mCounter, b.mCounter);
    }
    
    template<typename T>
    class SoraSharedClass {
    public:
        typedef SoraAutoPtr<T> Ptr;
    };

		
} // namespace sora
#endif