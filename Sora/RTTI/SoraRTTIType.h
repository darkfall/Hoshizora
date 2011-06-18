/*
 *  SoraRTTITyp.h
 *  PartitionTree
 *
 *  Created by Griffin Bu on 5/22/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_RTTI_TYPE_H_
#define SORA_RTTI_TYPE_H_

#include <iostream>
#include <string>

#include "SoraRTTIException.h"

namespace sora {

#ifdef WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef int64_t int64;
typedef uint64_t uint64;
#endif
	
	typedef enum {
		RTTI_FLAG_VIRTUAL	= 1,
		RTTI_FLAG_STATIC	= 2,
		RTTI_FLAG_PUBLIC	= 4,
		RTTI_FLAG_PROTECTED = 8,
		RTTI_FLAG_PRIVATE	= 16,
		
		// SoraRTTI interval flag
		RTTI_FLAG_OVERLOADED = 1024,
	};
	
	static std::string RTTI_FLAG_TO_STR(int flag) {
		std::string strDecl;
		
		if(flag & RTTI_FLAG_PUBLIC)
			strDecl += "public: ";
		if(flag & RTTI_FLAG_PRIVATE)
			strDecl += "private: ";
		if(flag & RTTI_FLAG_PROTECTED)
			strDecl += "protected: ";
		
		if(flag & RTTI_FLAG_VIRTUAL)
			strDecl += "virtual ";
		if(flag & RTTI_FLAG_STATIC)
			strDecl += "static ";
		return strDecl;
	}
	
	typedef enum {
		RTTI_UNKNOWN,
		
		RTTI_VOID, // void
		RTTI_BOOL, // bool
		
		RTTI_INT8,  // char, int8, byte
		RTTI_UINT8, // uchar, uint8, ubyte
		
		RTTI_INT16, // int16, short
		RTTI_UINT16, // uint16, ushort
		
		RTTI_INT32, // int32, int
		RTTI_UINT32, // uint32, uint
		
		RTTI_LONG32, // long32, long
		RTTI_ULONG32, // ulong32, ulong
		
		RTTI_INT64, // long long, _int64
		RTTI_UINT64, // unsigned long long, _uint64 
		
		RTTI_FLOAT, // float
		RTTI_DOUBLE, // double
		
		RTTI_CLASS, // struct, class
		RTTI_DERIVED, // derived class
		
		RTTI_PTR, // pointer
		RTTI_ARRAY, // a array
		
		RTTI_METHOD, // class method
		
		RTTI_STRING, // std::string
		RTTI_WSTRING,  // std::wstring
		
		RTTI_CONSTRUCTOR, // a class constructor
	};
	
	class SoraRTTIType {
	public:
		static SoraRTTIType unknownType;
		static SoraRTTIType voidType;
		static SoraRTTIType boolType;
		static SoraRTTIType int8Type;
		static SoraRTTIType uint8Type;
		static SoraRTTIType int16Type;
		static SoraRTTIType uint16Type;
		static SoraRTTIType int32Type;
		static SoraRTTIType uint32Type;
		static SoraRTTIType long32Type;
		static SoraRTTIType ulong32Type;
		static SoraRTTIType int64Type;
		static SoraRTTIType uint64Type;
		static SoraRTTIType floatType;
		static SoraRTTIType doubleType;
		static SoraRTTIType stringType;
		static SoraRTTIType wstringType;
		
		bool isInternal() const {
			return RTTITag <= RTTI_DOUBLE;
		}
		
		bool isPointer() const {
			return RTTITag == RTTI_PTR;
		}
		
		bool isArray() const {
			return RTTITag == RTTI_ARRAY;
		}
		
		bool isString() const {
			return RTTITag == RTTI_STRING || RTTITag == RTTI_WSTRING;
		}
		
		// check whether the type is a class
		bool isClass() const {
			return RTTITag == RTTI_CLASS;
		}
		
		// check whether the class is a dervied class 
		bool isDerived() const {
			return RTTITag == RTTI_DERIVED;
		}
		
		bool isMethod() const {
			return RTTITag == RTTI_METHOD;
		}
		
		int getTag() const {
			return RTTITag;
		}
		
		// get the representive string of the type
		virtual std::string getName();
		
		SoraRTTIType(int tag): RTTITag(tag) {}
		
	protected:
		int RTTITag;
		std::string RTTIName;
		
		SoraRTTIType(SoraRTTIType* type): RTTITag(type->getTag()) {}
	};
	
	class SoraRTTIArrayType: public SoraRTTIType {
	public:
		SoraRTTIArrayType(SoraRTTIType* arrayType, unsigned int size): SoraRTTIType(RTTI_ARRAY),
			mArrayType(arrayType),
			mArraySize(size) {
		}
		
		virtual std::string getName();
		
		unsigned int getArraySize() const;
		SoraRTTIType* getArrayType() const;
		
	private:
		unsigned int mArraySize;
		SoraRTTIType* mArrayType;
	};
	
	class SoraRTTIPtrType: public SoraRTTIType {
	public:
		SoraRTTIPtrType(SoraRTTIType* pointeeType): SoraRTTIType(RTTI_PTR),
			mPointeeType(pointeeType) {
		}
		
		virtual std::string getName();
		
		SoraRTTIType* getPointeeType() const;
		
	private:
		SoraRTTIType* mPointeeType;
	};
	
} // namespace sora

#endif // SORA_RTTI_TYPE_H_