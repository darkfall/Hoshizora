/*
 *  SoraASSM_Variable.h
 *  Sora
 *
 *  Created by Griffin Bu on 2/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_ASSM_VARIABLE_H_
#define _SORA_ASSM_VARIABLE_H_

#include "SoraPlatform.h"
#include "SoraNamedObject.h"
#include "vector3.h"

namespace sora {
	
	/*
	 Base class for state machine variables
     bool
     int
     float
     ulong32
     vector3
	 */
    template<typename VAR_TYPE>
	class SoraASSM_Variable: public SoraNamedObject {
	public:
		SoraASSM_Variable(): bValueAssigned(false), defaultValue(0), value(0) {}
		virtual ~SoraASSM_Variable() {}
		
		void reset() { defaultValue = 0; value = 0; }
		
		VAR_TYPE& Value() { 
			if(!bValueAssigned)
				return defaultValue;
			return value;
		}
		
		SoraASSM_Variable operator =(VAR_TYPE val) { defaultValue = val; }
	
		SoraASSM_Variable& operator +(VAR_TYPE val) { defaultValue += val; }
		SoraASSM_Variable& operator -(VAR_TYPE val) { defaultValue -= val; }
		SoraASSM_Variable* operator *(VAR_TYPE val) { defaultValue *= val; }
		SoraASSM_Variable& operator /(VAR_TYPE val) { defaultValue /= val; }
		
		bool operator==(const SoraASSM_Variable& rhs) { 
			if(!bValueAssigned)
				return defaultValue == rhs.defaultValue;
			return value == rhs.value;
		}
		
	private:
		VAR_TYPE value;
		VAR_TYPE defaultValue;
		bool bValueAssigned;
	};
    
    typedef SoraASSM_Variable<float> SMFloatVariable;
    typedef SoraASSM_Variable<int>   SMIntegerVariable;
    typedef SoraASSM_Variable<bool>  SMBoolVariable;
    typedef SoraASSM_Variable<ulong32> SMUlongVariable;
    
    class SoraVector3: public vector3 {
    public:
        SoraVector3& operator = (int i) {
            x = i; y = i;
            return *this;
        }
    };

    typedef SoraASSM_Variable<SoraVector3> SMVector2Variable;
    
} // namespace sora 


#endif // _SORA_ASSM_VARIABLE_H_