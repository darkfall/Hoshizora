/*
 *  rfMap.cpp
 *  rfMap
 *
 *  Created by GriffinBu on 12/13/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#include "rfMap.h"
#include "lMemFile.h"

namespace rf {
	
#define RFMAP_HEADER "RFMP"

	rfMap::rfMap(): uMapWidth(0), uMapHeight(0), uGlobalBoxSize(0), uGlobalBoxEdgeNum(0), uMapBoxCountV(0), uMapBoxCountH(0), bBuilded(false) {
	}
	
	void rfMap::setMapWidth(uint32 w) {
		uMapWidth = w;
		bBuilded = false;
	}
	
	void rfMap::setMapHeight(uint32 h) {
		uMapHeight = h;
		bBuilded = false;
	}

	void rfMap::setMapSize(uint32 w, uint32 h) {
		uMapWidth = w;
		uMapHeight = h;
		bBuilded = false;
	}
	
	void rfMap::setGlobalBoxSize(uint16 size) {
		uGlobalBoxSize = size;
		bBuilded = false;
	}
	
	void rfMap::setGlobalBoxEdgeNum(uint8 num) {
		uGlobalBoxEdgeNum = num;
		bBuilded = false;
	}

	void rfMap::setBox(uint16 v, uint16 h, const rfBox& box) {
		if(!bBuilded)
			build();
		
		if(h < uMapBoxCountH && v < uMapBoxCountV) {
			mapBoxes[v][h] = box;
		}
	}
	
	void rfMap::markBoxAsLightPath(uint16 v, uint16 h, uint8 isLightPath) {
		if(!bBuilded)
			build();
		
		if(h < uMapBoxCountH && v < uMapBoxCountV) {
			mapBoxes[v][h].uIsLightPath = isLightPath;
		}
	}
	
	void rfMap::addLight(uint8 boxV, uint8 boxH, float direction, float rotateAngel, uint8 type) {
		if(!bBuilded)
			build();
		
		if(boxV < uMapBoxCountV && boxH < uMapBoxCountH) {
			mapBoxes[boxV][boxH].light = rfLightSource(boxV, boxH, direction, rotateAngel, type);
			mapBoxes[boxV][boxH].uHasLight = 1;
		}
	}
	
	void rfMap::addLight(const rfLightSource& sr) {
		if(!bBuilded)
			build();
		
		if(sr.uBoxV > uMapBoxCountV && sr.uBoxV < uMapBoxCountH) {
			mapBoxes[sr.uBoxV][sr.uBoxH].light = sr;
			mapBoxes[sr.uBoxV][sr.uBoxH].uHasLight = 1;
		}
	}
	
	void rfMap::build() {
		mapBoxes.clear();
		
		if(uGlobalBoxSize == 0 || uGlobalBoxEdgeNum == 0) {
			uMapBoxCountH = uMapBoxCountV = 0;
			return;
		}
		
		if(uMapWidth % uGlobalBoxSize != 0)
			uMapWidth -= uMapWidth%uGlobalBoxSize;
		if(uMapHeight % uGlobalBoxSize != 0)
			uMapHeight -= uMapHeight%uGlobalBoxSize;
		
		uMapBoxCountV = uMapHeight / uGlobalBoxSize;
		uMapBoxCountH = uMapWidth / uGlobalBoxSize;
		
		uint32 cx = 0;
		uint32 cy = 0;
		for(uint16 i=0; i<uMapBoxCountV; ++i) {
			RF_MAP_CONT_H boxLine;
			for(uint16 j=0; j<uMapBoxCountH; ++j) {
				rfBox box(uGlobalBoxEdgeNum, uGlobalBoxSize);
				box.BoxPosX = cx;
				box.BoxPosY = cy;
				box.MirrorDirection = 180.f / box.uBoxEdgeNum;

				boxLine.push_back(box);
				
				cx += getGlobalBoxSize();
			}
			cy += getGlobalBoxSize();
			cx = 0;
			mapBoxes.push_back(boxLine);
		}
		
		bBuilded = true;
	}
	
	uint32 rfMap::getMapWidth() const {
		return uMapWidth;
	}
	
	uint32 rfMap::getMapHeight() const {
		return uMapHeight;
	}
	
	uint16 rfMap::getGlobalBoxSize() const {
		return uGlobalBoxSize;
	}
	
	uint8 rfMap::getGlobalBoxEdgeNum() const {
		return uGlobalBoxEdgeNum;
	}
	
	uint32 rfMap::getMapBoxCount() const {
		if(uGlobalBoxSize == 0 || uGlobalBoxEdgeNum == 0)
			return 0;
		
		if(!bBuilded) {
			uint32 w = uMapWidth;
			uint32 h = uMapHeight;
			uint16 hc, vc;
			if(w % uGlobalBoxSize != 0)
				w -= w%uGlobalBoxSize;
			if(h % uGlobalBoxSize != 0)
				h -= h%uGlobalBoxSize;
			
			hc = w / uGlobalBoxSize;
			vc = h / uGlobalBoxSize;
			
			return hc*vc;
		}
		return uMapBoxCountH*uMapBoxCountV;
	}
	
	uint16 rfMap::getMapBoxCountH() const {
		if(uGlobalBoxSize == 0 || uGlobalBoxEdgeNum == 0)
			return 0;
		
		if(!bBuilded) {
			uint32 w = uMapWidth;
			if(w % uGlobalBoxSize != 0)
				w -= w%uGlobalBoxSize;
			return w / uGlobalBoxSize;
		} 
		return uMapBoxCountH;
	}
	
	uint16 rfMap::getMapBoxCountV() const {
		if(uGlobalBoxSize == 0 || uGlobalBoxEdgeNum == 0)
			return 0;
		
		if(!bBuilded) {
			uint32 h = uMapHeight;
			if(h % uGlobalBoxSize != 0)
				h -= h%uGlobalBoxSize;
			return h / uGlobalBoxSize;
		} 
		return uMapBoxCountV;
	}
	
	rfBox rfMap::getBox(uint16 v, uint16 h) {
		if(!bBuilded)
			build();
		
		if(h < uMapBoxCountH && v < uMapBoxCountV) {
			return mapBoxes[v][h];
		}
		return rfBox();
	}
	
	uint8 rfMap::saveMap(const char* pstrOut) {
		if(!bBuilded)
			build();
		
		FILE* fp = fopen(pstrOut, "wb");
		if(!fp)
			return 0;
		
		fwrite(RFMAP_HEADER, 4, 1, fp);
		
		bool err = false;
		err = fwriterf(fp, uMapWidth);		// 4
		err = fwriterf(fp, uMapHeight);		// 4
		err = fwriterf(fp, uGlobalBoxSize);	// 2
		err = fwriterf(fp, uGlobalBoxEdgeNum);	// 1
		err = fwriterf(fp, uMapBoxCountH);	// 2
		err = fwriterf(fp, uMapBoxCountV);	// 2
				
		if(!err) {
			for(uint16 i=0; i<uMapBoxCountV; ++i) {
				for(uint16 j=0; j<uMapBoxCountH; ++j) {
					err = fwriterf(fp, mapBoxes[i][j].uBoxEdgeNum);	 // 1
					err = fwriterf(fp, mapBoxes[i][j].uBoxSize);	 // 2
					err = fwriterf(fp, mapBoxes[i][j].uHasMirror);	 // 1
					err = fwriterf(fp, mapBoxes[i][j].uIsLightPath); // 1
					err = fwriterf(fp, mapBoxes[i][j].uAvailable);	 // 1
					err = fwriterf(fp, mapBoxes[i][j].uCustom);		 // 1
					
					err = fwriterf(fp, mapBoxes[i][j].BoxPosX);	// 4
					err = fwriterf(fp, mapBoxes[i][j].BoxPosY);	// 4
					err = fwriterf(fp, mapBoxes[i][j].BoxDegree);  // 2
					err = fwriterf(fp, mapBoxes[i][j].MirrorDirection); // 4
					
					err = fwriterf(fp, mapBoxes[i][j].uHasLight);  // 1
					if(mapBoxes[i][j].uHasLight == 1) {
						err = fwriterf(fp, mapBoxes[i][j].light.fDirection);
						err = fwriterf(fp, mapBoxes[i][j].light.fRotateAngel);
						err = fwriterf(fp, mapBoxes[i][j].light.uType);
					}
					
					if(err) 
						break;
				}
			}
		}
		
		fclose(fp);
		if(err)
			return 0;
		
		return 1;
	}
	
	uint8 rfMap::loadMap(const char* pstrIn) {
		FILE* fp = fopen(pstrIn, "rb");
		if(!fp)
			return 0;
		
		uint32 size = fgetsize(fp);
		void* pData = malloc(size);
		if(!pData) {
			fclose(fp);
			return 0;
		}
		
		fclose(fp);
		int r = loadMapMem(pData, size);
		free(pData);
		return r;
	}
	
	uint8 rfMap::loadMapMem(void* pData, uint32 size) {
		lMemFile* pmfile = new lMemFile(pData, size);
		
		char header[5];
		pmfile->read(header, 4);
		header[4] = '\0';
		if(strcmp(header, RFMAP_HEADER) != 0) {
			delete pmfile;
			return 0;
		}
		
		bool err = false;
		err = !pmfile->read(&uMapWidth);
		err = !pmfile->read(&uMapHeight);
		err = !pmfile->read(&uGlobalBoxSize);
		err = !pmfile->read(&uGlobalBoxEdgeNum);
		err = !pmfile->read(&uMapBoxCountH);
		err = !pmfile->read(&uMapBoxCountV);
		
		if(!err) {
			for(uint16 i=0; i<uMapBoxCountV; ++i) {
				RF_MAP_CONT_H boxline;
				for(uint16 j=0; j<uMapBoxCountH; ++j) {
					rfBox box;
					err = !pmfile->read(&box.uBoxEdgeNum);
					err = !pmfile->read(&box.uBoxSize);
					err = !pmfile->read(&box.uHasMirror);
					err = !pmfile->read(&box.uIsLightPath);
					err = !pmfile->read(&box.uAvailable);
					err = !pmfile->read(&box.uCustom);
					
					err = !pmfile->read(&box.BoxPosX);
					err = !pmfile->read(&box.BoxPosY);
					err = !pmfile->read(&box.BoxDegree);
					err = !pmfile->read(&box.MirrorDirection);
					
					err = !pmfile->read(&box.uHasLight);
					if(box.uHasLight == 1) {
						err = !pmfile->read(&box.light.fDirection);
						err = !pmfile->read(&box.light.fRotateAngel);
						err = !pmfile->read(&box.light.uType);
					}
									
					boxline.push_back(box);
				}
				mapBoxes.push_back(boxline);
				if(err)
					break;
			}
		}
		delete pmfile;
		
		if(err)
			return 0;
		
		bBuilded = true;
		return 1;
	}
	
} // namespace rf