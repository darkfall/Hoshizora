/*
 *  TemplateTransformer.h
 *  Sora
 *
 *  Created by GriffinBu on 2/1/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef TEMPLATE_TRANSFORMER_H_
#define TEMPLATE_TRANSFORMER_H_

#include "SoraPlatform.h"
#include <math.h>

#include "CoreTransform.h"
#include "CoreTransformer.h"

namespace sora {
	
	template<typename type, typename transformer=CoreLinearTransformer<type> >
	class TemplateTransformer {
	public:
		inline type slerp(type start, type end, float32 t) {
			return _transformer.slerp(start, end, t);
		}
		
		transformer _transformer;
	};
	
} // namespace sora


#endif // TEMPLATE_TRANSFORMER_H_