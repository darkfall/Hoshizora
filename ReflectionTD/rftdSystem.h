/*
 *  rftdSystem.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_SYSTEM_H_
#define RFTD_SYSTEM_H_

#include "rftdCommon.h"
#include "SoraSingleton.h"

namespace rftd {
	
	class rftdSystem: public sora::SoraSingleton<rftdSystem> {
		friend class sora::SoraSingleton<rftdSystem>;
	};
	
} // namespace rftd

#endif // RFTD_SYSTEM_H_
