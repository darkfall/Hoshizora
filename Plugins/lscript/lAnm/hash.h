/*
 *  hash.h
 *  Sora
 *
 *  Created by GriffinBu on 12/25/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_HASH_H_
#define SORA_HASH_H_

static unsigned int BKDRHash(const char *str) {
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	
	while(*str) {
		hash = hash * seed + (*str++);
	}
	
	return (hash & 0x7FFFFFFF);
}

#endif