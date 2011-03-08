/*
 *  SoraAutoPtr.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_AUTO_PTR_H_
#define SORA_AUTO_PTR_H_

#include <typeinfo>
#include <map>
using std::map;

#include "SoraPlatform.h"
#include "SoraException.h"

/*
 a simple reference counted auto pointer implemention
 */

namespace sora {
	
#define sora_safe_delete(ptr) { if(ptr) delete ptr; ptr=0; }

	template<typename T>
	class SoraAutoPtr {
	public:
		SoraAutoPtr(): ptr(NULL), refCount(NULL) {}
		SoraAutoPtr(T* t): ptr(t), refCount(NULL) { reference_count_init(); }
		SoraAutoPtr(const SoraAutoPtr<T>& rhs): ptr(NULL), refCount(NULL) { *this = rhs; }
		~SoraAutoPtr() { reference_count_dec(); }
		
		void alloc() {
			if(ptr) reference_count_dec();
			ptr = new T; 
			reference_count_init();
		}
		void release() {
			if(ptr) reference_count_dec();
		}
		
		void operator=(const SoraAutoPtr<T>& rhs) { 
			if(ptr == rhs.ptr) {
				assert(refCount == rhs.refCount);
				return;
			}
			if(ptr != NULL) {
				reference_count_dec();
			}
			ptr = rhs.ptr;
			refCount = rhs.refCount;
			++(*refCount);
		}
		void operator=(T* rptr) {
			if(ptr != NULL) {
				if(typeid(rptr) != typeid(ptr)) {
					throw SORA_EXCEPTION("SoraAutoPrt::operator =: different pointer type assignment");
				}
			
				reference_count_dec();
				ptr = rptr;
				reference_count_init();
			} else {
				ptr = rptr;
				reference_count_init();
			}
		}
		
		T* operator->() {
			if(ptr == 0) {
				throw SORA_EXCEPTION("SoraAutoPtr::operator ->: pointer not allocted");
			}
			return ptr;
		}
		T* operator*() {
			if(ptr == 0) {
				throw SORA_EXCEPTION("SoraAutoPtr::operator *: pointer not allocted");
			}
			return ptr;
		}
		
		bool valid() {
			return ptr!=0;
		}

		T* pointer() const { return ptr; }
		int32 ref_count() const { 
			if(refCount)
				return *refCount;
			return 0;
		}
		
		bool operator == (const SoraAutoPtr<T>& rhs) {
			return ptr == rhs.ptr;
		}
		
	private:
		T* ptr;
		int32* refCount;
		
	protected:
		inline void reference_count_init() {
			if(refCount != NULL)
				sora_safe_delete(refCount);

			refCount = new int;
			*refCount = 1;
			
#ifdef _DEBUG
			printf("holding obj: %lu, refCount:%d\n", (ulong32)ptr, *refCount);
#endif
		}
		
		inline void reference_count_dec() {
			if(refCount == NULL) return;
			--(*refCount);
			if(*refCount == 0) {
#ifdef _DEBUG
				printf("deallocing obj: %lu, refCount:%d\n", (ulong32)ptr, *refCount);
#endif
				sora_safe_delete(ptr);
				sora_safe_delete(refCount);
			}
		}
				
		void* operator new(size_t);
		//void operator delete(void*);
	};
	
} // namespace sora
#endif