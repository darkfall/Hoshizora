/*
 *  rftdMap.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdMap.h"
#include "rftdEnemyObject.h"
#include "SoraLocalizer.h"

namespace rftd {
	
	rftdMap::rftdMap() {
		enemyCont = NULL;
		mBGSprite = NULL;
	}

	rftdMap::~rftdMap() {
		if(enemyCont) {
			delete enemyCont;
			enemyCont = NULL;
		}
		if(mBGSprite) {
			delete mBGSprite;
			mBGSprite = NULL;
		}
	}
	
	void rftdMap::logic() {
		BackgroundContainer::logic();
		
		float32 dt = sora::SORA->getDelta();
		if(mBGSprite)
			mBGSprite->update(dt);
		if(enemyCont)
			enemyCont->update(dt);
	}
	
	void rftdMap::draw(gcn::Graphics* graphics) {
		BackgroundContainer::draw(graphics);
		if(mBGSprite)
			mBGSprite->render(getX(), getY());
		if(enemyCont)
			enemyCont->render();
	}
	
	void rftdMap::setMode(int32 mode) {
		switch (mode) {
			case MAP_MODE_EDIT:
				addMouseListener(this);
				addKeyListener(this);
				break;
			case MAP_MODE_PLAY:
				// disable key callbacks;
				removeMouseListener(this);
				removeKeyListener(this);
				break;
		}
	}
	
	bool rftdMap::readMapConf(const std::wstring& val) {
		ulong32 size;
		const char* data = (const char*)sora::SORA->getResourceFile(val, size);
		if(data) {
			Json::Reader reader;
			Json::Value root;
			if(reader.parse(data, data+size, root)) {			
				Json::Value enemyConfs = root["enemy_confs"];
				if(enemyConfs.isArray()) {
					EnemyConfLoader::Instance()->clear();
					for(size_t i=0; i<enemyConfs.size(); ++i)
						EnemyConfLoader::Instance()->readEnemyConf(sora::s2ws(enemyConfs[i].asString()));
				}
				
				if(root.isMember("background_tex")) {
					mBGSprite = PRODUCE_SPRITE(root["background_tex"]);
				}
				
				if(root.isMember("map_rect")) {
					Json::Value mapRectVal = root["map_rect"];
					if(mapRectVal.isObject()) {
						setDimension(gcn::Rectangle(mapRectVal["x"].asInt(),
													mapRectVal["y"].asInt(),
													mapRectVal["w"].asInt(),
													mapRectVal["h"].asInt()));
					}
				}
				
				
				if(enemyCont) {
					delete enemyCont;
				}
				enemyCont = new rftdMapEnemyCont(this);
				if(!enemyCont->readLevelConf(root)) {
					delete enemyCont;
					enemyCont = NULL;
					sora::SORA->messageBoxW(L"Error parsing map conf "+val, L"Error", MB_OK | MB_ICONERROR);
				}
				
				sora::SORA->freeResourceFile((void*)data);
				return true;
			} else 
				sora::SORA->messageBoxW(L"Error parsing map conf "+val, L"Error", MB_OK | MB_ICONERROR);
			
			sora::SORA->freeResourceFile((void*)data);
		} else 
			sora::SORA->messageBoxW(L"Error loading map conf "+val, L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	
	void rftdMap::mousePressed(gcn::MouseEvent& mouseEvent) {
	}
	
	void rftdMap::mouseReleased(gcn::MouseEvent& mouseEvent) {
	}
	
	void rftdMap::mouseDragged(gcn::MouseEvent& mouseEvent) {
	}
	
	void rftdMap::keyPressed(gcn::KeyEvent& kev) {
	}
	
	void rftdMap::keyReleased(gcn::KeyEvent& kev) {
	}
	
	void rftdMap::start() {
		if(enemyCont)
			enemyCont->start();
	}
	
	void rftdMap::pause() {
		if(enemyCont)
			enemyCont->pause();
	}
	
	void rftdMap::reset() {
		if(enemyCont)
			enemyCont->reset();
	}
	
	int32 rftdMap::getEnemySize() const {
		if(enemyCont)
			return enemyCont->getEnemySize();
		return 0;
	}
	
	float32 rftdMap::getCurrLevelInterval() const {
		if(enemyCont)
			return enemyCont->getCurrLevelInterval();
		return 0.f;
	}
	
	float32 rftdMap::getLevelInterval() const {
		if(enemyCont)
			return enemyCont->getLevelInterval();
		return 0.f;
	}
	
	int32 rftdMap::getCurrLevel() const {
		if(enemyCont)
			return enemyCont->getCurrLevel();
		return 0;
	}

	std::wstring rftdMap::getCurrLevelName() const {
		if(enemyCont)
			return enemyCont->getCurrLevelName();
		return L"\0";
	}
	
	bool rftdMap::isMapFinished() const {
		if(enemyCont)
			return enemyCont->isMapFinished();
		return true;
		
	}

} // namespace rftd