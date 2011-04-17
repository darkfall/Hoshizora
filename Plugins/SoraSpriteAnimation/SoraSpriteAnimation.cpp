/*
 *  SoraSpriteAnimation.cpp
 *  lpack
 *
 *  Created by GriffinBu on 12/9/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#include "SoraSpriteAnimation.h"

#include "llexer/llexer.h"
#include "lMemFile.h"
#include "hash.h"
#include "common.h"

#include "SoraCore.h"

#include <cassert>

namespace sora {


inline uint32 cFileReadToken(llexer* lexer, bool& err) {
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

int SoraSpriteAnimationPacker::pack(const char* pstrScript, const char* pstrDest) {
	llexer* lexer = new llexer;
	if(lexer->loadFile(pstrScript) != LEX_NO_ERROR) 
		return 0;
	
	bool err = false;

	FILE* pf = fopen(pstrDest, "wb");
	if(!pf) 
		return 0;
	fwrite(LANM_HEADER, 4, 1, pf);
	if(ferror(pf))
		err = true;
	
	Token token = lexer->getNextToken();
	if(token != TOKEN_TYPE_STRING)
		err = true;
	
    std::string texturePath = lexer->getCurrLexeme();
	if(!err) {
        size_t size = texturePath.size();
		fwrite(&size, 4, 1, pf);
		fwrite(texturePath.c_str(), texturePath.size(), 1, pf);
		
		if(ferror(pf)) 
			err = true;
	}
	
	token = lexer->getNextToken();
	if(token != TOKEN_TYPE_INT)
		err = true;
	
	if(!err) {
		uint32 anmSize = (atoi(lexer->getCurrLexeme()));
		err = cFileWriteT(pf, anmSize);
		
		std::string defaultName = nextAnmNameToken(lexer, err);
		uint32 nameSize = defaultName.size();
		err = cFileWriteT(pf, nameSize);
		fwrite(defaultName.c_str(), nameSize, 1, pf);
		uint32 defaultNameHash = sora::BKDRHash(defaultName.c_str());
		err = cFileWriteT(pf, defaultNameHash);
		
#ifdef _DEBUG
		printf("size: %d, defaultNameHash: %d\n", anmSize, defaultNameHash);
#endif
		
		for(uint8 i=0; i<anmSize; ++i) {
			std::string name = nextAnmNameToken(lexer, err);
			uint32 nameSize = name.size();
			
			err = cFileWriteT(pf, nameSize);
			fwrite(name.c_str(), nameSize, 1, pf);
			uint32 nameHash = sora::BKDRHash(name.c_str());
			cFileWriteT(pf, nameHash);
			
			uint32 frameCount = cFileReadToken(lexer, err);
			uint32 anmCount = cFileReadToken(lexer, err);
			
			err = cFileWriteT(pf, frameCount);
			err = cFileWriteT(pf, anmCount);
			assert(anmCount != 0);
			
#ifdef _DEBUG
			printf("nameHash: %d, frameCount: %d, anmCount: %d\n", nameHash, frameCount, anmCount);
#endif
			
			if(!err) {
				for(uint8 j=0; j<anmCount; ++j) {
					uint32 tx = cFileReadToken(lexer, err);	err = cFileWriteT(pf, tx);
					uint32 ty = cFileReadToken(lexer, err);	err = cFileWriteT(pf, ty);
					uint32 tw = cFileReadToken(lexer, err);	err = cFileWriteT(pf, tw);
					uint32 th = cFileReadToken(lexer, err);	err = cFileWriteT(pf, th);
				}
			}
			
			if(err) {
				break;
			}
		}
	}
	
	fclose(pf);
	delete lexer;
	
	if(err) 
		return 0;
	return 1;
}

int SoraSpriteAnimationPacker::pack2(const char* pstrTex, const char* pstrDest) {
	FILE* pf = fopen(pstrDest, "wb");
	if(!pf) 
		return 0;

	bool err = false;
	
	fwrite(LANM_HEADER, 4, 1, pf);
	if(!err) {
        uint32 size = strlen(pstrTex);
		fwrite(&size, 4, 1, pf);
		fwrite(pstrTex, strlen(pstrTex), 1, pf);
		if(ferror(pf)) {
			err = true;
		}
	}
	
	uint32 cSize(anmNodes.size());
	err = cFileWriteT(pf, cSize);
	
	uint32 nameSize = defaultName.size();
	err = cFileWriteT(pf, nameSize);
	fwrite(defaultName.c_str(), nameSize, 1, pf);
	err = cFileWriteT(pf, sora::BKDRHash(defaultName.c_str()));
	
	if(!err) {
		ANM_NODE_LIST::iterator it = anmNodes.begin();
		while(it != anmNodes.end()) {
			uint32 nameSize = it->name.size();
			err = cFileWriteT(pf, nameSize);
			fwrite(it->name.c_str(), nameSize, 1, pf);
			uint32 nameHash = sora::BKDRHash(it->name.c_str());
			cFileWriteT(pf, nameHash);
			
			err = cFileWriteT(pf, it->anmFrame);
			err = cFileWriteT(pf, it->texList.size());
		
			LANM_NODE::ANM_TEX_LIST::iterator texit = it->texList.begin();
			while(texit != it->texList.end()) {
				err = cFileWriteT(pf, texit->tx);
				err = cFileWriteT(pf, texit->ty);
				err = cFileWriteT(pf, texit->tw);
				err = cFileWriteT(pf, texit->th);
				
				++texit;
			}
			if(ferror(pf)) {
				err = true;
			}
			++it;
		}
	}
	
	
	fclose(pf);
	
	if(err)
		return 0;
	return 1;
}

SoraSpriteAnimation* SoraSpriteAnimationPacker::unpack(void* pData, unsigned long size) {
	SoraSpriteAnimation* panm = new SoraSpriteAnimation;
	
	bool err = false;
	if(panm) {
		lMemFile* pmfile = new lMemFile(pData, size);
		
		char strHeader[5];
		pmfile->read(strHeader, 4);
		strHeader[4] = '\0';

		if(strcmp(strHeader, LANM_HEADER) != 0)
			err = true;
		
        if(!err) {
            uint32 texSize;
			err = !pmfile->read(&texSize);
            char texturePath[texSize+1];
			err = !pmfile->read(texturePath, texSize);
			texturePath[texSize] = '\0';
			panm->texturePath = texturePath;
		}
		
		if(!err) {
			err = !pmfile->read(&panm->anmCount); 
		}
		if(!err) {
			uint32 size;
			err = !pmfile->read(&size);
			char name[size+1];
			err = !pmfile->read(name, size);
			name[size] = '\0';
			panm->defaultName = name;
			
			err = !pmfile->read(&panm->defaultNameHash);
			panm->defaultId = panm->defaultNameHash;
		}
		
#ifdef _DEBUG
		printf("anmCount: %d, dnameHash: %d, name: %s\n", panm->anmCount, panm->defaultNameHash, panm->defaultName.c_str());
#endif
		
		if(!err) {
			for(uint8 i=0; i<panm->anmCount; ++i) {
				LANM_NODE node;
				uint32 size;
				err = !pmfile->read(&size);
				char name[size+1];
				err = !pmfile->read(name, size);
				name[size] = '\0';
				node.name = name;
				
				err = !pmfile->read(&node.nameHash);
				err = !pmfile->read(&node.anmFrame);
				
				uint32 anmSize;
				err = !pmfile->read(&anmSize);
#ifdef _DEBUG
				printf("nameHash: %d, anmFrame: %d, anmCount: %d, name: %s\n", node.nameHash, node.anmFrame, anmSize, node.name.c_str());
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

		pmfile->seek(0);
		delete pmfile;
	}
	
	if(err) {
		delete panm;
		return 0;
	}
	
	panm->init();
	return panm;
}

SoraSpriteAnimation* SoraSpriteAnimationPacker::unpack(const char* pstrlanm) {
	FILE* pf = fopen(pstrlanm, "rb");
	if(!pf) 
		return 0;
	
	unsigned long size = 0;
	fseek(pf, 0, SEEK_END);
	size = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	void* pData = malloc(size);
	fread(pData, 1, size, pf);
	SoraSpriteAnimation* anm = unpack(pData, size);
	free(pData);
	return anm;
}


int SoraSpriteAnimationPacker::unpackToFile(const char* pstrlanm) {
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
	
	uint32 texSize = cFileReadT<uint32> (pf, err);
	if(!err) {
		char tex[texSize+1];
		fread(tex, texSize, 1, pf);
		tex[texSize] = '\0';
		fprintf(pd, "%s\n", tex);
	}
	
	
	uint8 anmSize = cFileReadT<uint8> (pf, err);
	fprintf(pd, "%d ", anmSize);
	fprintf(pd, "%d", cFileReadT<uint32> (pf, err));
	fprintf(pd, "\n");
	if(!err) {
		for(uint8 i=0; i<anmSize; ++i) {
			uint32 nameHash = cFileReadT<uint32> (pf, err);
			uint32 frameCount = cFileReadT<uint32> (pf, err);
			uint8 anmCount = cFileReadT<uint8> (pf, err);
			
			fprintf(pd, "%d ", nameHash);
			fprintf(pd, "%d ", frameCount);
			fprintf(pd, "%d", anmCount);
			fprintf(pd, "\n");
			
			for(uint8 j=0; j<anmCount; ++j) {
				fprintf(pd, "\t");
				uint32 tx = cFileReadT<uint32> (pf, err);	fprintf(pd, "%d ", tx);
				uint32 ty = cFileReadT<uint32> (pf, err);   fprintf(pd, "%d ", ty);
				uint32 tw = cFileReadT<uint32> (pf, err);   fprintf(pd, "%d ", tw);
				uint32 th = cFileReadT<uint32> (pf, err);	fprintf(pd, "%d",  th);
				fprintf(pd, "\n");
			}
			
			if(err)
				break;
		}
	}
	
	
	fclose(pf);
	fclose(pd);
	
	if(err)
		return 0;
	return 1;
}

void SoraSpriteAnimationPacker::clear() {
	anmNodes.clear();
}

uint32 SoraSpriteAnimationPacker::addNode(const char* anmName, uint32 frame) {
	LANM_NODE node;
	node.nameHash = BKDRHash(anmName);
	node.name = anmName;
	node.anmFrame = frame;
	
	int size = anmNodes.size();
	anmNodes.push_back(node);
	return size;
}

void SoraSpriteAnimationPacker::addNodeTex(uint32 nodeId, uint32 tx, uint32 ty, uint32 tw, uint32 th) {
	if(nodeId >= 0 && nodeId < anmNodes.size()) {
		LANM_TEX tex(tx, ty, tw, th);
		anmNodes[nodeId].texList.push_back(tex);
	}
}

SoraSpriteAnimation::SoraSpriteAnimation():
	anmCount(0), currId(0), currAnmIndex(0), 
	bPaused(false), bIsPlaying(false), bLoop(true), 
	defaultId(0), currTime(0), totalTime(0), currNodeId(0), pAnmSprite(NULL)  {
}

SoraSpriteAnimation::~SoraSpriteAnimation() {
	if(pAnmSprite)
		delete pAnmSprite;
}

int SoraSpriteAnimation::getNodeIdByAnmId(uint32 anmId) {
	for(size_t i=0; i<anmNodes.size(); ++i) {
		if(anmNodes[i].nameHash == anmId) {
			return i;
		}
	}
	return -1;
}

uint32 SoraSpriteAnimation::update(float dt) {
	if(!bIsPlaying) return 0;
	
	if(!bPaused && totalTime != 0) {
		if(currTime >= distTime) {
			++currAnmIndex;
			if(currAnmIndex >= anmNodes[currNodeId].texList.size())
				toNextAnm();
			currTime = 0;
		}
		currTime++;
	}
    return 1;
}

void SoraSpriteAnimation::toNextAnm() {
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

void SoraSpriteAnimation::toCurrAnmNode() {
	currNodeId = getNodeIdByAnmId(currId);
	totalTime = anmNodes[currNodeId].anmFrame;
	distTime = totalTime / anmNodes[currNodeId].texList.size();
}

void SoraSpriteAnimation::setDefaultAnimation(const char* name) {
	uint32 hash = BKDRHash(name);
	for(size_t i=0; i<anmNodes.size(); ++i) {
		if(anmNodes[i].nameHash == hash) {
			defaultId = hash;
			break;
		}
	}
	defaultId = -1;
}

LANM_TEX SoraSpriteAnimation::getCurrTex() {
	if(currNodeId != -1)
		return anmNodes[currNodeId].texList[currAnmIndex];
	return LANM_TEX();
}

void SoraSpriteAnimation::play() {
	if(defaultId == -1)
		return;
	
	bPaused = false;
	bIsPlaying = true;
	currTime = 0;
	currId = defaultId;
	toCurrAnmNode();
}

void SoraSpriteAnimation::play(const char* name, bool loop, bool isQueue) {
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

void SoraSpriteAnimation::pause() {
	bPaused = true;
	bIsPlaying = false;
}

void SoraSpriteAnimation::resume() {
	bPaused = false;
	bIsPlaying = true;
}

bool SoraSpriteAnimation::isPlaying() const {
	return bIsPlaying;
}

uint32 SoraSpriteAnimation::getAnimationSize() const {
	return anmCount;
}

uint32 SoraSpriteAnimation::getCurrAnimationIndex() const {
	return currAnmIndex;
}

void SoraSpriteAnimation::init() {
    assert(texturePath.size() != 0);
    pAnmSprite = SORA->createSprite(s2ws(texturePath));
	if(anmCount != 0 && anmNodes[0].texList.size() != 0)
		pAnmSprite->setTextureRect(anmNodes[0].texList[0].tx, anmNodes[0].texList[0].ty, anmNodes[0].texList[0].tw, anmNodes[0].texList[0].th);
    if(pAnmSprite == NULL)
        throw SORA_EXCEPTION("error loading animation sprite");
}

void SoraSpriteAnimation::render() {
    if(pAnmSprite) {
        LANM_TEX texRect = getCurrTex();
        pAnmSprite->setTextureRect(texRect.tx, texRect.ty, texRect.tw, texRect.th);
        pAnmSprite->render(getPositionX(), getPositionY());
    }
}
	
	std::string SoraSpriteAnimation::getAnimationName(size_t id) const {
		return anmNodes[id].name;
	}
	
	std::string SoraSpriteAnimation::getCurrentAnimationName() const {
		if(currNodeId != -1) 
			return anmNodes[currNodeId].name;
		return "\0";
	}

    SoraSprite* SoraSpriteAnimation::getSprite() const {
		return pAnmSprite;
	}
	
	void SoraSpriteAnimation::setAnchor(ANIMATION_SPRITE_ANCHOR anchor) {
		switch (anchor) {
			case ANCHOR_UPPER_LEFT: if(pAnmSprite) pAnmSprite->setCenter(0.f, 0.f); break;
			case ANCHOR_UPPER_RIGHT: if(pAnmSprite) pAnmSprite->setCenter(pAnmSprite->getSpriteWidth(), 0.f); break;
			case ANCHOR_LOWER_RIGHT: if(pAnmSprite) pAnmSprite->setCenter(pAnmSprite->getSpriteWidth(), pAnmSprite->getSpriteHeight()); break;
			case ANCHOR_LOWER_LEFT: if(pAnmSprite) pAnmSprite->setCenter(0.f, pAnmSprite->getSpriteHeight()); break;
			case ANCHOR_MIDDLE: if(pAnmSprite) pAnmSprite->setCenter(pAnmSprite->getSpriteWidth()/2, pAnmSprite->getSpriteHeight()/2); break;
		}
	}
	
	ANIMATION_SPRITE_ANCHOR SoraSpriteAnimation::getAnchor() const {
		return sprAnchor;
	}
} // namespace sora

