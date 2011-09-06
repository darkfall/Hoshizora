/*
 *  SoraUncopyable.h
 *  Sora
 *
 *  Created by GriffinBu on 12/18/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#ifndef SORA_UNCOPYABLE_H_
#define SORA_UNCOPYABLE_H_

namespace sora {
    
    class SoraUncopyable {
	protected:
		SoraUncopyable() {}
		
    private:
        const SoraUncopyable& operator=(const SoraUncopyable&);
        SoraUncopyable(const SoraUncopyable&);
    };
}


#endif