/*
 *  SoraUncopyable.h
 *  Sora
 *
 *  Created by Robert Bu on 12/18/10.
 *  Copyright 2010 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */


#ifndef SORA_UNCOPYABLE_H_
#define SORA_UNCOPYABLE_H_

#include "SoraPlatform.h"

namespace sora {
    
    class SORA_API SoraUncopyable {
	protected:
		SoraUncopyable() {}
		
    private:
        const SoraUncopyable& operator=(const SoraUncopyable&);
        SoraUncopyable(const SoraUncopyable&);
    };
}


#endif