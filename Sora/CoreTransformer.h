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
	class CoreTransformer {
	public:
		virtual T slerp(T& t1, T& t2, float32 pos) = 0;
		
		virtual CoreTransformer<T>* copy() = 0;
	};
	
	template<typename T>
	class CoreLinearTransformer: public CoreTransformer<T> {
	public:
		T slerp(T& t1, T& t2, float32 pos) {
			if(pos > 1.f) pos = 1.f;
			else if(pos < 0.f) pos = 0.f;
				
			return (t1+(t2-t1)*pos);
		}
		
		CoreLinearTransformer<T>* copy() {
			return new CoreLinearTransformer<T>;
		}
	};
	
	/*
	 0 - 2_PI sinf
	 */
	template<typename T>
	class CoreTrigTransformer: public CoreTransformer<T> {
	public:
		T slerp(T& t1, T& t2, float32 pos) {
			if(pos > 1.f) pos = 1.f;
			else if(pos < 0.f) pos = 0.f;
			
			//printf("%f, %f\n", pos, (t1+(t2-t1)*sinf(D_PI_2*pos)));

			return (t1+(t2-t1)*sinf(F_PI_2*pos));
		}
		
		CoreTrigTransformer<T>* copy() {
			return new CoreTrigTransformer<T>;
		}
	};
	
	template<typename T>
	T LinearSlerp(T& t1, T& t2, float32 pos) {
		return (t1+(t2-t1)*pos);
	}
	
	template<typename T>
	T TrigSlerp(T& t1, T& t2, float32 pos) {
		return (t1+(t2-t1)*sinf(D_PI_2*pos));
	}
	
		
} // namespace sora


#endif
