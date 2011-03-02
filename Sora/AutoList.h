/*
 *  AutoList.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_AUTO_LIST_H_
#define SORA_AUTO_LIST_H_

#include <list>

namespace sora {
	
	/*
	 inheritant objects from this to create a obj list,
	 whenever the object is created, it will be add to the list
	 whenever the object is destroyed, it will deleted from the list
	 */
	
	template<typename T>
	class AutoListElement {
	public:
		typedef std::list<T*> ObjList;
	
	private:
		
		static ObjList members;
		
	protected:
		
		AutoListElement() {
			members.push_back(static_cast<T*> (this));
		}
											  
		
		~AutoListElement() {
			members.remove(static_cast<T*> (this));
		}
									
	public:
		
		static ObjList& getAllMembers() const { return members; }
	};
	
	template<T>
	std::list<T*> AutoListElement<T>::members;
	
	
} // namespace sora


#endif