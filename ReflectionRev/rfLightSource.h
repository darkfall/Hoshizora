/*
 *  rfLightSource.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_LIGHT_SOURCE_H_
#define RF_LIGHT_SOURCE_H_

#include "rfCommon.h"
#include "SoraSprite.h"
#include "gcnExtend/gcnDraggableIcon.h"

namespace reflection {
	
	/*
	 base class for a light, controlled by lightsource
	 */
	class rfLight {
	public:
		rfLight();
		~rfLight();
		
		rfPoint getDirection() const;
		rfPoint getStartPoint() const;
		rfPoint getEndPoint() const;
		
		void setDirection(const rfPoint& d);
		void setStartPoint(const rfPoint& s);
		void setEndPoint(const rfPoint& e);
		
		virtual void draw(gcn::Graphics* graphics);
		virtual void logic();
		
	protected:
		rfPoint direction;		
		rfPoint ptStart;
		rfPoint ptEnd;
	};
	
	class rfMap;
		
	/*
	 base class for a light source, which can shoot a light
	 */
	class rfLightSource: public gcn::DraggableIcon {
	public:
		rfLightSource();
		virtual ~rfLightSource();
		
		void setFacing(const rfPoint& face);
		rfPoint getFacing() const;
		
		rfLight* getLight() const;
		rfLight* shootLight();
		
		void setSourceImage(sora::SoraSprite* image);
		sora::SoraSprite* getSourceImage() const;
		
		void enableEditing(bool flag);
		bool isEditingEnabled() const;
		
		virtual void draw(gcn::Graphics* graphics);
		
		virtual Json::Value& writeJsonValue(Json::Value& inValue);
		virtual void readJsonValue(Json::Value& inValue);
		
		void setParentMap(rfMap* map);
		rfMap* getParentMap() const;
		
	protected:
		rfLight* pLight;
		rfPoint facing;
				
		bool bEditing;
		sora::SoraSprite* pSourceImage;
		
		rfMap* pParentMap;
	};
	
	
} // namespace reflection


#endif // RF_LIGHT_SOURCE_H_