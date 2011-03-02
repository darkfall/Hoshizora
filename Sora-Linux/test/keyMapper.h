/*
 *  keyMapper.h
 *  Sora
 *
 *  Created by GriffinBu on 1/5/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef KEY_MAPPER_H_
#define KEY_MAPPER_H_

#include "SoraKeyInfo.h"
#include "SoraSingleton.h"

#include <map>

class keyMapper: public sora::SoraSingleton<keyMapper> {
	friend class sora::SoraSingleton<keyMapper>;
protected:
	keyMapper() {
		sora = sora::SoraCore::Instance();
	}
	
public:
	
	void setKey(int32 key, int32 sorakey) {
		keyMap[key] = sorakey;
	}
	
	bool keyDown(int32 key) {
		return sora->keyDown(keyMap[key]);
	}
	
private:
	typedef std::map<int32, int32> KEY_MAP;
	KEY_MAP keyMap;
	
	sora::SoraCore* sora;
};

#endif