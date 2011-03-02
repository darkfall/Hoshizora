/*
 *  lAnm.h
 *  lpack
 *
 *  Created by GriffinBu on 12/9/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#ifndef _LANM_H_
#define _LANM_H_

#define LANM_HEADER "LANM" // 4

#include <vector>
#include <string>
#include <stack>

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

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
	uint32 nameHash;
	uint32 anmFrame;
	
	typedef std::vector<LANM_TEX> ANM_TEX_LIST;
	ANM_TEX_LIST texList;
};

class lAnm;

class lAnmPacker {
public:
	int pack(const char* pstrScript, const char* pstrDest);
	int pack2(const char* pstrTex, const char* dest);
	
	static lAnm* unpack(const char* pstrlanm);
	static lAnm* unpack(void* pData, unsigned long size);
	
	static int unpackToFile(const char* pstrlanm, const char* pstrTexDest);
	
	void setDefaultAnm(const char* name);
	uint32 addNode(const char* name, uint32 frame);
	void addNodeTex(uint32 nodeId, uint32 tx, uint32 ty, uint32 tw, uint32 th);
	
	void clear();
	
private:
	typedef std::vector<LANM_NODE> ANM_NODE_LIST;
	ANM_NODE_LIST anmNodes;
	
	std::string defaultName;
};

class lAnm {
	friend class lAnmPacker;
	
public:
	~lAnm();

	void update(float dt);
	LANM_TEX getCurrTex();
	
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
	void play(const char* name, bool bLoop=true, bool bQueue=false);
	
	void pause();
	void resume();
	
	void setDefaultAnm(const char* name);
	
	uint8 getAnmCount();
	uint8 getCurrAnmIndex();
	
	bool isPlaying();

	void* getTexData() const;
	uint32 getTexSize() const;
	
	void freeTexData();

private:
	inline void toNextAnm();
	inline void toCurrAnmNode();
	
	lAnm();
	
	lAnm(const lAnm&);
	lAnm& operator=(const lAnm&);
	
	typedef std::vector<LANM_NODE> ANM_NODE_LIST;
	ANM_NODE_LIST anmNodes;
	
	inline int getNodeIdByAnmId(uint32 anmId);
	
	uint8 anmCount;
	uint32 defaultNameHash;
	
	void* pTexData;
	uint32 texSize;
	
	uint32 currId;
	uint8 currAnmIndex;
	uint8 currTime;
	uint8 totalTime;
	uint8 distTime;
	
	int defaultId;
	int currNodeId;
	
	bool bIsPlaying;
	bool bPaused;
	bool bLoop;
	
	typedef std::stack<uint8> ANM_QUEUE;
	ANM_QUEUE anmQueue;
};

#endif // _LANM_H_