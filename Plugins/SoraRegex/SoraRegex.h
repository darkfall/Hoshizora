//
//  SoraRegex.h
//  Sora
//
//  Created by Ruiwei Bu on 8/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraRegex_h
#define Sora_SoraRegex_h

#include "SoraPlatform.h"
#include "uncopyable.h"
#include <vector>


extern "C" {
    struct real_pcre;
    typedef struct real_pcre pcre;
    struct pcre_extra;
}

namespace sora {
    
    /** 
     *  Original RegularExpression class from Poco Library
     *  Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
     *  and Contributors.
     *  Modified for Hoshizora by Griffin Bu
     **/
    
    class SORA_API SoraRegex: public uncopyable {
    public:
        enum Options {
            // case insensitive matching
            // [ctor]
            REGEX_CASELESS          = 0x00000001,
            // enable multi-line mode; affects ^ and $
            // [ctor]
            REGEX_MULTILINE         = 0x00000002,
            // dot matches all characters, including newline
            // [ctor]
            REGEX_DOTALL            = 0x00000004,
            // totally ignore whitespace
            // [ctor]
            REGEX_EXTENDED          = 0x00000004,
            // treat pattern as if it starts with a ^
            // [ctor]
            REGEX_ANCHORED          = 0x00000010,
            // dollar matches end-of-string only, not last newline in string
            // [ctor, match]
            REGEX_DOLLAR_ENDONLY    = 0x00000020,
            // enable optional PCRE functionality
            // [ctor]
            REGEX_EXTRA             = 0x00000040,
            // circumflex does not match beginning of string
            // [match]
            REGEX_NOTBOL            = 0x00000080,
            // $ does not match end of string
            // [match]
            REGEX_NOTEOL            = 0x00000100,
            // make quantifiers ungreedy
            // [ctor]
            REGEX_UNGREEDY          = 0x00000200,
            // empty string never matches
            // [match]
            REGEX_NOTEMPTY          = 0x00000400,
            // assume pattern and subject is UTF8 encoded
            // [ctor]
            REGEX_UTF8              = 0x00000800,
            // disable numbered capturing parentheses
            // [ctor, match]
            REGEX_NO_AUTO_CAPTURE   = 0x00001000,
            // do not check validity of UTF-8 code sequence
            // [match]
            REGEX_NO_UTF8_CHECK     = 0x00002000,
            // an unanchored pattern is required to match before or at the first
            // new line in the subject string though the matched text may continue
            // over the new line
            // [ctor]
            REGEX_FIRSTLINE         = 0x00040000,
            // names used to identify capturing subpatterns need not be unique
            // [ctor]
            REGEX_DUPNAMES          = 0x00080000,
            // assume newline is CR('\r')
            // [ctor]
            REGEX_NEWLINE_CR        = 0x00100000,
            // assume new line is LF('\n')
            // [ctor]
            REGEX_NEWLINE_LF        = 0x00200000,
            // assume new line is CRLF("\r\n")
            // [ctor]
            REGEX_NEWLINE_CRLF      = 0x00400000,
            // assume newline is any valid Unicode newline character, CR, LF, CRLF
            // [ctor]
            REGEX_NEWLINE_ANYCRLF   = 0x00500000,
            // replace all occurences
            // [subst]
            REGEX_GLOBAL            = 0x10000000,
            // treat dollar in replacement string as ordinary character
            // [subst]
            REGEX_NO_VARS           = 0x20000000
        };
        
        typedef std::string::size_type SizeType;
        
        struct Match {
            SizeType offset;
            SizeType length;
        };
        
        typedef std::vector<Match> MatchVec;
        
        /**
         * Creates a regular expression and parses the given pattern
         * If study is true, the pattern is analyzed and optimized.
         * This is mainly useful if the pattern is used more than once
         * For a description of the options, please see the PCRE documentations
         * Throws a SoraException if the pattern cannot be compiled
         **/
        SoraRegex(const std::string& pattern, int options = 0, bool study=true);
        ~SoraRegex();
        
        /**
         * Matches the given subject string against the patttern.
         * Return the position of the first captured substring in mtch
         * If no part of the subject matches the pattern
         *      mtch.offset = std::string::npos
         *      mtch.length = 0
         * Throws SoraException in case of an error
         * Returns the number of matches
         **/
        int match(const std::string& subject, Match& mtch, int options=0) const;
        /**
         * Matches the given subject string against the patttern. starting at offset
         * Return the position of the first captured substring in mtch
         * If no part of the subject matches the pattern
         *      mtch.offset = std::string::npos
         *      mtch.length = 0
         * Throws SoraException in case of an error
         * Returns the number of matches
         **/
        int match(const std::string& subject, SizeType offset, Match& mtch, int options = 0) const;
        /**
         * Matches the given subject string against the patttern.
         * The first entry in matches contains the position of the captured substring
         * The following entries identify matching subpatterns. 
         * See the PCRE documentations for more detailed explanation
         * If no part of the subject matches the pattern
         *      matches is empty
         * Throws SoraException in case of an error
         * Returns the number of matches
         **/
        int match(const std::string& subject, SizeType offset, MatchVec& matches, int options=0) const;
        /**
         * Returns true if and only if the subject matches the regular expression
         * Internally, this method sets the REGEX_ANCHORED and REGEX_NOTEMPTY options of matching
         * which means that the empty string will never match and the pattern is treated 
         * as if it starts with a ^
         **/
        bool match(const std::string& subject, SizeType offset = 0) const;
        /**
         * Returns true if and only if the subject matches the regular expression
         **/
        bool match(const std::string& subject, SizeType offset, int options) const;
        /**
         * Same as bool match
         **/
        bool operator ==(const std::string& subject) const;
        bool operator !=(const std::string& subject) const;
        
        /**
         * Matches the given subject string against the pattern
         * Returns the captured string
         * Throws a SoraException in case of an error
         * Returns the number of matches
         **/
        int extract(const std::string& subject, std::string& str, int options=0) const;
        /**
         * Matches the given subject string, starting at offset, against the pattern
         * Returns the captured string
         * Throws a SoraException in case of an error
         * Returns the number of matches
         **/
        int extract(const std::string& subject, SizeType offset, std::string& str, int options=0) const;
        /**
         * Matches the given subject string against the pattern
         * The first entry in captured is the captured substring
         * The following entries contain substrings matching subpattern
         * Please see PCRE documentation for more detailed explanation
         * If no part of the subject matches the pattern, captured is empty
         * Throw a SoraException in case of an error
         * Returns the number of matches
         **/
        int split(const std::string& subject, std::vector<std::string>& strings, int options=0) const;
        /**
         * Matches the given subject string, starting at offset, against the pattern
         * The first entry in captured is the captured substring
         * The following entries contain substrings matching subpattern
         * Please see PCRE documentation for more detailed explanation
         * If no part of the subject matches the pattern, captured is empty
         * Throw a SoraException in case of an error
         * Returns the number of matches
         **/
        int split(const std::string& subject, SizeType offset, std::vector<std::string>& strings, int options=0) const;
        /**
         * Subsitute in subject all matches of the pattern with replacement
         * If REGEX_GLOBAL is specified as option, all matches are replaced
         * Otherwise only the first match is replaced
         * Occurences of $<n> (for example, $1, $2, ....) in replacement are replaced
         * with corresponding captured string. $0 is the original subject string
         * Returns the number of replaced occurences
         **/
        int subst(std::string& subject, const std::string& replacement, int options=0) const;
        /**
         * Subsitute in subject all matches of the pattern with replacement, starting at offset
         * If REGEX_GLOBAL is specified as option, all matches are replaced
         * Otherwise only the first match is replaced
         * Unless REGEX_NO_VARS is specified, 
         * Occurences of $<n> (for example, $1, $2, ....) in replacement are replaced 
         * with corresponding captured string. $1 ... $n are the substrings matching the subpatterns
         * Returns the number of replaced occurences
         **/
        int subst(std::string& subject, const SizeType offset, const std::string& replacement, int options=0) const;
        /**
         * Matches the given subject string against the regular expression given in pattern
         * using given pattern options
         **/
        static bool match(const std::string& subject, const std::string& pattern, int options=0);
        
    protected:
        SizeType substOne(std::string& subject, SizeType offset, const std::string& replacement, int options) const;
        
    private:
        pcre* _pcre;
        pcre_extra* _extra;
        
        static const int OVEC_SIZE;
        
        SoraRegex();
    };
    
    inline int SoraRegex::match(const std::string& subject, Match& mtch, int options) const {
        return match(subject, 0, mtch, options);
    }
    
    inline int SoraRegex::split(const std::string& subject, std::vector<std::string>& strings, int options) const {
        return split(subject, 0, strings, options);
    }
    
    inline int SoraRegex::subst(std::string& subject, const std::string& replacement, int options) const {
        return subst(subject, 0, replacement, options);
    }
    
    inline bool SoraRegex::operator ==(const std::string& subject) const {
        return match(subject);
    }
    
    inline bool SoraRegex::operator !=(const std::string& subject) const {
        return !match(subject);
    }
} // namespace sora


#endif
