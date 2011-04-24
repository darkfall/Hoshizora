/*
 *  rfShape.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfShape.h"

namespace reflection {

	rfShapeBase::rfShapeBase(): currentState(STATE_PLACEABLE), pMirror(NULL), pLightSource(NULL), sprContainer(NULL) {
		enableDragFrame(true);
		
		addWidgetListener(this);
	}
	
	rfShapeBase::~rfShapeBase() {
		if(pMirror) {
			delete pMirror;
			pMirror = NULL;
		}
		if(pLightSource) {
			delete pLightSource;
			pLightSource = NULL;
		}
	}
	
	void rfShapeBase::setStateColor(const gcn::Color& waitforlight, const gcn::Color& lighted) {
		lightedColor = lighted;
		waitForLightColor = waitforlight;
	}

	gcn::Color rfShapeBase::getWaitForLightColor() const {
		return waitForLightColor;
	}
	
	gcn::Color rfShapeBase::getLightedColor() const {
		return lightedColor;
	}
	
	void rfShapeBase::setState(SHAPE_STATE state) {
		currentState = state;
	}
	
	rfShapeBase::SHAPE_STATE rfShapeBase::getState() const {
		return currentState;
	}
	
	rfMirror* rfShapeBase::getMirror() const {
		return pMirror;
	}
	
	void rfShapeBase::removeMirror() {
		if(pMirror) {
			delete pMirror;
			pMirror = NULL;
		}
		setState(STATE_PLACEABLE);
	}
	
	void rfShapeBase::setComponentImage(sora::SoraSprite* image) {
		pComponentImage = image;
	}
	
	sora::SoraSprite* rfShapeBase::getComponentImage () const {
		return pComponentImage;
	}
	
	void rfShapeBase::addLightSource(const rfPoint& facing) {
		rfLightSource* lightSource = new rfLightSource;
		lightSource->setFacing(facing);
		lightSource->setPosition(getX(), getY());
		lightSource->setSourceImage(pComponentImage);
		
		if(pLightSource)
			delete pLightSource;
		pLightSource = lightSource;
		
		setState(STATE_BLOCK);
	}
	
	void rfShapeBase::widgetMoved(const gcn::Event& event) {
		if(!mEnabled)
			return;
		
		if(mDragged) {
			if(pLightSource)
				pLightSource->setPosition(getX(), getY());
			if(pMirror) {
				sora::SoraSprite* psp;
				if((psp = pMirror->getSourceImage()) != NULL) {
					pMirror->setPosition(getX()+(getWidth()-psp->getSpriteWidth())/2, getY()+(getHeight()-psp->getSpriteHeight())/2);
				} else {
					pMirror->setPosition(getX(), getY());
				}
				
			}
		}
	}
	
	void rfShapeBase::setShapeSpriteContainer(rfShapeSpriteContainer* cont) {
		sprContainer = cont;
	}
	
	rfShapeSpriteContainer* rfShapeBase::getShapeSpriteContainer() const {
		return sprContainer;
	}
	
	rfLightSource* rfShapeBase::getLightSource() const {
		return pLightSource;
	}
	
	
	void rfShapeSpriteContainer::pushSprite(rfUInt edgeNum, const rfShapeSprite& sprite) {
		sprites[edgeNum] = sprite;
	}
	
	const rfShapeSprite* rfShapeSpriteContainer::getSpriteByEdgeNum(rfUInt edgeNum) {
		SPRITE_CONT::const_iterator itspr = sprites.find(edgeNum);
		if(itspr != sprites.end()) {
			return &(itspr->second);
		}
		return NULL;
	}
	
	Json::Value& rfShapeSpriteContainer::writeJsonValue(Json::Value& val) {
		SPRITE_CONT::const_iterator itSpr = sprites.begin();
		Json::Value root;
		while(itSpr != sprites.end()) {
			Json::Value val;
			val["name"] = itSpr->second.sImage;
			
			Json::Value nt;
			nt["x"] = itSpr->second.normal.x;
			nt["y"] = itSpr->second.normal.y;
			nt["width"] = itSpr->second.normal.width;
			nt["height"] = itSpr->second.normal.height;
			val["normal"] = nt;
			
			nt["x"] = itSpr->second.pushed.x;
			nt["y"] = itSpr->second.pushed.y;
			nt["width"] = itSpr->second.pushed.width;
			nt["height"] = itSpr->second.pushed.height;
			val["pushed"] = nt;
			
			root[sora::int_to_str(itSpr->first)] = val;
			
			++itSpr;
		}
		val["sprites"] = root;
		return val;
	}
	
	void rfShapeSpriteContainer::readJsonValue(Json::Value& val) {
		Json::Value sprites = val["sprites"];
		if(!sprites.isNull() && sprites.isObject()) {
			Json::Value::Members members = sprites.getMemberNames();
			Json::Value::Members::iterator itMember = members.begin();
			while(itMember != members.end()) {
				const char* name = itMember->c_str();
				Json::Value sprite = sprites[name];
				if(sprite.isObject()) {
					rfShapeSprite shapeSprite;
					if(sprite.isMember("name")) {
						shapeSprite.sImage = sprite["name"].asString();
					}
					
					if(!sprite.isMember("normal") || !sprites.isMember("pushed")) {
				
						Json::Value rect = sprite["normal"];
						shapeSprite.normal = gcn::Rectangle(rect["x"].asInt(),
															rect["y"].asInt(),
															rect["width"].asInt(),
															rect["height"].asInt());
						rect = sprite["pushed"];
						shapeSprite.pushed = gcn::Rectangle(rect["x"].asInt(),
															rect["y"].asInt(),
															rect["width"].asInt(),
															rect["height"].asInt());
					}
				}
				
				++itMember;
			}
		}
	}
	
	rfShapeBox::rfShapeBox(): edgeNum(4) {
		mListener.setParent(this);
		addActionListener(&mListener);
		
		enableDragFrame(true);
	}
	rfShapeBox::~rfShapeBox() {
	}
	
	void rfShapeBox::setEdgeNum(rfUInt num) {
		edgeNum = num;
		
		if(!sprContainer)
			return;
		const rfShapeSprite* spr = sprContainer->getSpriteByEdgeNum(num);
		if(spr) {
			setImage(spr->sImage, spr->normal, spr->pushed);
			adjustSize();
		}
	}
	
	rfUInt rfShapeBox::getEdgeNum() const {
		return edgeNum;
	}
	
	/*
	void rfShapeBox::setSize(rfUInt s) {
		size = s;
		setDimension(gcn::Rectangle(getX(), getY(), s*2, s*2));
		calculateEdgeLength();
	}
	
	rfUInt rfShapeBox::getSize() const {
		return size;
	}
	
	void rfShapeBox::calculateEdgeLength() {
		rfFloat angle = rfDgrToRad(360.f/edgeNum);
		if(angle == 90.f || angle == 60.f)
			edgeLength = size;
		else  
			edgeLength = rfGetThirdEdgeWithEdgeAndAngle(size, size, angle);
	}*/
	
	void rfShapeBox::addMirror() {
		if(pMirror)
			delete pMirror;
		
		pMirror = new rfMirror;
		pMirror->setFacing(rfPoint(1.f, 0.f));
		pMirror->setSourceImage(pComponentImage);
		pMirror->setDimension(gcn::Rectangle(getX()+(getWidth()-pComponentImage->getSpriteWidth())/2, 
											 getY()+(getHeight()-pComponentImage->getSpriteHeight())/2, 
											 pComponentImage->getSpriteWidth(), 
											 pComponentImage->getSpriteHeight()));
		
		setState(STATE_PLACED);
	}
	
	void rfShapeBox::draw(gcn::Graphics* graphics) {		
		if(getImage() != NULL) {
			if(currentState == STATE_WAITFORLIGHT)
				getImage()->setColor(waitForLightColor);
			else if(currentState == STATE_LIGHTED)
				getImage()->setColor(lightedColor);
			else 
				getImage()->setColor(gcn::Color(255, 255, 255));
		}
		DraggableImageButton::draw(graphics);
		
		if(pMirror != NULL)
			pMirror->draw(graphics);
		else if(pLightSource != NULL)
			pLightSource->draw(graphics);
	}
	
	std::string rfShapeBox::getTypeName() const {
		return "shape_box";
	}
	
	Json::Value& rfShapeBox::writeJsonValue(Json::Value& inValue) {
		Json::Value shapeValue;
		shapeValue["x"] = getX();
		shapeValue["y"] = getY();
		shapeValue["edge"] = getEdgeNum();
		shapeValue["selectioncolor"]		= sora::JsonGuiWritter::getHexColor(getSelectionColor());
		shapeValue["lightedcolor"]			= sora::JsonGuiWritter::getHexColor(lightedColor);
		shapeValue["waitforlightcolor"]		= sora::JsonGuiWritter::getHexColor(waitForLightColor);
		if(pMirror) {
			Json::Value mirroValue;
			shapeValue["mirror"]= pMirror->writeJsonValue(mirroValue);
		} else if(pLightSource) {
			Json::Value lightValue;
			shapeValue["lightsource"] = pMirror->writeJsonValue(lightValue);
		}
		inValue[getTypeName()] = shapeValue;
		
		return inValue;
	}
	
	void rfShapeBox::readJsonValue(Json::Value& inValue) {
		if(inValue.isMember("edge"))
			setEdgeNum(inValue["edge"].asInt());
	
		unsigned int lightedColor;
		unsigned int wflColor;
		if(inValue.isMember("lightedcolor")) {
			sscanf(inValue["lightedcolor"].asCString(), "%x", &lightedColor);
		}
		if(inValue.isMember("waitforlightcolor")) {
			sscanf(inValue["waitforlightcolor"].asCString(), "%x", &wflColor);
		}
		setStateColor(gcn::Color(CGETR(lightedColor),
								 CGETG(lightedColor),
								 CGETB(lightedColor),
								 CGETA(lightedColor)),
					  gcn::Color(CGETR(wflColor),
								 CGETG(wflColor),
								 CGETB(wflColor),
								 CGETA(wflColor)));
		if (inValue.isMember("selectioncolor")) {
			unsigned int selectionColor;
			sscanf(inValue["selectioncolor"].asCString(), "%x", &selectionColor);
			setSelectionColor(gcn::Color(CGETR(selectionColor),
										 CGETG(selectionColor),
										 CGETB(selectionColor),
										 CGETA(selectionColor)));
		}
		
		if(inValue.isMember("x"))
			setX(inValue["x"].asInt());
		if(inValue.isMember("y"))
			setY(inValue["y"].asInt());
		
		if(inValue.isMember("mirror")) {
			Json::Value mirrorValue = inValue["mirror"];
			addMirror();
			pMirror->readJsonValue(mirrorValue);
		}
		if(inValue.isMember("lightsource")) {
			Json::Value lightsourceValue = inValue["lightsource"];
			addLightSource(rfPoint(0.f, 0.f));
			pLightSource->readJsonValue(lightsourceValue);
		}
	}
	
} // namespace reflection