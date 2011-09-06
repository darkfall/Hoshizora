//
//  SoraURI.h
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraURI_h
#define Sora_SoraURI_h

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
    
    /**
     An URI object can split a Uniform Resource Identifier string into
     its components or build a string from URI components. Please note
     that the memory footprint of an URI object is always bigger then
     a pure String object, so if memory usage is of concern, it is advised
     to keep paths as String objects around, and only use URI objects
     to encode and decode them.
     
     An URI is made of the following components, where most of them
     are optional:
     
     Scheme://UserInfo@Host:Port/LocalPath#Fragment?Query
     
     Example URIs:
     
     http://user:password@www.myserver.com:8080/index.html#main
     http://www.myserver.com/query?user=bla
     ftp://ftp.myserver.com/pub/bla.zip
     file:///c:/temp/bla.txt
     file://SambaServer/temp/blub.txt
     
     Note that assigns will be resolved before splitting a URI into its
     components, for instance the assign "textures" could be defined
     as:
     
     Assign("textures", "http://www.dataserv.com/myapp/textures/");
     
     So a path to a texture URI could be defined as:
     
     URI("textures:mytex.dds")
     
     Which would actually resolve into:
     
     http://www.dataserv.com/myapp/textures/mytex.dds
     
     Decoding into components happens in the init constructor or the 
     Set() method in the following steps:
     
     - resolve any assigns in the original string
     - split into Scheme, Host and Path blocks
     - resolve Host and Path blocks further
     
     Enconding from components into string happens in the AsString()
     method in the following steps:
     
     - concatenate URI string from components
     - convert part of the string back into an existing assign
     **/
    
    class SoraURI {
    public:
        SoraURI();
        SoraURI(const util::String& s);
        SoraURI(const char* s);
        SoraURI(const SoraURI& s);
        
        void operator=(const SoraURI& rhs);
        bool operator==(const SoraURI& rhs) const;
        bool operator!=(const SoraURI& rhs) const;
        
        void set(const util::String& s);
        util::String asString() const;
        
        bool isEmpty() const;
        bool isValid() const;
        
        void clear();
        void setScheme(const util::String& s);
        const util::String& scheme() const;
        void setUserInfo(const util::String& s);
        const util::String& userInfo() const;
        void setHost(const util::String& s);
        const util::String& host() const;
        void setPort(const util::String& s);
        const util::String& port() const;
        void setLocalPath(const util::String& s);
        const util::String& localPath(const util::String& s);
    };
    
} // namespace sora


#endif
