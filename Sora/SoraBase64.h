//
//  Base64.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/10/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_Base64_h
#define Project_Unknown_Base64_h

#include "SoraPlatform.h"
#include "util/SoraArray.h"

namespace sora {
    
    SoraArray<uint8> base64_decode(const SoraString& input);
    SoraString base64_encode(const uint8* bytes_to_encode, uint32 in_len);
    
} // namespace ukn


#endif
