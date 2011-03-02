/*
 *  lScene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_SCENE_H_
#define L_SCENE_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraSingleton.h"
#include "SoraCore.h"
#include "SoraTimerEvent.h"

#include <list>

namespace projl {
	
#define LSCENE_TYPE 0x00000010
	
	class lSceneManager;
	
	enum lSceneTimerEventId {
		EVENT_NULL,
		EVENT_SWITCH_TO_START,
		EVENT_SWITCH_FROM_START,
		EVENT_SWITCH_TO_HALF,
		EVENT_SWITCH_FROM_HALF,
		EVENT_SWITCH_TO_FINISH,
		EVENT_SWITCH_FROM_FINISH,
	};
	
	class lSceneTimerEvent: public sora::SoraTimerEvent {
	public:
		lSceneTimerEvent(lSceneTimerEventId _id): id(_id) {}
		lSceneTimerEvent(): id(EVENT_NULL) {}
		lSceneTimerEventId id;
		float32 switchTime;
	};
	
	class lScene: public sora::SoraObject {
		friend class lSceneManager;
		
	public:
		lScene();
		virtual ~lScene();
	
		void add(SoraObject* obj);
		
		virtual void render();
		virtual uint32 update(float32 dt);
		virtual void onSceneTimerEvent(const lSceneTimerEvent* tev) = 0;
		
		float32 getSceneWidth() const;
		float32 getSceneHeight() const;
		float32 getScenePosX() const;
		float32 getScenePosY() const;
		
		void setDimension(float32 x, float32 y, float32 w, float32 h);
		
		bool isActive() const;
		
		lSceneManager* getParentManager() const;
		
		void setDepth(float32 _depth) { depth = _depth; }
		float32 getDepth() const { return depth; }
		
	private:
		hgeRect sceneRect;
		
		float32 depth;
		bool bActive;
		
		lSceneManager* pParentManager;
	}; 
	
	
	class lSceneManager: public sora::SoraEventHandler {
	public:
		lSceneManager(float32 x, float32 y, float32 w, float32 h);
		~lSceneManager();
		
	public:
		
		void addScene(lScene* scene);
		void delScene(lScene* scene);
		
		void update(float32 dt);
		void render();
		
		void setSwitchColor(const sora::SoraColorRGBA& color);
		void setSwitchEffectEnabled(bool flag);
		
		/* 
		 @param switchTime, if switchTime == 0.f then switch immediately
		 */
		void toNextScene(float32 switchTime);
		void switchToScene(lScene* scene, float32 switchTime);
				
		bool isSwitching();
		
		void onSceneTimerEvent(const lSceneTimerEvent* tev);
		
		lScene* getCurrScene();
		
		lScene* getSceneByName(const SoraString& name);
		
	private:		
		sora::SoraCore* sora;
		sora::SoraSprite* pSprBlackCover;
		
		typedef std::list<lScene*> SCENE_LIST;
		SCENE_LIST scenes;
		SCENE_LIST::iterator currScene;
		SCENE_LIST::iterator nextScene;
		
		inline SCENE_LIST::iterator getSceneIterator(lScene* scene);
		inline void switchToScene();
		
		lSceneTimerEvent evToStart;
		lSceneTimerEvent evToHalf;
		lSceneTimerEvent evToFinish;
		lSceneTimerEvent evFromStart;
		lSceneTimerEvent evFromHalf;
		lSceneTimerEvent evFromFinish;
		
		bool bSwitching;
		bool bSwitchHalf;
		float32 switchTime;
		
		sora::CoreTrigTransformer<sora::CoreTransform> trigTransformer;
		
		hgeRect sceneRect;
		
		bool bEffectEnabled;
	};
	
} // namespace projl 


#endif