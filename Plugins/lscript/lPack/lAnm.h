/*
 *  lAnm.h
 *  lpack
 *
 *  Created by GriffinBu on 12/9/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#ifndef _LANIM_H_
#define _LANIM_H_

#define LANIM_HEADER "LANM" // 4

#include <vector>

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
};

struct LANM_NODE {
	uint8 anmId;
	uint8 sprCount;
	
	vector<LANM_TEX> texlist;
};


class lAnm {
public:
	
	
private:
	lAnm(const lAnm&);
	lAnm& operator(const lAnm&);
};

#endif // _L_ANIM_H_