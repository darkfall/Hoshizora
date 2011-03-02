/*
 *  rfMap.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/13/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef _RF_MAP_H_
#define _RF_MAP_H_

#include <vector>
#include "rfCommon.h"

namespace rf {
	
	class rfLightSource {
	public:
		uint8 uBoxH;
		uint8 uBoxV;
		
		float fDirection;
		float fRotateAngel;
		
		uint8 uType; // reserved;
		
		rfLightSource(): uBoxH(0), uBoxV(0), fDirection(0.f), fRotateAngel(0.f), uType(0) {}
		rfLightSource(uint8 boxv, uint8 boxh, float dir, float ra, uint8 type=0): uBoxH(boxh), uBoxV(boxv), fDirection(dir), fRotateAngel(ra), uType(type) {}
	};
	
	class rfBox {
	public:
		uint8  uBoxEdgeNum;
		uint8  uIsLightPath;
		uint8  uHasMirror;
		uint8  uMirrorPlacable;
		uint8  uAvailable;
		uint8  uHasLight;
		uint8  uCustom; // custom val, reserved
		
		int BoxDegree;
		int BoxPosX;
		int BoxPosY;
		float MirrorDirection;
		
		uint16 uBoxSize; // reserved
		
		uint8  uLightPassed;
		uint8  uMirrorReflected;
		
		rfLightSource light;
		
		rfBox(): uBoxEdgeNum(0), uBoxSize(0), uIsLightPath(0), uCustom(0), uHasMirror(0), MirrorDirection(0), uAvailable(1), 
																BoxDegree(0), BoxPosX(0), BoxPosY(0), uHasLight(0), uLightPassed(0), uMirrorReflected(0), uMirrorPlacable(1) {}
		rfBox(uint8 edgeNum, uint16 size, uint8 lightPath=0): uBoxEdgeNum(edgeNum), uBoxSize(size), uIsLightPath(lightPath), uLightPassed(0), uMirrorReflected(0), 
																uCustom(0), uHasMirror(0), MirrorDirection(0), uAvailable(1), BoxDegree(0), BoxPosX(0), BoxPosY(0), uHasLight(0), uMirrorPlacable(1) {}
	};
	
	class rfMap {
	public:
		rfMap();
		
		void setMapWidth(uint32 w);
		void setMapHeight(uint32 h);
		void setMapSize(uint32 w, uint32 h);
		
		void setGlobalBoxSize(uint16 size);
		void setGlobalBoxEdgeNum(uint8 num);
		void setBox(uint16 v, uint16 h, const rfBox& box);

		void markBoxAsLightPath(uint16 v, uint16 h, uint8 isLightPath);
		
		void addLight(uint8 boxV, uint8 boxH, float direction, float rotateAngel, uint8 type=0);
		void addLight(const rfLightSource& sr);
		
		uint32 getMapWidth() const;
		uint32 getMapHeight() const;
		
		rfBox  getBox(uint16 v, uint16 h);
		uint16 getGlobalBoxSize() const;
		uint8  getGlobalBoxEdgeNum() const;
		uint32 getMapBoxCount() const;
		uint16 getMapBoxCountH() const;
		uint16 getMapBoxCountV() const;
		
		void build();
		
		uint8 saveMap(const char* pstrOut);
		uint8 loadMap(const char* pstrIn);
		uint8 loadMapMem(void* pData, uint32 size);
		
	private:
		rfMap(const rfMap&);
		rfMap& operator=(const rfMap&);
		
		uint16 uGlobalBoxSize;
		uint8  uGlobalBoxEdgeNum;
		uint32 uMapWidth, uMapHeight;
		uint16 uMapBoxCountV, uMapBoxCountH;
		
		bool bBuilded;
		
		typedef std::vector<rfBox> RF_MAP_CONT_H;
		typedef std::vector<RF_MAP_CONT_H> RF_MAP_CONT;
		RF_MAP_CONT mapBoxes;
	};
	
} // namespace rf


#endif // _RF_MAP_H_

