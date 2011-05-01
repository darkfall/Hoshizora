/*
 *  lAnm.h
 *  lpack
 *
 *  Created by GriffinBu on 12/9/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#ifndef SORA_SPRITE_ANIMATION_H_
#define SORA_SPRITE_ANIMATION_H_

#define LANM_HEADER "LANM" // 4

#include <vector>
#include <string>
#include <stack>
#include "SoraSprite.h"
#include "SoraPlatform.h"

namespace sora {

struct LANM_HEAD {
	uint8 anmCount; // 1
};

struct LANM_TEX {
	uint32 tx;
	uint32 ty;
	uint32 tw;
	uint32 th;
	
	LANM_TEX(): tx(0), ty(0), tw(0), th(0) {}
	LANM_TEX(uint32 x, uint32 y, uint32 w, uint32 h): tx(x), ty(y), tw(w), th(h) {}
};

struct LANM_NODE {
	uint8 anmId;	
	std::string name;
	uint32 nameHash;
	uint32 anmFrame;
	
	typedef std::vector<LANM_TEX> ANM_TEX_LIST;
	ANM_TEX_LIST texList;
};

class SoraSpriteAnimation;

class SoraSpriteAnimationPacker {
public:
	static int pack(const char* pstrScript, const char* pstrDest);
	int pack2(const char* pstrTex, const char* dest);
	
	static SoraSpriteAnimation* unpack(const char* pstrlanm);
	static SoraSpriteAnimation* unpack(void* pData, unsigned long size);
	
	static int unpackToFile(const char* pstrlanm);
	
	void setDefaultAnm(const char* name);
	uint32 addNode(const char* name, uint32 frame);
	void addNodeTex(uint32 nodeId, uint32 tx, uint32 ty, uint32 tw, uint32 th);
	
	void clear();
	
private:
	typedef std::vector<LANM_NODE> ANM_NODE_LIST;
	ANM_NODE_LIST anmNodes;
	
	std::string defaultName;
};
	
#define PACK_ANIMATION(script, dest) SoraSpriteAnimationPacker::pack(script, dest)
#define UNPACK_ANIMATION(script) SoraSpriteAnimationPacker::unpack(script)
#define UNPACK_ANIMATION_IN_MEM(data, size) SoraSpriteAnimationPacker::unpack(data, size)
	
	typedef enum {
		ANCHOR_UPPER_LEFT=1,
		ANCHOR_UPPER_RIGHT=2,
		ANCHOR_LOWER_RIGHT=3,
		ANCHOR_LOWER_LEFT=4,
		ANCHOR_MIDDLE=5,
	} ANIMATION_SPRITE_ANCHOR;
	
class SoraSpriteAnimation: public SoraObject {
	friend class SoraSpriteAnimationPacker;
	
public:
	SoraSpriteAnimation(const std::wstring& anmPath);
	~SoraSpriteAnimation();
	
	typedef LANM_TEX SoraAnimationRect;

	uint32 update(float dt);
    void render();
	SoraAnimationRect getCurrTex();
	
	/*
	 plays the default anm
	 */
	void play();
	/*
	 plays a anime
	 @param name, name of the anime
	 @param bLoop, is loop
	 @param bQueue, is put the anime in playing queue, if false, then the anime would start playing immediately
	 */
	void playEx(const char* name, bool bLoop=true, bool bQueue=false);
	
	void pause();
	void resume();
	
	void setDefaultAnimation(const char* name);
	
	uint32 getAnimationSize() const;
	uint32 getCurrAnimationIndex() const;
	
	bool isPlaying() const;
	
	std::string getAnimationName(size_t id) const;
	std::string getCurrentAnimationName() const;
	
	SoraSprite* getSprite() const;
	
	void setAnchor(int32 anchor);
	ANIMATION_SPRITE_ANCHOR getAnchor() const;
		
private:
    void init();
	inline void toNextAnm();
	inline void toCurrAnmNode();
	
	SoraSpriteAnimation();
	
	SoraSpriteAnimation(const SoraSpriteAnimation&);
	SoraSpriteAnimation& operator=(const SoraSpriteAnimation&);
	
	typedef std::vector<LANM_NODE> ANM_NODE_LIST;
	ANM_NODE_LIST anmNodes;
	
	inline int getNodeIdByAnmId(uint32 anmId);
	
	uint32 anmCount;
	uint32 defaultNameHash;
	std::string defaultName;
	
	uint32 currId;
	uint32 currAnmIndex;
	uint32 currTime;
	uint32 totalTime;
	uint32 distTime;
	
	uint32 defaultId;
	uint32 currNodeId;
	
	bool bIsPlaying;
	bool bPaused;
	bool bLoop;
    
    std::string texturePath;
	SoraSprite* pAnmSprite;
    
	typedef std::stack<uint8> ANM_QUEUE;
	ANM_QUEUE anmQueue;
	
	ANIMATION_SPRITE_ANCHOR sprAnchor;
};
    
} // namespace sora

#endif // SORA_SPRITE_ANIMATION_H_