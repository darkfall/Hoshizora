/*
 *  rfLightSource.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfLightSource.h"

namespace reflection {

	rfLight::rfLight() {}
	rfLight::~rfLight() {
	}
	
	rfPoint rfLight::getDirection() const { 
		return direction;
	}
	
	rfPoint rfLight::getStartPoint() const {
		return ptStart;
	}
	
	rfPoint rfLight::getEndPoint() const {
		return ptEnd;
	}
	
	void rfLight::setDirection(const rfPoint& d) {
		direction = d;
	}
	
	void rfLight::setStartPoint(const rfPoint& s) {
		ptStart = s;
	}
	
	void rfLight::setEndPoint(const rfPoint& e) {
		ptEnd = e;
	}
	
	rfLightSource::rfLightSource(): pLight(NULL), pSourceImage(NULL) {
	}
	
	rfLightSource::~rfLightSource() {
		if(pLight) {
			delete pLight;
			pLight = NULL;
		}
	}
	
	rfLight* rfLightSource::getLight() const {
		return pLight;
	}
	
	rfPoint rfLightSource::getFacing() const {
		return facing;
	}
	
	void rfLightSource::setFacing(const rfPoint& face) {
		facing = face;
	}
	
	rfLight* rfLightSource::shootLight() {
		rfLight* newLight = new rfLight;
		newLight->setDirection(facing);
		newLight->setStartPoint(rfPoint(getX(), getY()));
		if(pLight) {
			delete pLight;
		}
		pLight = newLight;
		return newLight;
	}
	
	void rfLightSource::setSourceImage(sora::SoraSprite* image) {
		pSourceImage = image;
		pSourceImage->setCenter(pSourceImage->getSpriteWidth()/2, pSourceImage->getSpriteHeight()/2);
	}
	
	sora::SoraSprite* rfLightSource::getSourceImage() const {
		return pSourceImage;
	}

	void rfLightSource::enableEditing(bool flag) {
		bEditing = flag;
		if(flag)
			enableDrag(true);
		else
			enableDrag(false);
	}
	
	bool rfLightSource::isEditingEnabled() const {
		return bEditing;
	}
	
	void rfLightSource::draw(gcn::Graphics* graphics) {
		DraggableIcon::draw(graphics);
		if(pSourceImage) {
			pSourceImage->setRotation(-getFacing().Angle());
			pSourceImage->render(getX()+getWidth()/2, getY()+getWidth()/2);
		}
	}
	
	Json::Value& rfLightSource::writeJsonValue(Json::Value& inValue) {
		Json::Value facingValue; 
		facingValue["x"] = facing.x;
		facingValue["y"] = facing.y;
		inValue["facing"] = facingValue;
		inValue["x"] = getX();
		inValue["y"] = getY();
		return inValue;
	}
	
	void rfLightSource::readJsonValue(Json::Value& inValue) {
		if(inValue.isMember("facing")) {
			rfDouble ffacingx = inValue["facing"]["x"].asDouble();
			rfDouble ffacingy = inValue["facing"]["y"].asDouble();

			setFacing(rfPoint(ffacingx, ffacingy));
		}
		if(inValue.isMember("x"))
			setX(inValue["x"].asInt());
		if(inValue.isMember("y"))
			setY(inValue["y"].asInt());
	}
	
} // namespace reflection