/*
 *  rfMirror.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfMirror.h"

namespace reflection {

	rfMirror::rfMirror(): pInLight(NULL), bVisited(false) {
		enableDrag(false);
	}
	
	rfMirror::~rfMirror() {
	}
	
	rfLight* rfMirror::reflect(rfLight* inLight) {
		if(inLight == NULL)
			return NULL;
		
		rfPoint inDir = inLight->getDirection();
		
		float32 inFace = inDir.Angle();
		float32 deltaDir = inDir.Angle(&facing);
		if(abs(deltaDir) == rfDgrToRad(90) || abs(deltaDir) == rfDgrToRad(180)) {
			pInLight = NULL;
			return NULL;
		}
			
		inDir.Rotate(deltaDir*2);
		
		printf("d: %f, in: %f, f: %f, out: %f\n", rfRadToDgr(deltaDir), rfRadToDgr(inFace), rfRadToDgr(facing.Angle()), rfRadToDgr(inDir.Angle()));
		
		if(!pLight) {
			pLight = new rfLight;
		}
		pLight->setDirection(inDir);
		pLight->setStartPoint(rfPoint(getX()+getWidth()/2, getY()+getHeight()/2));
		
		pInLight = inLight;
		
		if(pSourceImage)
			pSourceImage->setRotation(-facing.Angle());
		return pLight;
	}
	
	rfLight* rfMirror::turn(float32 delta) {
		rfPoint dir = getFacing();
		dir.Rotate(delta);
		dir.Normalize();
		setFacing(dir);
		
		return reflect(pInLight);
	}
	
	void rfMirror::setVisited(bool flag) {
		bVisited = flag;
	}
	
	bool rfMirror::isVisited() const {
		return bVisited;
	}
/*	
	Json::Value& rfMirror::writeJsonValue(Json::Value& inValue) {
		Json::Value facingValue; 
		facingValue["x"] = facing.x;
		facingValue["y"] = facing.y;
		inValue["facing"] = facingValue;
		inValue["x"] = getX();
		inValue["y"] = getY();
		return inValue;
	}
	
	void rfMirror::readJsonValue(Json::Value& inValue) {
		if(inValue.isMember("facing")) {
			rfDouble ffacing = inValue["facing"].asDouble();
			setFacing(rfPoint(sin(ffacing), cos(ffacing)));
		}
		if(inValue.isMember("x"))
			setX(inValue["x"].asInt());
		if(inValue.isMember("y"))
			setY(inValue["y"].asInt());
		
		printf("mirr\n");
	}*/

} // namespace reflection