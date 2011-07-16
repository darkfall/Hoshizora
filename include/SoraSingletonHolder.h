//
//  SoraSingletonHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 7/11/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSingletonHolder_h
#define Sora_SoraSingletonHolder_h

namespace sora {
    
    /**
     * This is a helper template class for managing singleton objects allocated on the heap
     * static SoraSingletonHolder<MyClass> MyClass etc
     **/
    
    template<class T>
    class SORA_API SoraSingletonHolder {
    public:
        SoraSingletonHolder():
        mInstance(0) {
            
        }
        
        ~SoraSingletonHolder() {
            delete mInstance;
        }
        
        T* get() {
            if(!mInstance) mInstance = new T;
            return mInstance;
        }
        
    private:
        T* mInstance;
    };
    
} // namespace sora


#endif
