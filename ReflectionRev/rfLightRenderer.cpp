/*
 *  rfLightRenderer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfLightRenderer.h"

namespace reflection {

	void rfMapLightRenderer::addLightSource(rfLightSource* llight) {
		mLightSourceList.push_back(llight);
		recalculate();
	}
	
	void rfMapLightRenderer::addMirror(rfMirror* mirror) {
		mMirrorList.push_back(mirror);
		recalculate();
	}
	
	void rfMapLightRenderer::addToLightBox(rfShapeBase* shape) {
		mShapeList.push_back(shape);
		recalculate();
	}
	
	void rfMapLightRenderer::removeLightSource(rfLightSource* light) {
		mLightSourceList.remove(light);
		recalculate();
	}
	
	void rfMapLightRenderer::removeMirror(rfMirror* mirror) {
		mMirrorList.remove(mirror);
		recalculate();
	}
	
	void rfMapLightRenderer::removeToLightBox(rfShapeBase* shape) {
		mShapeList.remove(shape);
		recalculate();
	}
	
	void rfMapLightRenderer::addReflectionLight(rfLight* light) {
		mLightList.push_back(light);
	}
	
	void rfMapLightRenderer::reset() {
		mLightSourceList.clear();
		mMirrorList.clear();
		mShapeList.clear();
		mLightList.clear();
	}
	
	void rfMapLightRenderer::recalculate() {
		lightedBox = 0;
		mLightList.clear();
		
		LIGHT_SOURCE_LIST::iterator itLight = mLightSourceList.begin();
		while(itLight != mLightSourceList.end()) {
			rfLight* startLight = (*itLight)->shootLight();
			calculateLightPath(startLight);
		}
	}
	
	void rfMapLightRenderer::calculateLightPath(rfLight* inLight) {
	}
	
	void rfMapLightRenderer::draw(gcn::Graphics* graphics) {
		LIGHT_LIST::iterator itLight = mLightList.begin();
		while(itLight != mLightList.end()) {
			(*itLight)->draw(graphics);
			++itLight;
		}
	}
	
	void rfMapLightRenderer::logic() {
		LIGHT_LIST::iterator itLight = mLightList.begin();
		while(itLight != mLightList.end()) {
			(*itLight)->logic();
			++itLight;
		}
	}
	
	rfUInt rfMapLightRenderer::getLightedBoxNumber() const {
		return lightedBox;
	}
	
	rfUInt rfMapLightRenderer::getLightNumber() const {
		return mLightList.size();
	}
	
	rfUInt rfMapLightRenderer::getLightSourceNumber() const {
		return mLightSourceList.size();
	}
	
	rfUInt rfMapLightRenderer::getShapeBoxNumber() const {
		return mShapeList.size();
	}
	
	rfUInt rfMapLightRenderer::getMirrorNumber() const {
		return mMirrorList.size();
	}

} // namespace reflection