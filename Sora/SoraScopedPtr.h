//
//  SoraScopedPtr.h
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraScopedPtr_h
#define Sora_SoraScopedPtr_h

#include "SoraPlatform.h"
#include "uncopyable.h"

namespace sora {
    
    /**
     * Simple boost::scoped_ptr impl
     **/
    
    template<class T>
    class SoraScopedPtr: uncopyable {
    public:
        explicit SoraScopedPtr(T* _ptr):
        ptr(_ptr) {
        }
        SoraScopedPtr():
        ptr(NULL) {
            
        }
        ~SoraScopedPtr() {
            if(ptr) {
                delete ptr;
                ptr = NULL;
            }
        }
        
        void reset(T* p = 0) {
            ptr = p;
        }
        
        T& operator*() const {
            return *ptr;
        }
        T* operator->() const {
            return ptr;
        }
        
        T* get() const {
            return ptr;
        }
        
        void swap(SoraScopedPtr& b);
        
    private:
        T* ptr;
    };
    
    template<class T>
    void swap(SoraScopedPtr<T>& a, SoraScopedPtr<T>& b) {
        std::swap(a.ptr, b.ptr);
    }
    
} // namespace sora

#endif
