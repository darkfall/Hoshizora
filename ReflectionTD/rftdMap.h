/*
 *  rftdMap.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_MAP_H_
#define RFTD_MAP_H_

#include "gcnExtend/gcnBackgroundContainer.h"

#include "json/json.h"

#include "rftdMapGrid.h"
#include "rftdSpawnPoint.h"
#include "rftdMapEnemyCont.h"

namespace rftd {
	
	enum {
		MAP_MODE_PLAY = 1,
		MAP_MODE_EDIT = 2
	};
	
	class rftdMap: public gcn::BackgroundContainer, public gcn::MouseListener, public gcn::KeyListener {
	public:
		rftdMap();
		virtual ~rftdMap();
		
		bool readMapConf(const std::wstring& val);
		
		void setMode(int32 mode);
		
		void start();
		void pause();
		void reset();
		
		int32 getEnemySize() const;
		int32 getCurrLevel() const;
		float32 getCurrLevelInterval() const;
		float32 getLevelInterval() const;
		std::wstring getCurrLevelName() const;
		bool isMapFinished() const;
		
		/*
		 inheritated from BackgroundContainer
		 */
		virtual void logic();
		virtual void draw(gcn::Graphics* graphics);
		/*
		 inheritated from MouseListener
		 */
		virtual void mousePressed(gcn::MouseEvent& mouseEvent);
		virtual void mouseReleased(gcn::MouseEvent& mouseEvent);
        virtual void mouseDragged(gcn::MouseEvent& mouseEvent);
		
		/*
		 inheritated from KeyListener
		 */
		virtual void keyPressed(gcn::KeyEvent& kev);
		virtual void keyReleased(gcn::KeyEvent& kev);
		
		
	private:
		rftdMapEnemyCont* enemyCont;
		rftdMapGrid* mBuildMap;
		
		sora::SoraSprite* mBGSprite;
	};
	
} // namespace rftd

#endif // RFTD_MA_H_