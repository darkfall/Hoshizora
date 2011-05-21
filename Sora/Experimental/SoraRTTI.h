/*
 *  SoraRTTI.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_RTTI_H_
#define SORA_RTTI_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"

namespace sora {
	
	typedef void* (*SoraRTTIConstructorFunc)();
	
	class SoraRTTIRepository: public SoraSingleton<SoraRTTIRepository> {
		friend class SoraSingleton<SoraRTTIRepository>;
		
	public:
		void registerClass(SoraRTTIConstructorFunc func, const SoraString& name) {
			mClassContructors[name] = func;
		}
		
		void* constructClass(const SoraString& name) {
			CONSTRUCTOR_MAP::iterator itConstructor = mClassContructors.find(name);
			if(itConstructor != mClassContructors.end()) {
				return itConstructor->second();
			}
		}
		
	private:
		typedef hash_map<SoraString, SoraRTTIConstructorFunc> CONSTRUCTOR_MAP;
		CONSTRUCTOR_MAP mClassContructors;		
	};
	
	class SoraRTTIClassDescriptor {
	public:
		SoraRTTIClassDescriptor(const char* className, uint32 classSize, const char* name, SoraRTTIConstructorFunc func) {
			SoraRTTIRepository::Instance()->registerClass(func, name);
			
			name = className;
			size = classSize;
		}
		
	private:
		uint32 size;
		
		SoraString name;
	};
	
	
	#define REGISTER_CLASS(T, name) \
		static T* SoraRTTIConstructor_##T() { \
			return new T(); \
		} \
		sora::SoraRTTIClassDescriptor SoraRTTIDescriptor_##T(\
			#T, \
			sizeof(T), \
			name, \
			(sora::SoraRTTIConstructorFunc)SoraRTTIConstructor_##T);
	
	#define CONSTRUCT_CLASS(name) \
		sora::SoraRTTIRepository::Instance()->constructClass(name); 
	
	
	
} // namespace sora


#endif // SORA_RTTI_H_