/*
 *  SoraNamedObject.h
 *  Sora
 *
 *  Created by Griffin Bu on 2/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_ASSM_NAMED_OBJECT_H_
#define _SORA_ASSM_NAMED_OBJECT_H_

#include "stringId.h"
#include "common.h"

#include <list>
#include <algorithm>

#include "SoraMemoryBuffer.h"
#include "Serializable.h"
#include <cassert>

namespace sora {
	
#define MAX_NAME_LENGTH 1024
	
	/*
	 Base class for all objects that have a name
	 */
	class SoraNamedObject: public Serializable {
	public:
		SoraNamedObject(): name(0) {}
		SoraNamedObject(const SoraString& _name): name(str2id(_name)) {}
		SoraNamedObject(stringId sid): name(sid) {}
		virtual ~SoraNamedObject() {}
		
		void setName(stringId n) { name = n; }
		stringId getName() const { return name; }
		
		virtual void serialize(SoraStream& bufferStream) {
            std::string strData = id2str(name).c_str();
			bufferStream<<strData;
		}
		virtual void unserialize(SoraStream& bufferStream) {
            std::string strData;
            bufferStream>>strData;
            setName(str2id(strData));
		}
		
		// be default
		// a named object is serializable
		// serialized file is length(4 bytes) + name(length)
		virtual bool serializable() { return true; }
		
	protected:
		stringId name;
	};
	
	class SoraNamedObjectList: public Serializable {
	public:
		typedef std::list<SoraNamedObject*> OBJ_LIST;

		SoraNamedObjectList() {}
		virtual ~SoraNamedObjectList() { for_each(objList.begin(), objList.end(), DeleteSTLPtr()); }
		
		void addObject(SoraNamedObject* obj) { objList.push_back(obj); }
		void delObject(SoraNamedObject* obj) { objList.remove(obj); }
		
		SoraNamedObject* getObjectByName(stringId name) {
			OBJ_LIST::iterator itObj = std::find_if(objList.begin(), objList.end(), std::bind2nd(compareObjectName(), name));
			if(itObj != objList.end())
				return *itObj;
			return NULL;
		}
		
		void removeObjectByName(stringId name) {
			OBJ_LIST::iterator itPos = std::remove_if(objList.begin(), objList.end(), std::bind2nd(compareObjectName(), name));
			for_each(itPos,
					 objList.end(),
					 DeleteSTLPtr());
			objList.erase(itPos, objList.end());
		}
		
		// STL compability defines and functions
		typedef OBJ_LIST::iterator Iterator;
		typedef OBJ_LIST::iterator iterator;

		Iterator begin() { return objList.begin(); }
		Iterator end()   { return objList.end(); }
		
		SoraNamedObject* back() { return objList.back(); }
		SoraNamedObject* front() { return objList.front(); }
		
		void push_back(SoraNamedObject* obj) { objList.push_back(obj); }
		void push_front(SoraNamedObject* obj) { objList.push_front(obj); }
		void insert(Iterator itWhere, SoraNamedObject* obj) { objList.insert(itWhere, obj); }
		void remove(SoraNamedObject* obj) { objList.remove(obj); }
		void erase(Iterator it) { objList.erase(it); }
		
		size_t size() { return objList.size(); }
		size_t count() { return objList.size(); }
		
		void serialize(SoraMemoryBuffer& memoryStream) {}
		void unserialize(SoraMemoryBuffer& memoryStream) {}
		bool serializable() { return false; }
		
	private:
		struct compareObjectName: public std::binary_function<SoraNamedObject*, stringId, bool> {
			bool operator() (SoraNamedObject* obj, stringId name) const {
				return name == obj->getName();
			}
		};
		
		OBJ_LIST objList;
	};
		
	
} // namespace sora


#endif // _SORA_ASSM_NAMED_OBJECT_H_