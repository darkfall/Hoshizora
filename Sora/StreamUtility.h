/*
 *  StreamUtility.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_STREAM_UTILITY_H_
#define SORA_STREAM_UTILITY_H_

#include <string>
#include <sstream>
#include <iomanip>

namespace sora {
	
	template<typename T>
	inline std::string anyToString(const T& t, int precision=3) {
		std::ostringstream str;
		str << std::fixed << std::setprecision(precision) << t;
		return str;
	}
	
	
} // namespace sora

#endif