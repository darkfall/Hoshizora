/*
 *  ModifierBase.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GNC_MODIFIER_BASE_H_
#define SORA_GNC_MODIFIER_BASE_H_

namespace gcn {
	
	class Widget;
	
	/*
	 * Represents a modifier that modify the contents of a widget
	 * The modifier would be automatically deleted if isFinished() == true
	 */
		
	class Modifier {
	public:
		Modifier(): mFinished(false) {}
		
		virtual void update(Widget* widget) = 0;
	
		bool isFinished() const { return mFinished; }

	protected:
		bool mFinished;
	};
		
} // namespace gcn
		
#endif // SORA_MODIFIER_BASE_H_