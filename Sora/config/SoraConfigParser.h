//
//  SoraConfigurationParser.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConfigParser_h
#define Sora_SoraConfigParser_h

#include "prerequisites.h"

#include <vector>

namespace sora {
    
    class SoraConfigParserImpl;
    
    class SoraConfigParser: uncopyable {
    public:
        SoraConfigParser();
        ~SoraConfigParser();
        
        bool open(const std::wstring& path, ConfigType type);
        bool open(void* data, ulong32 size, ConfigType type);
        bool open(const std::string& string, ConfigType type);
        bool create();
        bool writeToFile(const std::wstring& path);
        std::string writeToString() const;
        
        void close();
        
        bool hasNode(const std::string& node) const;
        std::string getCurrentNodeName() const;
        std::string getCurrentNodePath() const;
        
        /**
         * set current node to a node, / for root, otherwise use relative path
         * /config/window/ etc
         **/
        bool toNode(const std::string& node);
        
        /**
         * set current node to first node child
         **/
        bool toFirstChild(const std::string& name=std::string());
        /**
         * set current node to next slibling child
         **/
        bool toNextChild(const std::string& name=std::string());
        /**
         * set current node to node parent
         **/
        bool toParent();
        
        
        bool hasAttribute(const std::string& attr) const;
        /**
         * get all attributes of current node
         **/
        std::vector<std::string> getAttributes() const;
        
        /**
         * get current node as string
         */
        std::string getString(const std::string& attr) const;
        /**
         * get current node as bool
         */
        std::wstring getWString(const std::string& attr) const;
        /**
         * get current node as bool
         */
        bool getBool(const std::string& attr) const;
        /**
         * get current node as bool
         */
        int32 getInt(const std::string& attr) const;
        /**
         * get current node as bool
         */
        float getFloat(const std::string& attr) const;
        
        /**
         * get current node as optinal string
         */
        std::string getString(const std::string& attr, const std::string& opt) const;
        /**
         * get current node as optinal bool
         */
        std::wstring getWString(const std::string& attr, const std::wstring& opt) const;
        /**
         * get current node as optinal bool
         */
        bool getBool(const std::string& attr, bool opt) const;
        /**
         * get current node as optinal bool
         */
        int32 getInt(const std::string& attr, int32 opt) const;
        /**
         * get current node as optinal bool
         */
        float getFloat(const std::string& attr, float opt) const;
        
        
        /**
         * Begin a new node under current node
         **/
        bool beginNode(const std::string& nodeName);
        void endNode();
        
        /**
         * set a string attribute under current node
         **/
        void setString(const std::string& attr, const std::string& val);
        /**
         * set a bool attribute under current node
         **/
        void setBool(const std::string& attr, bool val);
        /**
         * set a int attribute under current node
         **/
        void setInt(const std::string& attr, int32 val);
        /**
         * set a long attribute under current node
         **/
        void setLong(const std::string& attr, long32 val);
        /**
         * set a float attribute under current node
         **/
        void setFloat(const std::string& attr, float val);
        
    private:
        SoraConfigParserImpl* mImpl;
    };
    
    
} // namespace sora



#endif
