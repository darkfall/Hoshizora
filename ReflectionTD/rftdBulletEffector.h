/*
 *  rftdBulletEffector.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_BULLET_EFFECTOR_H_
#define RFTD_BULLET_EFFECTOR_H_

#include "SoraPlatform.h"

namespace rftd {
	
	typedef struct {
		std::string name;
		
		std::vector<float32> param_list;
	} BulletEffectorConf;
	
	
} // namespace rftd

#endif // RFTD_BULLET_EFFECTOR_H_