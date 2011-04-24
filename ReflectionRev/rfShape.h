/*
 *  rfShape.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_SHAPE_REV_H_
#define RF_SHAPE_REV_H_

#include "rfCommon.h"
#include "rfLightSource.h"
#include "rfMirror.h"
#include "SoraSprite.h"
#include "gcnExtend/gcnDraggableImageButton.h"

#include <map>

namespace reflection {
	
	
	struct rfShapeSprite {
		std::string sImage;
		gcn::Rectangle normal;
		gcn::Rectangle pushed;
		
		rfShapeSprite() {}
		rfShapeSprite(const std::string& image, const gcn::Rectangle& nrect, const gcn::Rectangle& prect):
		sImage(image), normal(nrect), pushed(prect) {}
	};
	
	class rfShapeSpriteContainer {
		friend class rfShapeBox;
		friend class rfShapeCircle;
		friend class rfShapeBase;
		
	public:
		void pushSprite(rfUInt edgeNum, const rfShapeSprite& sprite);
		const rfShapeSprite* getSpriteByEdgeNum(rfUInt edgeNum);
		
		Json::Value& writeJsonValue(Json::Value& val);
		void readJsonValue(Json::Value& val);
		
	private:
		typedef std::map<rfUInt, rfShapeSprite> SPRITE_CONT;
		SPRITE_CONT sprites;
	};
		
	/*
	 base class for all shape
	 */
	class rfShapeBase: public gcn::DraggableImageButton, public gcn::WidgetListener {
	public:
		enum SHAPE_STATE {
			STATE_LIGHTED,
			STATE_WAITFORLIGHT,
			STATE_PLACEABLE,
			STATE_PLACED,
			STATE_BLOCK,
		};
		
		rfShapeBase();
		virtual ~rfShapeBase();
		
		void setStateColor(const gcn::Color& waitforlight, const gcn::Color& lighted);
		gcn::Color getLightedColor() const;
		gcn::Color getWaitForLightColor() const;
		
		void setState(SHAPE_STATE state);
		SHAPE_STATE getState() const;
		
		virtual void addMirror() = 0;
		void removeMirror();
		rfMirror* getMirror() const;
		
		void addLightSource(const rfPoint& facing);
		rfLightSource* getLightSource() const;
		
		void setComponentImage(sora::SoraSprite* image);
		sora::SoraSprite* getComponentImage() const;
		
		virtual Json::Value& writeJsonValue(Json::Value& inValue) = 0;
		virtual void readJsonValue(Json::Value& inValue) = 0;
		
		virtual std::string getTypeName() const = 0;
		
		void setShapeSpriteContainer(rfShapeSpriteContainer* cont);
		rfShapeSpriteContainer* getShapeSpriteContainer() const;
		
		/*
		 inheritant from widget listener
		 */
		virtual void widgetMoved(const gcn::Event& event);
		
		void setParentMap(rfMap* map) ;
		rfMap* getParentMap() const;

	protected:
		gcn::Color lightedColor;
		gcn::Color waitForLightColor;
		
		sora::SoraSprite* pComponentImage;
		
		rfShapeSpriteContainer* sprContainer;
		
		SHAPE_STATE currentState;
		
		/*
		 the light source that the shape have
		 if this is not null, then the shape is a block
		 */
		rfLightSource* pLightSource;
		/*
		 the mirror that the shape have
		 state must equal to placeable to contain mirror
		 */
		rfMirror* pMirror;
		
		rfMap* pParentMap;
	};
		
	/*
	 box shape,
	 handle any right even n_edge shape
	 */
	class rfShapeBox: public rfShapeBase {
		friend class rfShapeBoxActionListener;
	public:
		rfShapeBox();
		virtual ~rfShapeBox();
		
		void setEdgeNum(rfUInt number);
		rfUInt getEdgeNum() const;
						
		virtual void addMirror();
		
		/*
		 inheritate from image button
		 */
		virtual void draw(gcn::Graphics* graphics);
		
		/* 
		 inheritate from shape
		 */
		virtual Json::Value& writeJsonValue(Json::Value& inValue);
		virtual void readJsonValue(Json::Value& inValue);
		
		std::string getTypeName() const;

	protected:		
		rfUInt edgeNum;
				
		class rfShapeBoxActionListener: public gcn::ActionListener {
		public:
			rfShapeBoxActionListener(): pParent(NULL) {}
			rfShapeBoxActionListener(rfShapeBox* parent): pParent(parent) {}
			
			void setParent(rfShapeBox* parent) {
				pParent = parent;
			}
			
			void action(const gcn::ActionEvent& actionEvent);
				   
			rfShapeBox* pParent;
		};
		rfShapeBoxActionListener mListener;
	};
	
	/*
	 circle shape
	 handle a circle
	 */
	class rfShapeCircle: public rfShapeBase {
	public:
		rfShapeCircle();
		virtual ~rfShapeCircle();
	};
	
	
} // namespace reflection

#endif // RF_SHAPE_REV_H_