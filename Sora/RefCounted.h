/*
 *  RefCounted.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/16/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_REF_COUNTED_H_
#define SORA_REF_COUNTED_H_

namespace sora {
	
	class RefCounted {
	public:
		RefCounted(): refCount(NULL) {}
		virtual ~RefCounted() {
			ref_dec();
			if(ref_count() == 0)
				release();
		}
		
		virtual void release() {
		};
		
		void ref_reassign(const RefCounted& ref) {
			ref_dec();
			refCount = ref.refCount;
		}
		
		void ref_init() {
			if(refCount)
				delete refCount;
			refCount = new size_t;
			*refCount = 1;
		}
		
		void ref_dec() {
			--*refCount;
			if(*refCount == 0) {
				delete refCount;
				refCount = NULL;
				
				release();
			}
		}
		
		void ref_inc() {
			++*refCount;
		}
		
		size_t ref_count() {
			if(refCount)
				return *refCount;
			return 0;
		}
		
	private:
		size_t* refCount;
	};
	
} // namespace sora


#endif // SORA_REF_COUNTED_H_