/*
 *  Transformer.h
 *  Sora
 *
 *  Created by GriffinBu on 12/20/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef CORE_TRANSFORMER_H_
#define CORE_TRANSFORMER_H_

#include "SoraPlatform.h"
#include "SoraMath.h"

namespace sora {
	
	template<typename T>
	class SoraCoreTransformer {
	public:        
        virtual SoraCoreTransformer<T>* clone() = 0;
        
		virtual T slerp(T& t1, T& t2, float pos) = 0;
    };
	
	template<typename T>
	class SoraCoreLinearTransformer: public SoraCoreTransformer<T> {
	public:
		T slerp(T& t1, T& t2, float pos) {
			if(pos > 1.f) pos = 1.f;
			else if(pos < 0.f) pos = 0.f;
				
			return (t1+(t2-t1)*pos);
		}
        
        SoraCoreTransformer<T>* clone() {
            return new SoraCoreLinearTransformer<T>(*this);
        }
	};
	
	/*
	 0 - PI_2 sinf
	 */
	template<typename T>
	class SoraCoreTrigTransformer: public SoraCoreTransformer<T> {
	public:
		T slerp(T& t1, T& t2, float pos) {
			if(pos > 1.f) pos = 1.f;
			else if(pos < 0.f) pos = 0.f;
		
			return (t1+(t2-t1)*sinf(F_PI_2*pos));
		}
        
        SoraCoreTransformer<T>* clone() {
            return new SoraCoreTrigTransformer<T>(*this);
        }
	};
	
	/*
	 0 - PI*2 sinf, fullround
	 */
	template<typename T>
	class SoraCoreTrigTransformerFull: public SoraCoreTransformer<T> {
	public:
		T slerp(T& t1, T& t2, float pos) {
			if(pos > 1.f) pos = 1.f;
			else if(pos < 0.f) pos = 0.f;
					
			return (t1+(t2-t1)*sinf(F_PI*2*pos));
		}
        
        SoraCoreTransformer<T>* clone() {
            return new SoraCoreTrigTransformerFull<T>(*this);
        }
	};
	
	template<typename T>
	T LinearSlerp(T& t1, T& t2, float pos) {
		return (t1+(t2-t1)*pos);
	}
	
	template<typename T>
	T TrigSlerp(T& t1, T& t2, float pos) {
		return (t1+(t2-t1)*sinf(F_PI_2*pos));
	}
	
		
} // namespace sora


#endif
