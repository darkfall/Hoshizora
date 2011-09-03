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
#include "SoraException.h"

/*
 a simple reference counted auto pointer implemention
 notice: behave more like boost::shared_ptr or tr1::sharedptr but not std::auto_ptr
 if you want auto_ptr, use std::auto_ptr instead
 this is not a std::auto_ptr implemention, but reference counted shared ptr
 */

namespace sora {
	
#define sora_safe_delete(ptr) { if(ptr) delete ptr; ptr=0; }
    
    namespace autoptr {
        
        class RefCounter {
        public:
            RefCounter(): mRef(1) {}
            void incRef() {
                ++mRef;
            }
            int decRef() {
                return --mRef;
            }
            int getRef() const {
                return mRef;
            }
            
        private:
            int32 mRef;
        };
        
        template<class C>
        struct ReleasePolicy {
            static void release(C* obj) {
                if(obj)
                    delete obj;
            }
        };
        
        template<class C>
        struct ReleaseArrayPolicy {
            static void release(C* obj) {
                if(obj)
                    delete []obj;
            }
        };
    }
	
    
	template<typename T, class RC=autoptr::RefCounter, class RP=autoptr::ReleasePolicy<T> >
	class SORA_API SoraAutoPtr {
	public:
		SoraAutoPtr(): ptr(NULL), counter(new RC) {}
		
        explicit SoraAutoPtr(T* t): ptr(t), counter(new RC) { 
        }
        
        template<class Other, class OtherRP>
		SoraAutoPtr(const SoraAutoPtr<Other, RC, OtherRP>& rhs): 
        counter(rhs.counter),
        ptr(const_cast<Other*>(rhs.get())) {
            counter->incRef();
        }
        
        SoraAutoPtr(const SoraAutoPtr& rhs):
        counter(rhs.counter),
        ptr(rhs.ptr) {
            counter->incRef();
        }
            
        ~SoraAutoPtr() { 
            release();
        }
		
		SoraAutoPtr& assign(T* _ptr) {
            if(get() != _ptr) {
                RC* tmp = new RC;
                release();
                counter = tmp;
                ptr = _ptr;
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
        SoraAutoPtr& assign(const SoraAutoPtr<Other, RC, OtherRP>& _ptr) {
            if(_ptr.get() != ptr) {
                SoraAutoPtr tmp(ptr);
                swap(tmp);
            }
            return *this;
        }
      
        void reset(T* ptr=NULL) {
            assign(ptr);
        }

		SoraAutoPtr& operator=(const SoraAutoPtr& rhs) { 
			return assign(rhs);
		}
		SoraAutoPtr& operator=(T* rhs) {
            return assign(rhs);
		}
        template<class Other, class OtherRP>
        SoraAutoPtr& operator=(const SoraAutoPtr<Other, RC, OtherRP>& _ptr) {
            return assign<Other>(_ptr);
        }
        
        void swap(SoraAutoPtr& rhs) {
            std::swap(ptr, rhs.ptr);
            std::swap(counter, rhs.counter);
        }
        
        template<class Other>
        SoraAutoPtr<Other, RC, RP> cast() const {
            Other* other = dynamic_cast<Other*>(ptr);
            if(other) {
                return SoraAutoPtr<Other, RC, RP>(counter, other);
            }
            return SoraAutoPtr<Other, RC, RP>();
        }
        
        template<class Other>
		SoraAutoPtr<Other, RC, RP> unsafeCast() const {
            Other* other = static_cast<Other*>(ptr);
            return SoraAutoPtr<Other, RC, RP>(counter, other);
        }
        
        T* get() {
            return ptr;
        }
        T* get() const {
            return ptr;
        }
        
		T* operator->() {
			return deref();
		}
        const T* operator->() const {
            return deref();
        }
    
		T& operator*() {
			return *deref();
		}
        const T& operator*() const {
			return *deref();
		}
        
        operator T*() {
            return ptr;
        }
        operator const T*() const {
            return ptr;
        }
        
        operator bool() {
            return ptr != 0;
        }
        
        bool operator !()const {
            return ptr == 0;
        }
        bool operator ==(const SoraAutoPtr& rhs) const {
            return get() != rhs.get();
        }
        bool operator -=(const T* rhs) const {
            return get() != rhs;
        }
        bool operator ==(T* rhs) const {
            return get() != rhs;
        }
        bool operator !=(const SoraAutoPtr& rhs) const {
            return get() != rhs.get();
        }
        bool operator !=(const T* rhs) const {
            return get() != rhs;
        }
        bool operator !=(T* rhs) const {
            return get() != rhs;
        }
        bool operator>(const SoraAutoPtr& rhs) const {
            return get() > rhs.get();
        }
        bool operator>(T* rhs) const {
            return get() > rhs;
        }
        bool operator<(const T* rhs) const {
            return get() < rhs;
        }
        bool operator>(const T* rhs) const {
            return get() > rhs;
        }
        bool operator<(T* rhs) const {
            return get() < rhs;
        }
        bool operator<=(const SoraAutoPtr& rhs) const {
            return get() < rhs.get();
        }
        bool operator>=(const SoraAutoPtr& rhs) const {
            return get() > rhs.get();
        }
        bool operator>=(T* rhs) const {
            return get() > rhs;
        }
        bool operator<=(const T* rhs) const {
            return get() < rhs;
        }
        bool operator>=(const T* rhs) const {
            return get() > rhs;
        }
        bool operator<=(T* rhs) const {
            return get() < rhs;
        }
		
		bool valid() const {
			return ptr!=0;
		}
        bool isValid() const {
            return ptr!=0;
        }
        bool isNull() const {
            return ptr==0;
        }

		
		int32 ref_count() const { 
			return counter->getRef();
		}
        
		bool operator == (const SoraAutoPtr<T>& rhs) {
			return ptr == rhs.ptr;
		}
		
	private:
		T* deref() const {
            if(!ptr)
                THROW_SORA_EXCEPTION(NullPointerException, "");
            return ptr;
        }
        
        void release() {
            sora_assert(counter);
            int i = counter->decRef();
            if(i == 0) {
                RP::release(ptr);
                ptr = 0;
                
                delete counter;
                counter = 0;
            }
        }
        
        SoraAutoPtr(RC* pCounter, T* pPtr):
        counter(pCounter), ptr(pPtr) {
            sora_assert(pCounter);
            pCounter->incRef();
        }
        
    private:
        RC* counter;
        T* ptr;
        
        template<class OC, class ORC, class OPR>
        friend class SoraAutoPtr;
	};
    
    template <class C, class RC, class RP>
    inline void swap(SoraAutoPtr<C, RC, RP>& p1, SoraAutoPtr<C, RC, RP>& p2) {
        p1.swap(p2);
    }
	
	template<typename T>
	class SoraSharedClass: public SoraAutoPtr<T> {
	public:
		typedef SoraAutoPtr<T> Pointer;
	};
		
} // namespace sora
#endif