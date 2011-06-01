/*
 *  rfCommon.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

/*
 Base definitions for reflection
 */
#ifndef RF_COMMON_REV_H_
#define RF_COMMON_REV_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraMath.h"
#include "json/json.h"
#include "SoraGUIChan/jsongui.h"
#include "hgevector.h"

namespace reflection {

	typedef hgeVector rfPoint;
	typedef sora::SoraObject rfObject;
	typedef float32 rfFloat;
	typedef double rfDouble;
	typedef int32 rfInt;
	typedef ulong32 rfULong;
	typedef long32 rfLong;
	typedef uint32 rfUInt;
	
#define rfDgrToRad(dgr) sora::DGR_RAD(dgr)
#define rfRadToDgr(rad) sora::RAD_DGR(rad)
	
	static rfFloat rfGetThirdEdgeWithEdgeAndAngle(rfFloat edge1, rfFloat edge2, rfFloat angle) {
		rfFloat stedge = edge1*edge1 + edge2*edge2 - 2*edge1*edge2*cosf(angle);
		return sqrt(stedge);
	}

} // namespace reflection

#endif