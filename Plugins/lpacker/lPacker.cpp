/*
 *  lPacker.cpp
 *  lpack
 *
 *  Created by griffin clare on 12/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "lPacker.h"

lPacker::lPacker() {
}

lPacker::~lPacker() {
}

void lPacker::addFile(const char* pstrFile) {
	toPackFiles.push_back(pstrFile);
}

void lPacker::delFile(const char* pstrFile) {
	PACK_LIST_IT it = toPackFiles.begin();
	while(it != toPackFiles.end()) {
		if(it->compare(pstrFile) == 0) {
			toPackFiles.erase(it);
			break;
		}
		++it;
	}
}

int lPacker::end(FILE* file) {
	fclose(file);
	return 0;
}

void lPacker::setVer(uint32 major, uint32 minor) {
	header.majorVer = major;
	header.minorVer = minor;
}

int lPacker::pack(const char* dest) {
	FILE* file = fopen(dest, "wb");
	if(!file) return 0;
	
	packNodes.clear();
	failedFiles.clear();
	
	// head ident
	fwrite(LPACKER_HEADER, 4, 1, file);
	// header
	char cVer = header.majorVer == 255 ? LPACKER_MAJOR_VER : header.majorVer;
	fwrite(&cVer, 1, 1, file);
	cVer = header.minorVer == 255 ? LPACKER_MINOR_VER : header.minorVer;
	fwrite(&cVer, 1, 1, file);
	
	PACK_LIST_IT it = toPackFiles.begin();
	while(it != toPackFiles.end()) {		
		FILE* pf = fopen(it->c_str(), "rb");
		if(!pf) {
			failedFiles.push_back(it->c_str());
			it = toPackFiles.erase(it);
			fclose(pf);
		}
		else ++it;
	}
	
	uint32 size = toPackFiles.size();
	fwrite(&size, 4, 1, file);
	if(ferror(file)) return end(file);
	
	uint32 currOffset = 24;
	uint16 stringRefCount = 0;
	
	it = toPackFiles.begin();
	while(it != toPackFiles.end()) {
		LPACKER_NODE node;
		
		FILE* pf = fopen(it->c_str(), "rb");
		if(pf) {			
			void* pData = getData(pf, node.size, node.origSize);
			if(node.size > 0 && pData) {
				node.offset = currOffset;
				node.nameRefNum = stringRefCount;
				node.sName = it->c_str();
				packNodes.push_back(node);
			
				currOffset += node.size;
				currOffset += 14;
				stringRefCount += 1;
							
				fwrite(&node.offset, 4, 1, file);		
				fwrite(&node.size, 4, 1, file);	
				fwrite(&node.origSize, 4, 1, file);	
				fwrite(&node.nameRefNum, 2, 1, file);
				fwrite(pData, node.size, 1, file);
				
				if(ferror(file)) return end(file);
				
				free(pData);
			}
			fclose(pf);
		}
		
		++it;
	}
	
	PACK_FILE_LIST_IT itFile = packNodes.begin();
	while(itFile != packNodes.end()) {
		uint16 sLength = itFile->sName.size();
		fwrite(&sLength, 2, 1, file);					
		fwrite(itFile->sName.c_str(), sLength, 1, file);
		++itFile;
	}
	
	fclose(file);
	return 1;
}

void* lPacker::getData(FILE* pf, uint32& size, uint32& osize) {
	uint32 fsize = 0;
	fseek(pf, 0, SEEK_END);
	fsize = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	
	void* data = malloc(fsize);
	if(data) {
		fread(data, 1, fsize, pf);	if(ferror(pf)) return 0;
	} else {
		return 0;
	}
	osize = fsize;
	size = fsize;
	return data;
}

#include <cassert>

lPackLoader::lPackLoader() {
}

lPackLoader::~lPackLoader() {
}

void lPackLoader::close() {
	if(packFile)
		fclose(packFile);
	
	packNodes.clear();
}

int lPackLoader::loadPack(const char* pstrFile) {
	packFile = fopen(pstrFile, "rb");
	if(packFile) {
		char strHeader[5];
		fread(&strHeader, 4, 1, packFile);
		strHeader[4] = '\0';
		if(strcmp(strHeader, LPACKER_HEADER) != 0) {
			close();
			return 0;
		}
		
		char ver;
		fread(&ver, 1, 1, packFile);
		if(ver != header.majorVer) {
			close();
			return 0;
		}		
		fread(&ver, 1, 1, packFile);
		if(ver != header.minorVer) {
			close();
			return 0;
		}
		if(ferror(packFile)) {
			fclose(packFile);
			return 0;
		}
		
		fread(&header.fileNum, 4, 1, packFile);
		for(uint32 i=0; i<header.fileNum; ++i) {
			LPACKER_NODE node;
			
			fread(&node.offset, 4, 1, packFile);
			fread(&node.size, 4, 1, packFile);
			fread(&node.origSize, 4, 1, packFile);
			fread(&node.nameRefNum, 2, 1, packFile);
			
			if(ferror(packFile)) {
				fclose(packFile);
				return 0;
			}
			packNodes.push_back(node);
			
			fseek(packFile, node.offset+node.size, SEEK_SET);
		}
		
		LPACKER_NODE end = packNodes.back();
		fseek(packFile, end.offset+end.size, SEEK_SET);
		for(uint32 i=0; i<header.fileNum; ++i) {
			uint16 slen;
			fread(&slen, 2, 1, packFile);
			char* strFileName = (char*)malloc(slen+1);
			fread(strFileName, slen, 1, packFile);
			strFileName[slen] = '\0';
			packNodes[i].sName = strFileName;
			free(strFileName);
			
			if(ferror(packFile)) {
				fclose(packFile);
				return 0;
			}
		}
		
		fseek(packFile, 0, SEEK_SET);
		return 1;
	}
	return 0;
}

uint32 lPackLoader::getData(void* pDataIn, void* pDataOut, LPACKER_NODE node) {
	memcpy(pDataOut, pDataIn, node.origSize);
	return node.origSize;
}

uint32 lPackLoader::getFileSize(const char* pstrFileName) {
	if(!packFile) return 0;
	
	PACK_FILE_LIST_IT it = packNodes.begin();
	while(it != packNodes.end()) {
		if(it->sName.compare(pstrFileName) == 0) return it->origSize;
		++it;
	}
	return 0;
}

uint32 lPackLoader::getFile(const char* pstrFileName, void* pFileData) {
	if(!packFile) return 0;

	PACK_FILE_LIST_IT it = packNodes.begin();
	while(it != packNodes.end()) {
		if(it->sName.compare(pstrFileName) == 0) {
			fseek(packFile, it->offset, SEEK_SET);
			if(ferror(packFile)) {
				fclose(packFile);
				return 0;
			}
			
			void* pTempData = malloc(it->size);
			if(pTempData) {
				fread(pTempData, 1, it->size, packFile);
				uint32 readSize = getData(pTempData, pFileData, *it);
				free(pTempData);
				return readSize;
			}
			return 0;
		}
		++it;
	}
	return 0;
}

void lPackLoader::setVer(uint32 major, uint32 minor) {
	header.majorVer = major;
	header.minorVer = minor;
}
