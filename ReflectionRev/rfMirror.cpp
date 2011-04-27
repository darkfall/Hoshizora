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
				
		rfFloat inFace = inLight->getDirection() + rfDgrToRad(180.f);
		if(inFace < 0)
			inFace = rfDgrToRad(360.f) + inFace;
		rfFloat mirrorFace = facing;
		
		float32 deltaDir = mirrorFace - inFace;

		printf("inFace: %f, mirrorFace: %f, deltaFace: %f, out:%f\n", rfRadToDgr(inFace), rfRadToDgr(mirrorFace), rfRadToDgr(deltaDir), rfRadToDgr(inFace+deltaDir*2));

		inFace += deltaDir*2;
//		inFace += rfDgrToRad(180.f);
				
		if(!pLight) {
			pLight = new rfLight;
		}
		pLight->setDirection(inFace);
		pLight->setStartPoint(rfPoint(getX()+getWidth()/2, getY()+getHeight()/2));
		
		pInLight = inLight;
		
		if(pSourceImage)
			pSourceImage->setRotation(-facing);
		return pLight;
	}
	
	rfLight* rfMirror::turn(float32 delta) {
		setFacing(getFacing()+delta);
		if(facing > rfDgrToRad(360.f))
			facing = facing - rfDgrToRad(360.f);
		
	//	facing = rfDgrToRad((rfInt)rfRadToDgr(facing));
		
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