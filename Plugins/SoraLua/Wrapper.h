#ifndef WRAPPER_H
#define WRAPPER_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include "SoraSprite.h"
#include "SoraCore.h"

namespace sora {

	namespace spritewrapper {

		using namespace sora;

		void setTextureRect	(HSORASPRITE h, float32 x, float32 y, float32 w, float32 fh);
		void setColor		(HSORASPRITE h, ulong32 c, int32 i=-1);
		void setZ			(HSORASPRITE h, float32 z, int32 i=-1);
		ulong32 getColor	(HSORASPRITE h, int32 i=-1);
		float32 getZ		(HSORASPRITE h, int32 i=-1);
		void setCenter		(HSORASPRITE h, float32 x, float32 y);
		float32 getCenterX	(HSORASPRITE h);
		float32 getCenterY	(HSORASPRITE h);
		void setFlip		(HSORASPRITE h, bool hf, bool vf);
		bool getVFlip		(HSORASPRITE h);
		bool getHFlip		(HSORASPRITE h);

		int32 getTextureWidth		(HSORASPRITE h);
		int32 getTextureHeight		(HSORASPRITE h);
		int32 getSpriteWidth		(HSORASPRITE h);
		int32 getSpriteHeight		(HSORASPRITE h);
		void setScale				(HSORASPRITE h, float32 vs, float32 hs);
		float32 getVScale			(HSORASPRITE h);
		float32 getHScale			(HSORASPRITE h);
		void setRotation			(HSORASPRITE h, float32 r);
		float32 getRotation			(HSORASPRITE h);
		ulong32 getPixel			(HSORASPRITE h, int32 x, int32 y);
		void setPixel				(HSORASPRITE h, ulong32 c, int32 x, int32 y);

		void setBlendMode		(HSORASPRITE h, int32 mode);
		int32 getBlendMode		(HSORASPRITE h);

		HSORASPRITE gaussBlurNew (HSORASPRITE h, double32 sigma, int32 radius);
		void gaussBlur			 (HSORASPRITE h, double32 sigma, int32 radius);
		HSORASPRITE grayNew	 	 (HSORASPRITE h);
		void gray				 (HSORASPRITE h);
		HSORASPRITE reverseNew 	 (HSORASPRITE h);
		void reverse			 (HSORASPRITE h);
		HSORASPRITE alphaMixNew	 (HSORASPRITE h1, HSORASPRITE h2, double32 a, int32 posx, int32 posy);
		void alphaMix			 (HSORASPRITE h1, HSORASPRITE h2, double32 a, int32 posx, int32 posy);

		void release (HSORASPRITE h, bool bDeleteTexture);
	} // namespace spritewrapper
	 
	/*namespace guiwrapper {

		using namespace sora;

		void setWidgetFrameSize(ulong32 h, uint32 framesize);
		uint32 getWidgetFrameSize(ulong32 h);
		
		ulong32 getWidgetParent(ulong32 h);

		void setWidgetWidth(ulong32 h, int32 w);
		void setWidgetHeight(ulong32 h, int32 ht);
		void setWidgetSize(ulong32 h, int32 w, int32 ht);
		int32 getWidgetWidth(ulong32 h);
		int32 getWidgetHeight(ulong32 h);
		void adjustWidgetSize(ulong32 h);

		void setWidgetX(ulong32 h, int32 x);
		void setWidgetY(ulong32 h, int32 y);
		void setWidgetPos(ulong32 h, int32 x, int32 y);
		int32 getWidgetX(ulong32 h);
		int32 getWidgetY(ulong32 h);

		void setWidgetFocusable(ulong32 h, bool focusable);
		bool isWidgetFocusable(ulong32 h);
		bool isWidgetFocused(ulong32 h);

		void setWidgetVisible(ulong32 h, bool visible);
		bool isWidgetVisible(ulong32 h);

		void setWidgetBaseColor(ulong32 h, int32 c);
		void setWidgetForegroundColor(ulong32 h, int32 c);
		void setWidgetBackgroundColor(ulong32 h, int32 c);
		void setWidgetSelectionColor(ulong32 h, int32 c);

		void setWidgetFont(ulong32 h, const SoraWString& fontName, int32 fontSize);

		void requestWidgetFocus(ulong32 h);
		void requestWidgetModalFocus(ulong32 h);
		void requestWidgetModalMouseInputFocus(ulong32 h);
		void releaseWidgetModalFocus(ulong32 h);
		void releaseWidgetModalMouseInputFocus(ulong32 h);
		bool isWidgetModalFocused(ulong32 h);
		bool isWidgetModalMouseInputFocused(ulong32 h);
		
		ulong32 getWidgetAt(ulong32 h, int32 x, int32 y);

		void setWidgetTabInEnabled(ulong32 h, bool flag);
		void setWidgetTabOutEnabled(ulong32 h, bool flag);
		bool isWidgetTabInEnabled(ulong32 h);
		bool isWidgetTabOutEnabled(ulong32 h);

		void setWidgetId(ulong32 h, const SoraString& str);
		SoraString getWidgetId(ulong32 h);
		void showWidgetPart(ulong32 h, int32 x, int32 y, int32 wd, int32 ht);

		bool canWidgetSetCaption(gcn::Widget* Widget, const SoraString& caption);
		bool canWidgetSetValue(gcn::Widget* Widget, float32 val);
		bool canWidgetSetOpaque(gcn::Widget* widget, bool flag);

		void setWidgetOpaque(ulong32 h, bool flag);
		bool isWidgetOpaque(ulong32 h);
		
		void setWidgetCaption(ulong32 h, const SoraString& caption);
		SoraString getWidgetCaption(ulong32 h);

		void setWidgetValue(ulong32 h, float32 val);
	} // namespace guiwrapper
	*/
	namespace imageeffectwrapper {

		ulong32 makeEffect_fade(float32 src, float32 dst, float32 time, int32 mode);
		ulong32 makeEffect_transitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 time, int32 mode);
		ulong32 makeEffect_scale_tensile(float32 sv, float32 dv, float32 sh, float32 dh, float32 time, int32 mode);
		ulong32 makeEffect_scale(float32 s, float32 d, float32 time, int32 mode);

	} // namespace iamgeeffectwrapper
}

#endif