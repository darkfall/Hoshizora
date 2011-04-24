/*
 *  rfMap.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_MAP_REV_H_
#define RF_MAP_REV_H_

#include "rfLightRenderer.h"
#include "gcnExtend/gcnSelectableContainer.h"

namespace reflection {

	
	/*
	 basic container for a reflection map
	 */
	class rfMap: public gcn::SelectableContainer {
	public:
		enum MAP_STATE {
			STATE_EDITING,
			STATE_PLAYING,
			STATE_FAILED,
			STATE_WAITING,
		};
		
		rfMap();
		virtual ~rfMap();
		
		void setState(MAP_STATE state);
		MAP_STATE getState() const;
		
		/*
		 inheritated from SelectableContainer
		 when in editing mode
		 handle the event to SelectableContainer
		 otherwise update map
		 */
		virtual void mousePressed(gcn::MouseEvent& event);
		virtual void mouseReleased(gcn::MouseEvent& event);
		
		
		virtual void add(gcn::Widget* widget);
		
		void pushSprite(rfUInt edgeNum, const rfShapeSprite& sprite);
		
		void setShapeSpriteContainer(const rfShapeSpriteContainer& cont);
		rfShapeSpriteContainer* getShapeSpriteContainer();
		
		void setMirrorImage(const std::wstring mirrorImage);
		sora::SoraSprite* getMirrorImage() const;
		
		void setLightSourceImage(const std::wstring lsImage);
		sora::SoraSprite* getLightSourceImage() const;
		
		bool writeConfig(const std::wstring& config, bool bStylish=true);
		bool loadConfig(const std::wstring& config);
		
		bool write(const std::wstring& file, bool bStylish=true);
		bool load(const std::wstring& file);
		
		virtual void logic();
		virtual void draw(gcn::Graphics* graphics);
		
		void updateShape(rfShapeBase* shape);
		void updateMirror(rfMirror* mirror);
		void updateLightSource(rfLightSource* ls);
		
		void removeShape(rfShapeBase* shape);
		void removeMirror(rfMirror* mirror);
		void removeLightSource(rfLightSource* ls);
		
		void recalculateLightMap();
		
		rfUInt getLightedBoxNumber() const;
		
		rfUInt getLightNumber() const;
		rfUInt getLightSourceNumber() const;
		rfUInt getShapeBoxNumber() const;
		rfUInt getMirrorNumber() const;
		
	private:
		void enableEditing();
		void disableEditing();
		
		void resetState();
		void calculateLightMap();
		
		Json::Value writeProperties();
		void readProperties(Json::Value& val);
		
		rfShapeBase* identTypeByName(const std::string& name);

		rfShapeSpriteContainer rootSprCont;
		
		sora::SoraSprite* pMirrorImage;
		std::wstring mirrorImagePath;
		
		sora::SoraSprite* pLightSourceImage;
		std::wstring lightSourceImagePath;
		
		rfUInt lightedShape;
		rfUInt shapeToLight;
		
		rfFloat totalTime;
		rfFloat timePassed;
		
		rfUInt totalScore;
		
		MAP_STATE mapState;
		
		rfMapLightRenderer lightRenderer;
	};
	
	
} // namespace reflection

#endif // namespace reflection