/*
 *  rfMapGrid.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_MAP_GRID_H_
#define RFTD_MAP_GRID_H_

#include "rftdCommon.h"

namespace rftd {
	
	// the grid map that contains the information for where towers can be built
	// the minimum division size, default 8 means minimun build area in the map is 8*8 pixels
	// then the tower size can be (8n) * (8m), n, m >= 1
	// GET_ENV_INT("rftd_map_grid_size")
	class rftdMapGrid {
	public:
		// the width and height of current map
		rftdMapGrid(int32 width, int32 height);
		~rftdMapGrid();
		
		// render grids if debug on
		void render();
		
		void enableDebug(bool flag);
			
		void insertBox(const hgeRect& box);
		
		// test a position can build or not
		bool canBuild(const hgeRect& buildBox);
		
	private:		
		bool mDebugMode;
		
		typedef std::vector<hgeRect> BUILD_BOX_CONT;
		BUILD_BOX_CONT buildBoxes;
		
		int32 mGridSize;
	};
	
} // namespace rftd


#endif // RFTD_MAP_GRID_H_