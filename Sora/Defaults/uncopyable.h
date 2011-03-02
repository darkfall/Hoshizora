/*
 *  uncopyable.h
 *  Sora
 *
 *  Created by GriffinBu on 12/18/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#ifndef SORA_UNCOPYABLE_H_
#define SORA_UNCOPYABLE_H_

class uncopyable {
public:
	virtual uncopyable() {}
	
private:
	uncopyable& operator=(const uncopyable&);
	uncopyable(const uncopyable&);
};


#endif