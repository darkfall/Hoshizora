//
//  SoraRegex.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraRegex.h"
#include "SoraException.h"

#include "pcre.h"

namespace sora {
    
    const int SoraRegex::OVEC_SIZE = 64;
    
    SoraRegex::SoraRegex(const std::string& pattern, int options, bool study):
    _pcre(0),
    _extra(0) {
        const char* error;
        int offs;
        _pcre = pcre_compile(pattern.c_str(),
                             options,
                             &error,
                             &offs,
                             0);
        if(!_pcre) {
            std::ostringstream msg;
            msg << error << " (at offset " << offs << ")";
            THROW_SORA_EXCEPTION(RegularExpressionException, msg.str());
        }
    }
    
    SoraRegex::~SoraRegex() {
        if(_pcre)
            pcre_free(_pcre);
        if(_extra)
            pcre_free(_extra);
    }
    
    int SoraRegex::match(const std::string& subject, SizeType offset, Match& mtch, int options) const {
        sora_assert(offset <= subject.length());
        
        int ovec[OVEC_SIZE];
        int rc = pcre_exec(_pcre, 
                           _extra, 
                           subject.c_str(), 
                           int(subject.size()), 
                           int(offset), 
                           options & 0xFFFF, 
                           ovec, 
                           OVEC_SIZE);
        
        if(rc == PCRE_ERROR_NOMATCH) {
            mtch.offset = std::string::npos;
            mtch.length = 0;
            return 0;
        } 
        else if(rc == PCRE_ERROR_BADOPTION) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: bad option");
        }
        else if(rc == 0) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: too many captured substrings");
        }
        else if(rc < 0) {
            std::ostringstream msg;
            msg << "PCRE error " <<rc;
            THROW_SORA_EXCEPTION(RegularExpressionException, msg.str());
        }
        mtch.offset = ovec[0] < 0 ? std::string::npos : ovec[0];
        mtch.length = ovec[1] - mtch.offset;
        return rc;
    }
    
    int SoraRegex::match(const std::string& subject, SizeType offset, MatchVec& matches, int options) const {
        sora_assert(offset <= subject.length());
        
        int ovec[OVEC_SIZE];
        int rc = pcre_exec(_pcre, 
                           _extra, 
                           subject.c_str(), 
                           int(subject.size()), 
                           int(offset), 
                           options & 0xFFFF, 
                           ovec, 
                           OVEC_SIZE);
        
        if(rc == PCRE_ERROR_NOMATCH) {
            return 0;
        } 
        else if(rc == PCRE_ERROR_BADOPTION) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: bad option");
        }
        else if(rc == 0) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: too many captured substrings");
        }
        else if(rc < 0) {
            std::ostringstream msg;
            msg << "PCRE error " <<rc;
            THROW_SORA_EXCEPTION(RegularExpressionException, msg.str());
        }
        matches.reserve(rc);
        for(int i=0; i<rc; ++i) {
            Match m;
            m.offset = ovec[i*2] < 0 ? std::string::npos : ovec[i*2];
            m.length = ovec[i*2+1] - m.offset;
            matches.push_back(m);
        }
        return rc;
    }
    
    bool SoraRegex::match(const std::string& subject, SizeType offset) const {
        Match mtch;
        match(subject, offset, mtch, REGEX_ANCHORED | REGEX_NOTEMPTY);
        return mtch.offset == offset && mtch.length == subject.length() - offset;
    }
    
    bool SoraRegex::match(const std::string& subject, SizeType offset, int options) const {
        Match mtch;
        match(subject, offset, mtch, options);
        return mtch.offset == offset && mtch.length == subject.length() - offset;
    }
    
    int SoraRegex::extract(const std::string& subject, std::string& str, int options) const {
        Match mtch;
        int rc = match(subject, 0, mtch, options);
        if(mtch.offset != std::string::npos)
            str.assign(subject, mtch.offset, mtch.length);
        else 
            str.clear();
        return rc;
    }
    
    int SoraRegex::extract(const std::string& subject, SizeType offset, std::string& str, int options) const {
        Match mtch;
        int rc = match(subject, offset, mtch, options);
        if(mtch.offset != std::string::npos)
            str.assign(subject, mtch.offset, mtch.length);
        else
            str.clear();
        return rc;
    }
    
    int SoraRegex::split(const std::string& subject, SizeType offset, std::vector<std::string>& strings, int options) const {
        MatchVec matches;
        strings.clear();
        int rc = match(subject, offset, matches, options);
        strings.reserve(matches.size());
        for(MatchVec::const_iterator it = matches.begin();
            it != matches.end();
            ++it) {
            if(it->offset != std::string::npos) 
                strings.push_back(subject.substr(it->offset, it->length));
            else
                strings.push_back(std::string());
        }
        return rc;
    }
    
    int SoraRegex::subst(std::string& subject, SizeType offset, const std::string& replacement, int options) const {
        if(options & REGEX_GLOBAL) {
            int rc = 0;
            SizeType pos = substOne(subject, offset, replacement, options);
            while( pos != std::string::npos ) {
                ++rc;
                pos = substOne(subject, pos, replacement, options);
            }
            return rc;
        } else {
            return substOne(subject, offset, replacement, options) != std::string::npos ? 1 : 0;
        }
    }
    
    SoraRegex::SizeType SoraRegex::substOne(std::string& subject, SizeType offset, const std::string& replacement, int options) const {
        if(offset >= subject.length())
            return std::string::npos;
        
        int ovec[OVEC_SIZE];
        int rc = pcre_exec(_pcre, 
                           _extra, 
                           subject.c_str(),
                           int(subject.size()),
                           int(offset),
                           options & 0xFFFF,
                           ovec,
                           OVEC_SIZE);
        if(rc == PCRE_ERROR_NOMATCH) {
            return std::string::npos;
        } 
        else if(rc == PCRE_ERROR_BADOPTION) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: bad option");
        }
        else if(rc == 0) {
            THROW_SORA_EXCEPTION(RegularExpressionException, "SoraRegex::match: too many captured substrings");
        }
        else if(rc < 0) {
            std::ostringstream msg;
            msg << "PCRE error " <<rc;
            THROW_SORA_EXCEPTION(RegularExpressionException, msg.str());
        }
        
        std::string result;
        SizeType len = subject.length();
        SizeType pos = 0;
        SizeType rp = std::string::npos;
        while(pos < len) {
            if(ovec[0] == pos) {
                std::string::const_iterator it = replacement.begin();
                std::string::const_iterator end = replacement.end();
                while(it != end) {
                    if(*it == '$' && !(options & REGEX_NO_VARS)) {
                        ++it;
                        if(it != end) {
                            char d = *it;
                            if(d >= '0' && d <= '9') {
                                int c = d - '0';
                                if(c < rc) {
                                    int o = ovec[c*2];
                                    int l = ovec[c*2+1] - o;
                                    result.append(subject, o, l);
                                }
                            } else {
                                result = '$';
                                result += d;
                            }
                            ++it;
                        } else
                            result += '$';
                    } else
                        result += *it++;
                }
                pos = ovec[1];
                rp = result.length();
            } else
                result += subject[pos++];
        }
        subject = result;
        return pos;
    }
    
    bool SoraRegex::match(const std::string& subject, const std::string& pattern, int options) {
        int ctorOptions = options & (REGEX_CASELESS | REGEX_MULTILINE | REGEX_DOTALL |
                                     REGEX_EXTENDED | REGEX_UTF8 | REGEX_NO_AUTO_CAPTURE | 
                                     REGEX_UNGREEDY | REGEX_DOLLAR_ENDONLY | REGEX_EXTRA | REGEX_ANCHORED);
        int mtchOptions = options & (REGEX_ANCHORED | REGEX_NOTBOL | REGEX_NOTEOL |
                                     REGEX_NOTEMPTY | REGEX_NO_AUTO_CAPTURE | 
                                     REGEX_NO_UTF8_CHECK);
        SoraRegex regex(pattern, ctorOptions, false);
        return regex.match(subject, 0, mtchOptions);
    }
} // namespace sora
