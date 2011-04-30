/*
 *  rfLightRenderer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rfLightRenderer.h"
#include "rfMap.h"

namespace reflection {
	
	rfMapLightRenderer::rfMapLightRenderer(): pParentMap(NULL), minMarchDistance(10), lightedBox(0) {}

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
		if(!pParentMap) return;
		
		lightedBox = 0;
		mLightList.clear();
		
		/*
		 set all mirror unvisited
		 */
		MIRROR_LIST::iterator itMirror = mMirrorList.begin();
		while(itMirror != mMirrorList.end()) {
			if((*itMirror))
			   (*itMirror)->setVisited(false);
			else {
				itMirror = mMirrorList.erase(itMirror);
				continue;
			}
			++itMirror;
		}
		
		LIGHT_SOURCE_LIST::iterator itLight = mLightSourceList.begin();
		while(itLight != mLightSourceList.end()) {
			rfLight* startLight = (*itLight)->shootLight();
			calculateLightPath(startLight, (*itLight));
			
			++itLight;
		}
	}
	
	void rfMapLightRenderer::calculateLightPath(rfLight* inLight, rfLightSource* start) {
		rfPoint startPos = inLight->getStartPoint();
		gcn::Rectangle mapRange = pParentMap->getDimension();
		
		rfFloat marchDir = inLight->getDirection();
		rfPoint currPos = startPos;
		rfFloat sh = sora::SORA->getScreenHeight();
		while(mapRange.isPointInRect(currPos.x, currPos.y)) {
			rfPoint nextPos(currPos.x + cosf(marchDir) * minMarchDistance, currPos.y - sinf(marchDir) * minMarchDistance);
			//printf("nextPos: %f,%f\n", nextPos.x, nextPos.y);
			gcn::Widget* pw = pParentMap->getWidgetAt(nextPos.x, nextPos.y);
			if(pw == NULL || pw == start) {
				currPos = nextPos;
				continue;
			}/* else if(pw == start) {
				// backing into the samepos, circle, break
				break;
			}*/
			
			// is shape
			rfShapeBase* shape = dynamic_cast<rfShapeBase*> (pw);
			if(shape != NULL) {
				if(shape->getState() == rfShapeBase::STATE_WAITFORLIGHT)
					shape->setState(rfShapeBase::STATE_LIGHTED);
				else {
					
					// is mirror
					// reflect a new light
					rfMirror* mirror = shape->getMirror();
					if(mirror != NULL) {
						// backing to the same mirror,
						// a circle
						// break
						if(mirror->isVisited()) {
							break;
						}
						if(!mirror->getDimension().isPointInRect(currPos.x, currPos.y)) {
							currPos.x = currPos.x + cosf(marchDir) * minMarchDistance / 2;
							currPos.y = currPos.y - sinf(marchDir) * minMarchDistance / 2;
							continue;
						}
						
						
					//	inLight->setDirection(atan2f(-mirror->getY()+startPos.y, mirror->getX()-startPos.x));
						rfLight* newLight = mirror->reflect(inLight);
						
						if(newLight != NULL) {
							mLightList.push_back(inLight);
							inLight->setEndPoint(rfPoint(mirror->getX()+mirror->getWidth()/2, (mirror->getY()+mirror->getHeight()/2)));

							inLight = newLight;
							marchDir = inLight->getDirection();
							mirror->setVisited(true);
							inLight->setStartPoint(rfPoint(mirror->getX()+mirror->getWidth()/2, (mirror->getY()+mirror->getHeight()/2)));
							
							currPos.x = currPos.x + cosf(marchDir) * shape->getWidth() * 2.f;
							currPos.y = currPos.y - sinf(marchDir) * shape->getHeight() * 2.f;
							gcn::Widget* pw2 = pParentMap->getWidgetAt(currPos.x, currPos.y);
							while(pw2 == pw) {
								currPos.x = currPos.x + cosf(marchDir) * shape->getWidth() * 2.f;
								currPos.y = currPos.y - sinf(marchDir) * shape->getHeight() * 2.f;
							}
							
							printf("inserting light, spos=%f,%f, epos=%f,%f, dir=%f\n", inLight->getStartPoint().x, inLight->getStartPoint().y,
								   inLight->getEndPoint().x, inLight->getEndPoint().y, rfRadToDgr(marchDir));
							startPos = currPos;
							
							continue;
						}
					}
				}
			}			
			currPos = nextPos;
		}
		inLight->setEndPoint(currPos);
		mLightList.push_back(inLight);
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
	
	void rfMapLightRenderer::setMap(rfMap* map) {
		pParentMap = map;
	}
	
	void rfMapLightRenderer::setMinMarchDistance(rfUInt distance) {
		minMarchDistance = distance;
	}
	
	rfUInt rfMapLightRenderer::getMinMarchDistance() const {
		return minMarchDistance;
	}

} // namespace reflection