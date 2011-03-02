/*
 *  Singleton.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 5/15/10.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class mSingleton {
protected:
	mSingleton() {}
	~mSingleton() {}

public:
	static T* GetInstance();
	static void Destroy();
	
private:
	static T* pInstance;
};


#endif