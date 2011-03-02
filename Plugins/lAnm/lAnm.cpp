/*
 *  lAnm.cpp
 *  lpack
 *
 *  Created by GriffinBu on 12/9/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#include "lAnm.h"

#include "llexer/llexer.h"
#include "lMemFile.h"
#include "hash.h"

#include <cassert>

inline uint32 nextAnmToken(llexer* lexer, bool& err) {
	Token token = lexer->getNextToken();
	if(token != TOKEN_TYPE_INT) {
		err = true;
		return 0;
	}
	uint32 i = atoi(lexer->getCurrLexeme());
	return i;
}

inline const char* nextAnmNameToken(llexer* lexer, bool& err) {
	Token token = lexer->getNextToken();
	if(token != TOKEN_TYPE_IDENT) {
		err = true;
		return "\0";
	}
	return lexer->getCurrLexeme();
}

template<typename T>
inline T nextAnmT(FILE* file, bool& err) {
	T i;
	fread(&i, sizeof(T), 1, file);
	if(ferror(file))
		err = true;
	return i;
}

template<typename T>
inline bool writeAnmT(FILE* file, T i) {
	fwrite(&i, sizeof(T), 1, file);
	if(ferror(file))
		return true;
	return false;
}

int lAnmPacker::pack(const char* pstrScript, const char* pstrDest) {
	llexer* lexer = new llexer;
	if(lexer->loadFile(pstrScript) != LEX_NO_ERROR) 
		return 0;
	
	FILE* pf = fopen(pstrDest, "wb");
	if(!pf) 
		return 0;
	
	bool err = false;
	Token token = lexer->getNextToken();
	if(token != TOKEN_TYPE_STRING)
		err = true;
	
	FILE* ptex = fopen(lexer->getCurrLexeme(), "rb");
	if(!ptex) {
		fclose(pf);
		return 0;
	}
	
	token = lexer->getNextToken();
	if(token != TOKEN_TYPE_INT)
		err = true;
	
	fwrite(LANM_HEADER, 4, 1, pf);
	if(ferror(pf))
		err = true;
	
	if(!err) {
		uint8 anmSize = static_cast<uint8> (atoi(lexer->getCurrLexeme()));
		err = writeAnmT(pf, anmSize);
		
		uint32 defaultNameHash = sora::BKDRHash(nextAnmNameToken(lexer, err));
		err = writeAnmT(pf, defaultNameHash);
		
#ifdef _DEBUG
		printf("size: %d, defaultNameHash: %d\n", anmSize, defaultNameHash);
#endif
		
		for(uint8 i=0; i<anmSize; ++i) {
			uint32 nameHash = sora::BKDRHash(nextAnmNameToken(lexer, err));
			uint32 frameCount = nextAnmToken(lexer, err);
			uint32 anmCount = nextAnmToken(lexer, err);
			err = writeAnmT(pf, nameHash);
			err = writeAnmT(pf, frameCount);
			err = writeAnmT(pf, anmCount);
			assert(anmCount != 0);
			
#ifdef _DEBUG
			printf("nameHash: %d, frameCount: %d, anmCount: %d\n", nameHash, frameCount, anmCount);
#endif
			
			if(!err) {
				for(uint8 j=0; j<anmCount; ++j) {
					uint32 tx = nextAnmToken(lexer, err);	err = writeAnmT(pf, tx);
					uint32 ty = nextAnmToken(lexer, err);	err = writeAnmT(pf, ty);
					uint32 tw = nextAnmToken(lexer, err);	err = writeAnmT(pf, tw);
					uint32 th = nextAnmToken(lexer, err);	err = writeAnmT(pf, th);
				}
			}
			
			if(err) {
				break;
			}
		}
	}
	
	uint32 texSize = 0;
	void *pData = 0;
	if(!err) {
		fseek(ptex, 0, SEEK_END);
		texSize = ftell(ptex);
		fseek(ptex, 0, SEEK_SET);
		
		pData = malloc(texSize);
		if(!pData || ferror(ptex))
			err = true;
	}
	
	if(!err) {
		fread(pData, 1, texSize, ptex);
		if(ferror(ptex)) {
			err = true;
		}
	}
	
	if(!err) {
		fwrite(&texSize, 4, 1, pf);
		fwrite(pData, 1, texSize, pf);
		
		if(ferror(pf)) 
			err = true;
	}
	
	fclose(ptex);
	fclose(pf);
	delete lexer;
	if(pData)
		free(pData);
	
	if(err) 
		return 0;
	return 1;
}

int lAnmPacker::pack2(const char* pstrTex, const char* pstrDest) {
	FILE* pf = fopen(pstrDest, "wb");
	if(!pf) 
		return 0;
	
	FILE* ptex = fopen(pstrTex, "rb");
	if(!ptex) {
		fclose(pf);
		return 0;
	}
	
	bool err = false;
	
	fwrite(LANM_HEADER, 4, 1, pf);
	uint8 cSize = static_cast<uint8> (anmNodes.size());
	err = writeAnmT(pf, cSize);
	err = writeAnmT(pf, sora::BKDRHash(defaultName.c_str()));
	
	if(!err) {
		ANM_NODE_LIST::iterator it = anmNodes.begin();
		while(it != anmNodes.end()) {
			err = writeAnmT(pf, it->nameHash);
			err = writeAnmT(pf, it->anmFrame);
			err = writeAnmT(pf, it->texList.size());
		
			LANM_NODE::ANM_TEX_LIST::iterator texit = it->texList.begin();
			while(texit != it->texList.end()) {
				err = writeAnmT(pf, texit->tx);
				err = writeAnmT(pf, texit->ty);
				err = writeAnmT(pf, texit->tw);
				err = writeAnmT(pf, texit->th);
				
				++texit;
			}
			if(ferror(pf)) {
				err = true;
			}
			++it;
		}
	}
	
	uint32 texSize = 0;
	void* pData = 0;
	
	if(!err) {
		fseek(ptex, 0, SEEK_END);
		texSize = ftell(ptex);
		fseek(ptex, 0, SEEK_SET);
		
		pData = malloc(texSize);
		if(!pData || ferror(ptex)) {
			err = true;
		}
	}
	
	if(!err) {
		fread(pData, 1, texSize, ptex);
		if(ferror(ptex)) {
			err = true;
		}
	}
	
	if(!err) {
		fwrite(&texSize, 4, 1, pf);
		fwrite(pData, 1, texSize, pf);
		if(ferror(pf)) {
			err = true;
		}
	}
	
	fclose(ptex);
	fclose(pf);
	if(pData)
		free(pData);
	
	if(err)
		return 0;
	return 1;
}

lAnm* lAnmPacker::unpack(void* pData, unsigned long size) {
	lAnm* panm = new lAnm;
	
	bool err = false;
	if(panm) {
		lMemFile* pmfile = new lMemFile(pData, size);
		
		char strHeader[5];
		pmfile->read(strHeader, 4);
		strHeader[4] = '\0';

		if(strcmp(strHeader, LANM_HEADER) != 0)
			err = true;
		
		if(!err) {
			err = !pmfile->read(&panm->anmCount); 
		}
		if(!err) {
			err = !pmfile->read(&panm->defaultNameHash);
			panm->defaultId = panm->defaultNameHash;
		}
		
#ifdef _DEBUG
		printf("anmCount: %d, dnameHash: %d\n", panm->anmCount, panm->defaultNameHash);
#endif
		
		if(!err) {
			for(uint8 i=0; i<panm->anmCount; ++i) {
				LANM_NODE node;
				err = !pmfile->read(&node.nameHash);
				err = !pmfile->read(&node.anmFrame);
				
				uint32 anmSize;
				err = !pmfile->read(&anmSize);
#ifdef _DEBUG
				printf("nameHash: %d, anmFrame: %d, anmCount: %d\n", node.nameHash, node.anmFrame, anmSize);
#endif
				for(uint8 j=0; j<anmSize; ++j) {
					LANM_TEX tex;
					err = !pmfile->read(&tex.tx);
					err = !pmfile->read(&tex.ty);
					err = !pmfile->read(&tex.tw);
					err = !pmfile->read(&tex.th);
					
					node.texList.push_back(tex);
				}
				
				panm->anmNodes.push_back(node);
				if(err)
					break;
			}
		}
		
		void* pTexData = 0;
		if(!err) {
			err = !pmfile->read(&panm->texSize);
			pTexData = malloc(panm->texSize);
			if(pTexData) {
				err = !pmfile->read(pTexData, panm->texSize);
			}
			if(err)
				free(pTexData);
		}
		if(!err) {
			panm->pTexData = pTexData;
		}
		
		pmfile->seek(0);
		delete pmfile;
	}
	
	if(err) {
		delete panm;
		return 0;
	}
	
	panm->play();
	return panm;
}

lAnm* lAnmPacker::unpack(const char* pstrlanm) {
	FILE* pf = fopen(pstrlanm, "rb");
	if(!pf) 
		return 0;
	
	unsigned long size = 0;
	fseek(pf, 0, SEEK_END);
	size = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	void* pData = malloc(size);
	fread(pData, 1, size, pf);
	lAnm* anm = unpack(pData, size);
	free(pData);
	return anm;
}


int lAnmPacker::unpackToFile(const char* pstrlanm, const char* pstrTexDest) {
	FILE* pf = fopen(pstrlanm, "rb");
	if(!pf) 
		return 0;
	
	std::string strDest = pstrlanm;
	strDest += ".txt";
	FILE* pd = fopen(strDest.c_str(), "w");
	if(!pd) {
		fclose(pf);
		return 0;
	}
	
	bool err = false;
	
	char strHeader[5];
	fread(strHeader, 4, 1, pf);
	strHeader[4] = '\0';
	if(strcmp(strHeader, LANM_HEADER) != 0) {
		fclose(pf);
		fclose(pd);
		return 0;
	}
	
	uint8 anmSize = nextAnmT<uint8> (pf, err);
	fprintf(pd, "%d ", anmSize);
	fprintf(pd, "%d", nextAnmT<uint32> (pf, err));
	fprintf(pd, "\n");
	if(!err) {
		for(uint8 i=0; i<anmSize; ++i) {
			uint32 nameHash = nextAnmT<uint32> (pf, err);
			uint32 frameCount = nextAnmT<uint32> (pf, err);
			uint8 anmCount = nextAnmT<uint8> (pf, err);
			
			fprintf(pd, "%d ", nameHash);
			fprintf(pd, "%d ", frameCount);
			fprintf(pd, "%d", anmCount);
			fprintf(pd, "\n");
			
			for(uint8 j=0; j<anmCount; ++j) {
				fprintf(pd, "\t");
				uint32 tx = nextAnmT<uint32> (pf, err);	fprintf(pd, "%d ", tx);
				uint32 ty = nextAnmT<uint32> (pf, err); fprintf(pd, "%d ", ty);
				uint32 tw = nextAnmT<uint32> (pf, err); fprintf(pd, "%d ", tw);
				uint32 th = nextAnmT<uint32> (pf, err);	fprintf(pd, "%d",  th);
				fprintf(pd, "\n");
			}
			
			if(err)
				break;
		}
	}
	
	uint32 texSize = nextAnmT<uint32> (pf, err);
	void* pTex = 0;
	if(!err) {
		pTex = malloc(texSize);
		fread(pTex, 1, texSize, pf);
	}
	
	if(!err) {
		FILE* pt = fopen(pstrTexDest, "wb");
		if(!pt)
			err = true;
		
		fwrite(pTex, 1, texSize, pt);
		fclose(pt);
	}
	
	fclose(pf);
	fclose(pd);
	if(pTex)
		free(pTex);
	
	if(err)
		return 0;
	return 1;
}

void lAnmPacker::clear() {
	anmNodes.clear();
}

uint32 lAnmPacker::addNode(const char* anmName, uint32 frame) {
	LANM_NODE node;
	node.nameHash = sora::BKDRHash(anmName);
	node.anmFrame = frame;
	
	int size = anmNodes.size();
	anmNodes.push_back(node);
	return size;
}

void lAnmPacker::addNodeTex(uint32 nodeId, uint32 tx, uint32 ty, uint32 tw, uint32 th) {
	if(nodeId >= 0 && nodeId < anmNodes.size()) {
		LANM_TEX tex(tx, ty, tw, th);
		anmNodes[nodeId].texList.push_back(tex);
	}
}

lAnm::lAnm():
	pTexData(0), anmCount(0), currId(0), currAnmIndex(0), 
	bPaused(false), bIsPlaying(false), bLoop(true), 
	defaultId(0), currTime(0), totalTime(0), currNodeId(0)  {
}

lAnm::~lAnm() {
	//if(pTexData)
	//	free(pTexData);
}

int lAnm::getNodeIdByAnmId(uint32 anmId) {
	for(size_t i=0; i<anmNodes.size(); ++i) {
		if(anmNodes[i].nameHash == anmId) {
			return i;
		}
	}
	return -1;
}

void lAnm::update(float dt) {
	if(!bIsPlaying) return;
	
	if(!bPaused && totalTime != 0) {
		if(currTime >= distTime) {
			++currAnmIndex;
			if(currAnmIndex >= anmNodes[currNodeId].texList.size())
				toNextAnm();
			currTime = 0;
		}
		currTime++;
	}
}

void lAnm::toNextAnm() {
	currAnmIndex = 0;
	if(!bLoop) {
		if(!anmQueue.empty()) {
			currId = anmQueue.top();
			toCurrAnmNode();
		}
		else if(currId != defaultId && defaultId != -1) {
			currId = defaultId;
			toCurrAnmNode();
			bLoop = true;
		}
	}
}

void lAnm::toCurrAnmNode() {
	currNodeId = getNodeIdByAnmId(currId);
	totalTime = anmNodes[currNodeId].anmFrame;
	distTime = totalTime / anmNodes[currNodeId].texList.size();
}

void lAnm::setDefaultAnm(const char* name) {
	uint32 hash = sora::BKDRHash(name);
	for(size_t i=0; i<anmNodes.size(); ++i) {
		if(anmNodes[i].nameHash == hash) {
			defaultId = hash;
			break;
		}
	}
	defaultId = -1;
}

LANM_TEX lAnm::getCurrTex() {
	if(currNodeId != -1)
		return anmNodes[currNodeId].texList[currAnmIndex];
	return LANM_TEX();
}

void lAnm::play() {
	if(defaultId == -1)
		return;
	
	bPaused = false;
	bIsPlaying = true;
	currTime = 0;
	currId = defaultId;
	toCurrAnmNode();
}

void lAnm::play(const char* name, bool loop, bool isQueue) {
	if(!isQueue) {
		int id = getNodeIdByAnmId(sora::BKDRHash(name));
		if(id == -1)
			return;
	
		currNodeId = id;
		currId = anmNodes[currNodeId].nameHash; 
		currTime = 0;
		totalTime = anmNodes[currNodeId].anmFrame;
		distTime = totalTime / anmNodes[currNodeId].texList.size();
		bLoop = loop;
		bIsPlaying = true;
	} else {
		anmQueue.push(sora::BKDRHash(name));
	}
}

void lAnm::pause() {
	bPaused = true;
	bIsPlaying = false;
}

void lAnm::resume() {
	bPaused = false;
	bIsPlaying = true;
}

bool lAnm::isPlaying() {
	return bIsPlaying;
}

uint8 lAnm::getAnmCount() {
	return anmCount;
}

uint8 lAnm::getCurrAnmIndex() {
	return currAnmIndex;
}

void* lAnm::getTexData() const {
	return pTexData;
}

uint32 lAnm::getTexSize() const {
	return texSize;
}

void lAnm::freeTexData() {
	if(pTexData) {
		free(pTexData);
		pTexData = 0;
	}
}


