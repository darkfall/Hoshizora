/*
 *  SoraRTTIMethod.h
 *  PartitionTree
 *
 *  Created by Griffin Bu on 5/22/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_RTTI_METHOD_H_
#define SORA_RTTI_METHOD_H_

#include "SoraRTTIType.h"
#include "SoraRTTIVar.h"

namespace sora {
	
	class SoraRTTIClassDescriptor;
	
	class SoraRTTIMethodType: public SoraRTTIType {
	public:
		SoraRTTIMethodType();
		virtual ~SoraRTTIMethodType();
		
		virtual std::string getName();
		
		unsigned int getParamNum() const;
		
		SoraRTTIType* getParamType(unsigned int iIndex) const;
		SoraRTTIType** getParamTypes() const;
		
		SoraRTTIType* getReturnType() const;
				
		virtual SoraRTTIType* invoke(void* obj, void* params[]) = 0;
		virtual SoraRTTIType* invoke(void* obj, const std::string& params) = 0;
		
		SoraRTTIClassDescriptor* getClass() const;
		
	protected:
		bool mStatic;
		unsigned int nParams;
		
		SoraRTTIType** mParamTypes;
		SoraRTTIType*  mReturnType;
		
		SoraRTTIClassDescriptor* mClass;
	};
	
	class SoraRTTIMethodDescriptor {
	public:
		SoraRTTIMethodDescriptor(const std::string& name, 
								 int flags, 
								 SoraRTTIMethodType* type, 
								 SoraRTTIClassDescriptor* RTTIClass);
		virtual ~SoraRTTIMethodDescriptor();
		
		SoraRTTIMethodDescriptor& operator, (SoraRTTIMethodDescriptor& rhs);
		
		std::string getDecl() const;
		std::string getName() const;
		
		SoraRTTIType* invoke(void* obj, void* params[]);
		SoraRTTIType* invoke(void* obj, const std::string& params);
		
		SoraRTTIClassDescriptor* getClass() const;
		
	protected:
		int mFlags;
		int mIndex;
		std::string mName;

		SoraRTTIMethodType* mType;
		SoraRTTIClassDescriptor* mClass;
	};
	
	template<typename __RT>
	static __RT ReturnValToVar(SoraRTTIType* type) {
		SoraRTTIVar<__RT>* var = dynamic_cast<SoraRTTIVar<__RT>*> (type);
		if(var == NULL) {
			SoraRTTIType* type = RTTITypeOfPtr((__RT*)0);
			throw SoraRTTIException("SoraRTTIFieldGet: Invalid RTTI Var Type, should be "+type->getName()+", but token "+var->getName());
		}
		__RT val = var->get();
		delete var;
		return val;
	}
	
	template<typename __RT>
	static SoraRTTIVar<__RT>* ReturnValToRTTIVar(SoraRTTIType* type) {
		SoraRTTIVar<__RT>* var = dynamic_cast<SoraRTTIVar<__RT>*> (type);
		if(var == NULL) {
			SoraRTTIType* type = RTTITypeOfPtr((__RT*)0);
			throw SoraRTTIException("SoraRTTIFieldGet: Invalid RTTI Var Type, should be "+type->getName()+", but token "+var->getName());
		}
		return var;
	}
	
}

#endif // SORA_RTTI_METHOD_H_