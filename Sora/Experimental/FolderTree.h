/*
 *  FolderTree.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef FOLDER_TREE_H_
#define FOLDER_TREE_H_

#include "SoraPlatform.h"
#include "SoraException.h"

namespace sora {
	
	class FolderTree {
	public:
		FolderTree();
		~FolderTree();
		
		void add(const SoraWString& fileName);
		void del(const SoraWString& fileName);
		
		
	};
	
} // namespace sora


#endif