//
//  Base64.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/10/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "SoraBase64.h"
#include "SoraMemory.h"
#include "SoraLogger.h"

namespace sora {
    
    static std::string base64_Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    SoraArray<uint8> base64_decode(const SoraString& input) {
        int in_len = (int)input.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        SoraArray<uint8> ret;
        
        while (in_len-- && ( input[in_] != '=') && is_base64(input[in_])) {
            char_array_4[i++] = input[in_]; in_++;
            if (i ==4) {
                for (i = 0; i <4; i++)
                    char_array_4[i] = base64_Chars.find(char_array_4[i]);
                
                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
                
                for (i = 0; (i < 3); i++)
                    ret.append(char_array_3[i]);
                i = 0;
            }
        }
        
        if (i) {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;
            
            for (j = 0; j <4; j++)
                char_array_4[j] = base64_Chars.find(char_array_4[j]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (j = 0; (j < i - 1); j++) ret.append(char_array_3[j]);
        }
        
        return ret;
    }
    
    SoraString base64_encode(const uint8* bytes_to_encode, uint32 in_len) {
        SoraString ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];
        
        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;
                
                for(i = 0; (i <4) ; i++)
                    ret += base64_Chars[char_array_4[i]];
                i = 0;
            }
        }
        
        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';
            
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (j = 0; (j < i + 1); j++)
                ret += base64_Chars[char_array_4[j]];
            
            while((i++ < 3))
                ret += '=';
        }
        
        return ret;
        
    }
    
} // namespace ukn
