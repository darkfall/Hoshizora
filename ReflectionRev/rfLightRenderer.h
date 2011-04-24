/*
 *  rfLightRenderer.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_LIGHT_RENDERER_H_
#define RF_LIGHT_RENDERER_H_

#include "rfCommon.h"
#include "rfMirror.h"
#include "rfShape.h"
#include "rfLightSource.h"

namespace reflection {
	
	/*
	 class that contains all lights and blocks in the map
	 dynamically create and destory
	 */
	class rfMapLightRenderer {
		friend class rfMap;
		
	public:
		rfMapLightRenderer();
		~rfMapLightRenderer() {}
		
		void addLightSource(rfLightSource* llight);
		void addMirror(rfMirror* mirror);
		void addToLightBox(rfShapeBase* shape);
		
		void removeLightSource(rfLightSource* light);
		void removeMirror(rfMirror* mirro);
		void removeToLightBox(rfShapeBase* shape);
		
		void addReflectionLight(rfLight* rlight);		
		
		void reset();
		void recalculate();
		
		void logic();
		void draw(gcn::Graphics* graphics);
		
		rfUInt getLightedBoxNumber() const;
		
		rfUInt getLightNumber() const;
		rfUInt getLightSourceNumber() const;
		rfUInt getShapeBoxNumber() const;
		rfUInt getMirrorNumber() const;
		
		void setMap(rfMap* map);
		
		void setMinMarchDistance(rfUInt distance);
		rfUInt getMinMarchDistance() const;
				
	private:
		void calculateLightPath(rfLight* inLight, rfLightSource* start);
		
		rfUInt lightedBox;
		rfUInt minMarchDistance;
		rfMap* pParentMap;
		
		typedef std::list<rfLightSource*> LIGHT_SOURCE_LIST;
		LIGHT_SOURCE_LIST mLightSourceList;
		
		typedef std::list<rfMirror*> MIRROR_LIST;
		MIRROR_LIST mMirrorList;
		
		typedef std::list<rfShapeBase*> SHAPE_LIST;
		SHAPE_LIST mShapeList;
		
		typedef std::list<rfLight*> LIGHT_LIST;
		LIGHT_LIST mLightList;

	};
} // namespace reflection

#endif  // RF_LIGHT_RENDERER_H_