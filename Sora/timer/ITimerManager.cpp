//
//  ITimerManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "ITimerManager.h"

namespace sora  {
      
    template<> 
    ITimerManager* SoraDirectSingleton<ITimerManager>::_singleton = NULL;
    
}
