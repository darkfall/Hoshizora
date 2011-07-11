/*
 *  SoraSingleton.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_SINGLETON_H_
#define SORA_SINGLETON_H_

#include "SoraPlatform.h"

namespace sora {
	
	template<class T>
	class SORA_API SoraSingleton {
	public:
		static T* Instance() {
			if(!pInstance) pInstance = new T();
			return pInstance;
		}
        
        static T& RefInstance() {
            if(!pInstance) pInstance = new T();
            return *pInstance;
        }
	
		static void Destroy() { 
            if(pInstance) 
                delete pInstance;
            pInstance = NULL;
        }
	
	private:
		static T* pInstance;
		SoraSingleton(const SoraSingleton& singleton){}
		SoraSingleton& operator=(const SoraSingleton& singleton){}
	
	protected:
		SoraSingleton() {}
		virtual ~SoraSingleton() {}
		
	};

	template<class T>
	T* SoraSingleton<T>::pInstance = NULL;
    
    template<class T>
    class SORA_API SoraThreadLocalSingleton {
    public:
        static T* Instance() {
            if(!pInstance) pInstance = new T();
            return pInstance;
        }
        
        static T& RefInstance() {
            if(!pInstance) pInstance = new T();
            return *pInstance;
        }
        
        static void Destroy() {
            if(pInstance)
                delete pInstance;
            pInstance = NULL;
        }
        
    private:
        static ThreadLocal T* pInstance;
        SoraThreadLocalSingleton(const SoraThreadLocalSingleton& singleton) {}
        SoraThreadLocalSingleton& operator=(const SoraThreadLocalSingleton& singleton){}
        
	protected:
		SoraThreadLocalSingleton() {}
		virtual ~SoraThreadLocalSingleton() {}
    };
    
    template<class T>
    ThreadLocal T* SoraThreadLocalSingleton<T>::pInstance = NULL;
	
} // namespace sora

#endif // SORA_SINGLETON_H