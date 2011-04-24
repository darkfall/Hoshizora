/*
 *  rfMap.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfMap.h"
#include "SoraFileUtility.h"
#include "json/json.h"

namespace reflection {

	rfMap::rfMap(): pMirrorImage(NULL), pLightSourceImage(NULL) {
		setState(STATE_WAITING);
	}
	
	rfMap::~rfMap() {
		if(pMirrorImage) {
			delete pMirrorImage;
			pMirrorImage = NULL;
		}
		if(pLightSourceImage) {
			delete pLightSourceImage;
			pLightSourceImage = NULL;
		}
	}
	
	void rfMap::setState(MAP_STATE state) {
		mapState = state;
		resetState();
		switch(state) {
			case STATE_PLAYING:
				disableEditing();
				break;
				
			case STATE_EDITING:
				enableEditing();
				break;
				
			case STATE_WAITING:
				disableEditing();
				break;
				
			case STATE_FAILED:
				disableEditing();
				break;
		}
	}
	
	rfMap::MAP_STATE rfMap::getState() const {
		return mapState;
	}

	void rfMap::mousePressed(gcn::MouseEvent& event) {
		if(mapState == STATE_EDITING)
			SelectableContainer::mousePressed(event);
		else {
			
		}
	}
	
	void rfMap::mouseReleased(gcn::MouseEvent& event) {
		if(mapState == STATE_EDITING)
			SelectableContainer::mouseReleased(event);
		else {
			
		}
	}
	
	void rfMap::setMirrorImage(const std::wstring mirrorImage) {
		pMirrorImage = sora::SORA->createSprite(mirrorImage);
		mirrorImagePath = mirrorImage;
	}
	
	sora::SoraSprite* rfMap::getMirrorImage() const {
		return pMirrorImage;
	}
	
	void rfMap::setLightSourceImage(const std::wstring lsImage) {
		pLightSourceImage = sora::SORA->createSprite(lsImage);
		lightSourceImagePath = lsImage;
	}
	
	sora::SoraSprite* rfMap::getLightSourceImage() const {
		return pLightSourceImage;
	}
	
	void rfMap::calculateLightMap() {
	}
	
	void rfMap::pushSprite(rfUInt edgeNum, const rfShapeSprite& sprite) {
		rootSprCont.pushSprite(edgeNum, sprite);
	}
	
	void rfMap::setShapeSpriteContainer(const rfShapeSpriteContainer& cont) {
		rootSprCont = cont;
	}
	
	rfShapeSpriteContainer* rfMap::getShapeSpriteContainer() {
		return &rootSprCont;
	}
	
	void rfMap::add(gcn::Widget* widget) {
		SelectableContainer::add(widget);
		
		rfShapeBase* prsb = dynamic_cast<rfShapeBase*> (widget);
		if(prsb) {
			prsb->setComponentImage(pMirrorImage);
			prsb->setShapeSpriteContainer(&rootSprCont);
			return;
		}
		
		rfLightSource* pls = dynamic_cast<rfLightSource*> (widget);
		if(pls) {
			pls->setSourceImage(pLightSourceImage);
			return;
		}
	}
	
	Json::Value rfMap::writeProperties() {
		Json::Value props;
		props["mirrorImagePath"] = sora::ws2s(mirrorImagePath);
		props["lightSourceImagePath"] = sora::ws2s(lightSourceImagePath);
		props["state"] = mapState;
		props["x"] = getX();
		props["y"] = getY();
		props["bgImage"] = getImage()->getName();
		props["name" ] = getId();
		props["width"] = getWidth();
		props["height"] = getHeight();
		return props;
	}
	
	void rfMap::readProperties(Json::Value& val) {
		Json::Value props = val["properties"];
		if(!props.isNull()) {
			mirrorImagePath = sora::s2ws(props["mirrorImagePath"].asString());
			if(mirrorImagePath.size() != 0)
				pMirrorImage = sora::SORA->createSprite(mirrorImagePath);
			
			lightSourceImagePath = sora::s2ws(props["lightSourceImagePath"].asString());
			if(lightSourceImagePath.size() != 0)
				pLightSourceImage = sora::SORA->createSprite(lightSourceImagePath);
			
			mapState = (MAP_STATE)props["state"].asInt();
			
			setDimension(gcn::Rectangle(props["x"].asInt(),
										props["y"].asInt(),
										props["width"].asInt(),
										props["height"].asInt()));
			
			setImage(props["bgImage"].asString());
			
			setId(props["name"].asString());
		}
	}
	
	bool rfMap::writeConfig(const std::wstring& filename, bool bStylish) {
		std::string realPath = sora::ws2s(sora::SoraFileUtility::getWrittablePath(filename));
		
		Json::Value Root;
		Root = rootSprCont.writeJsonValue(Root);
		Root["properties"] = writeProperties();
		
		std::string configString;
		if(bStylish) {
			Json::StyledWriter writer;
			configString = writer.write(Root);
		} else {
			Json::FastWriter writer;
			configString = writer.write(Root);
		}
		
		std::ofstream output(realPath.c_str());
		if(!output.is_open())
			return false;
		output<<configString;
		output.close();
		return true;
	}
	
	bool rfMap::loadConfig(const std::wstring& filename) {
		ulong32 size;
		void* data = sora::SORA->getResourceFile(filename, size);
		if(data) {
			Json::Reader reader;
			Json::Value root;
			if(reader.parse((const char*)data, (const char*)data+size, root)) {
				rootSprCont.readJsonValue(root);
				readProperties(root);
				return true;
			}
		}
		return false;
	}
	
	bool rfMap::write(const std::wstring& filename, bool bStylish) {
		std::string realPath = sora::ws2s(sora::SoraFileUtility::getWrittablePath(filename));
		
		Json::Value Root;
		Json::Value shapesArray;
		for(WidgetList::iterator itWidget = mWidgets.begin();
			itWidget != mWidgets.end();
			++itWidget) {
			rfShapeBase* shape = dynamic_cast<rfShapeBase*> ((*itWidget));
			if(shape) {
				Json::Value shapeValue;
				shape->writeJsonValue(shapeValue);
				shapesArray.append(shapeValue);
			}
		}
		Root["shapes"] = shapesArray;
		
		std::string jdoc;
		if(bStylish) {
			Json::StyledWriter writer;
			jdoc = writer.write(Root);
		} else {
			Json::FastWriter writer;
			jdoc = writer.write(Root);
		}
		
		std::ofstream output(realPath.c_str());
		if(!output.is_open())
			return false;
		
		output<<jdoc;
		output.close();
		
		return true;
	}
	
	rfShapeBase* rfMap::identTypeByName(const std::string& name) {
		if(name.compare("shape_box") == 0)
			return new rfShapeBox;
		return NULL;
	}
	
	bool rfMap::load(const std::wstring& filename) {
		clear();
		resetState();
		
		ulong32 size;
		void* data = sora::SORA->getResourceFile(filename, size);
		if(data) {
			Json::Reader reader;
			Json::Value root;
			if(reader.parse((const char*)data, (const char*)data+size, root)) {
				Json::Value shapesArray = root["shapes"];
				if(!shapesArray.isNull() && shapesArray.isArray()) {
					rfUInt size = shapesArray.size();
					for(rfUInt i=0; i<size; ++i) {
						Json::Value shapeRoot = shapesArray[i];
						
						Json::Value::Members members = shapeRoot.getMemberNames();
						Json::Value::Members::iterator itMember = members.begin();
						while(itMember != members.end()) {
							rfShapeBase* shape = identTypeByName(itMember->c_str());
							if(shape != NULL) {
								// add to container
								add(shape);
								
								shape->readJsonValue(shapeRoot[itMember->c_str()]);
							}
							++itMember;
						}
					}
				}
				setState(STATE_WAITING);
				return true;
			}
		}
		return false;
	}
	
	void rfMap::logic() {
		if(mapState == STATE_PLAYING) {
			timePassed += sora::SORA->getDelta();
			if(totalTime != 0.f) {
				if(timePassed >= totalTime)
					setState(STATE_FAILED);
			}
		}
		if(mapState != STATE_WAITING)
			lightRenderer.logic();
		
		SelectableContainer::logic();
	}
	
	void rfMap::draw(gcn::Graphics* graphics) {
		if(mapState != STATE_WAITING)
			lightRenderer.draw(graphics);
		
		SelectableContainer::draw(graphics);
	}
	
	void rfMap::resetState() {
		totalTime = timePassed = 0.f;
		lightedShape = shapeToLight = 0;
		totalScore = 0;
	}
	
	void rfMap::enableEditing() { 
		setSelectable(true);

		WidgetList::iterator itWidget = mWidgets.begin();
		while(itWidget != mWidgets.end()) {
			rfShapeBase* prsb = dynamic_cast<rfShapeBase*> ((*itWidget));
			if(prsb) {
				prsb->enableDrag(true);
				return;
			}
			++itWidget;
		}
	}
	
	void rfMap::disableEditing() {
		setSelectable(false);

		WidgetList::iterator itWidget = mWidgets.begin();
		while(itWidget != mWidgets.end()) {
			rfShapeBase* prsb = dynamic_cast<rfShapeBase*> ((*itWidget));
			if(prsb) {
				prsb->enableDrag(false);
				return;
			}
			++itWidget;
		}
	}

} // namespace reflection