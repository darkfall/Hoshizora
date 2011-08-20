//
//  SoraGlobalJsonFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraGlobalFactory_h
#define Sora_SoraGlobalFactory_h

#include "SoraFactory.h"

namespace sora {
   
#define REG_GLOBAL_RPODUCT_0(T, NAME, FN) \
    SoraAbstractFactory<T>::Instance()->reg(name, fn);
    
#define REG_GLOBAL_PRODUCT_1(T, NAME, FN, A0) \
    SoraAbstractFactory<T, A0>::Instance()->reg(name, fn);

#defien REG_GLOBAL_PRODUCT_2(T, name, FN, A1) \
    SoraAbstractFactory<T, A0, A1>::Instance()->reg(name, fn);
    
} // namespace sora



#endif
