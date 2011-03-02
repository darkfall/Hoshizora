/*
 *  Singleton.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 5/15/10.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "Singleton.h"

template<typename T>
T* mSingleton<T>::pInstance = 0;

template<typename T>
T* mSingleton<T>::GetInstance() {
	if(!pInstance) pInstance = new T;
	return pInstance;
}

template<typename T>
void mSingleton<T>::Destroy() {
	if(pInstance) delete pInstance;
}