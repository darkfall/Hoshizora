//
//  SoraConfigParser.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraConfigParser.h"

#include "SoraConfigParserImpl_Xml.h"
#include "SoraConfigParserImpl_Json.h"

namespace sora {
    
    SoraConfigParser::SoraConfigParser():
    mImpl(0) {
        
    }
    
    SoraConfigParser::~SoraConfigParser() {
        close();
    }
    
    bool SoraConfigParser::open(const std::wstring& path, ConfigType type) {
        switch(type) {
            case CONFIG_JSON:
                mImpl = new SoraConfigParserJsonImpl;
                break;
            case CONFIG_XML:
                mImpl = new SoraConfigParserXmlImpl;
                break;
                
        }
        if(mImpl) {
            return mImpl->open(path);
        }
    }
    
    bool SoraConfigParser::open(void* data, ulong32 size, ConfigType type) {
        switch(type) {
            case CONFIG_JSON:
                mImpl = new SoraConfigParserJsonImpl;
                break;
                
        }
        if(mImpl) {
            return mImpl->open(data, size);
        }
    }
    
    bool SoraConfigParser::open(const std::string& string, ConfigType type) {
        return open((void*)string.c_str(), (ulong32)string.size(), type);
    }
    
    bool SoraConfigParser::writeToFile(const std::wstring& path) {
        sora_assert(mImpl);
        return mImpl->writeToFile(path);
    }
    
    std::string SoraConfigParser::writeToString() const {
        sora_assert(mImpl);
        return mImpl->writeToString();
    }
    
    void SoraConfigParser::close() {
        if(mImpl) {
            mImpl->close();
            delete mImpl;
            mImpl = 0;
        }
    }
    
    bool SoraConfigParser::hasNode(const std::string& node) const {
        sora_assert(mImpl);
        return mImpl->hasNode(node);
    }
    
    std::string SoraConfigParser::getCurrentNodeName() const {
        sora_assert(mImpl);
        return mImpl->getCurrentNodeName();
    }
    
    std::string SoraConfigParser::getCurrentNodePath() const {
        sora_assert(mImpl);
        return mImpl->getCurrentNodePath();
    }
    
    bool SoraConfigParser::toNode(const std::string& node) {
        sora_assert(mImpl);
        return mImpl->toNode(node);
    }
    
    bool SoraConfigParser::toFirstChild(const std::string& name) {
        sora_assert(mImpl);
        return mImpl->toFirstChild(name);
    }
    
    bool SoraConfigParser::toNextChild(const std::string& name) {
        sora_assert(mImpl);
        return mImpl->toNextChild(name);
    }
    
    bool SoraConfigParser::toParent() {
        sora_assert(mImpl);
        return mImpl->toParent();
    }

    bool SoraConfigParser::hasAttribute(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->hasAttribute(attr);
    }
    
    std::vector<std::string> SoraConfigParser::getAttributes() const {
        sora_assert(mImpl);
        return mImpl->getAttributes();
    }
    
    std::string SoraConfigParser::getString(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getString(attr);
    }
    
    std::wstring SoraConfigParser::getWString(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getWString(attr);
    }
    
    bool SoraConfigParser::getBool(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getBool(attr);
    }
    
    int32 SoraConfigParser::getInt(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getInt(attr);
    }
    
    float SoraConfigParser::getFloat(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getFloat(attr);
    }
    
    std::string SoraConfigParser::getString(const std::string& attr, const std::string& opt) const {
        sora_assert(mImpl);
        return mImpl->getString(attr, opt);
    }
    
    std::wstring SoraConfigParser::getWString(const std::string& attr, const std::wstring& opt) const {
        sora_assert(mImpl);
        return mImpl->getWString(attr, opt);
    }
    
    bool SoraConfigParser::getBool(const std::string& attr, bool opt) const {
        sora_assert(mImpl);
        return mImpl->getBool(attr, opt);
    }
    
    int32 SoraConfigParser::getInt(const std::string& attr, int32 opt) const {
        sora_assert(mImpl);
        return mImpl->getInt(attr, opt);
    }
    
    float SoraConfigParser::getFloat(const std::string& attr, float opt) const {
        sora_assert(mImpl);
        return mImpl->getFloat(attr, opt);
    }
    
    bool SoraConfigParser::beginNode(const std::string& nodeName) {
        sora_assert(mImpl);
        return mImpl->beginNode(nodeName);
    }
    
    void SoraConfigParser::endNode() {
        sora_assert(mImpl);
        return mImpl->endNode();
    }
    
    void SoraConfigParser::setString(const std::string& attr, const std::string& val) {
        sora_assert(mImpl);
        return mImpl->setString(attr, val);
    }
    
    void SoraConfigParser::setBool(const std::string& attr, bool val) {
        sora_assert(mImpl);
        return mImpl->setBool(attr, val);
    }
    
    void SoraConfigParser::setInt(const std::string& attr, int32 val) {
        sora_assert(mImpl);
        return mImpl->setInt(attr, val);
    }
    
    void SoraConfigParser::setFloat(const std::string& attr, float val) {
        sora_assert(mImpl);
        return mImpl->setFloat(attr, val);
    }
    
} // namespace sora