/*
 *  rfMapGrid.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdMapGrid.h"

namespace rftd {

	rftdMapGrid::rftdMapGrid(int32 width, int32 height) {
		mGridSize = sora::GET_ENV_INT("rftd_map_gridsize", 8);
		
		mDebugMode = false;
	}
	
	rftdMapGrid::~rftdMapGrid() {
	}
	
	void rftdMapGrid::render() {
		BUILD_BOX_CONT::iterator itBox = buildBoxes.begin();
		while(itBox != buildBoxes.end()) {
			++itBox;
		}
	}
	
	void rftdMapGrid::enableDebug(bool flag) {
		mDebugMode = flag;
	}
	
	void rftdMapGrid::insertBox(const hgeRect& rect) {
		buildBoxes.push_back(rect);
	}
	
	bool rftdMapGrid::canBuild(const hgeRect& box) {
		BUILD_BOX_CONT::iterator itBox = buildBoxes.begin();
		while(itBox != buildBoxes.end()) {
			if(itBox->TestPoint(box.x1, box.y1) ||
			   itBox->TestPoint(box.x1, box.y1+box.y2) ||
			   itBox->TestPoint(box.x1+box.x2, box.y1+box.y2) ||
			   itBox->TestPoint(box.x1+box.x2, box.y1))
				return true;
			++itBox;
		}
		return false;
	}
	

} // namespace rftd