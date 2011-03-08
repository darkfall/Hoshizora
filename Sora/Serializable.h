/*
 *  Serializable.h
 *  Sora
 *
 *  Created by Griffin Bu on 3/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SERIALIZABLE_H__
#define SORA_SERIALIZABLE_H__

namespace sora {
	
	class SoraMemoryBuffer;
	
	class Serializable {
	public:
		virtual void serialize(SoraMemoryBuffer& bufferStream) = 0;
		virtual void unserialize(SoraMemoryBuffer& bufferStream) = 0;
		virtual bool serializable() = 0;
	};
	
} // namespace sora

#endif