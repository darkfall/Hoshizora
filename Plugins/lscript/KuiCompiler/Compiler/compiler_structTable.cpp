/*
 *  compiler_structTable.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月27日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "compiler_structTable.h"

int LSH_StructTable::addStruct(const string& sName) {
	if(isStructExist(sName))
		return -1;
	
	LSH_Struct* pTempStruct = new LSH_Struct;
	pTempStruct->sStructName = sName;
	pTempStruct->iRealSize = 0;
	pTempStruct->vElmnts.clear();
	
	vStructs.push_back(pTempStruct);
	
	return vStructs.size()-1;
}

int LSH_StructTable::addStructElmnt(int iStructIndex, const string& sElmntName, size_t iSize, const char* pstrOriginatedName) {
	if(isElmntExist(iStructIndex, sElmntName))
		return LSH_STRUCTTABLE_DUPLICATEELMNT;
	
	if(!isElmntStruct(sElmntName)) {
		if(iStructIndex >= static_cast<int>(vStructs.size()))
			return LSH_STRUCTTABLE_INVALID_INDEX;
		
		LSH_StructElmnt tempElmnt;
		tempElmnt.sElmntName = sElmntName;
		tempElmnt.iIndex = vStructs[iStructIndex]->vElmnts.size();
		tempElmnt.iSize = iSize;
		if(pstrOriginatedName) 
			tempElmnt.sOriginatedName = pstrOriginatedName;
		
		vStructs[iStructIndex]->vElmnts.push_back(tempElmnt);
		
		vStructs[iStructIndex]->iRealSize += iSize;		
	} else {
		LSH_Struct* pStruct = getStructByIdent(sElmntName);
		
		for(size_t iCurrElmnt = 0; iCurrElmnt < pStruct->vElmnts.size(); ++iCurrElmnt)
			addStructElmnt(iStructIndex, pStruct->vElmnts[iCurrElmnt].sElmntName, pStruct->vElmnts[iCurrElmnt].iSize, pstrOriginatedName);
	}
	return LSH_STRUCTTABLE_SUCCESS;
}

int LSH_StructTable::getElmntOffset(const string& sStructName, const string& sElmntName, size_t iIndex, const char* pstrOriginatedName) {
	LSH_Struct* pStruct = getStructByIdent(sStructName);
	if(pStruct) {
		for(size_t iCurrElmnt = 0; iCurrElmnt < pStruct->vElmnts.size(); ++iCurrElmnt) {
			if(pStruct->vElmnts[iCurrElmnt].sElmntName.compare(sElmntName) == 0) {
				if(pstrOriginatedName) {
					if(pStruct->vElmnts[iCurrElmnt].sOriginatedName.compare(pstrOriginatedName) == 0) {
						if(iIndex == 0) {
							if(pStruct->vElmnts[iCurrElmnt].iSize != 1)
								return -2;
						}
						return iCurrElmnt;
					}
				} else {
					if(iIndex == 0) {
						if(pStruct->vElmnts[iCurrElmnt].iSize != 1)
							return -2;
					}
					return iCurrElmnt;
				}
			}
		}
	}
	return -1;
}

bool LSH_StructTable::isElmntExist(int iStructIndex, const string& sElmntName) {
	if(iStructIndex >= static_cast<int>(vStructs.size()))
		return true;
	
	for(size_t iCurrElmnt = 0; iCurrElmnt < vStructs[iStructIndex]->vElmnts.size(); ++iCurrElmnt)
		if(vStructs[iStructIndex]->vElmnts[iCurrElmnt].sElmntName.compare(sElmntName) == 0)
			return true;
	return false;
}

bool LSH_StructTable::isElmntStruct(const string& sElmntName) {
	for(size_t iCurrStruct = 0; iCurrStruct < vStructs.size(); ++iCurrStruct) {
		if(vStructs[iCurrStruct]->sStructName.compare(sElmntName) == 0)
			return true;
	}
	return false;
}

bool LSH_StructTable::isStructExist(const string& sStructName) {
	for(size_t iCurrStruct = 0; iCurrStruct < vStructs.size(); ++iCurrStruct)
		if(vStructs[iCurrStruct]->sStructName.compare(sStructName) == 0)
			return true;
	return false;
}

LSH_Struct* LSH_StructTable::getStructByIdent(const string& sStructName) {
	for(size_t iCurrStruct = 0; iCurrStruct < vStructs.size(); ++iCurrStruct) {
		if(vStructs[iCurrStruct]->sStructName.compare(sStructName) == 0)
			return vStructs[iCurrStruct];
	}
	return 0;
}

int LSH_StructTable::getStructSize(const string& sName) {
	LSH_Struct* pStruct = getStructByIdent(sName);
	if(pStruct)
		return pStruct->iRealSize;
	return 0;
}
