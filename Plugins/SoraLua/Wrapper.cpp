#include "Wrapper.h"

#include "SoraStringConv.h"
#include "SoraGraphicEffect.h"
#include "SoraImageEffect.h"
#include "SoraRenderObject.h"

namespace sora {

	namespace spritewrapper {
		void setTextureRect(HSORASPRITE h, float32 x, float32 y, float32 w, float32 fh) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setTextureRect(x, y, w, fh);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		void setColor(HSORASPRITE h, ulong32 c, int32 i) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setColor(c, i);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		void setZ(HSORASPRITE h, float32 z, int32 i) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setZ(z, i);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		ulong32 getColor(HSORASPRITE h, int32 i) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getColor(i);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		float32 getZ(HSORASPRITE h, int32 i) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getZ(i);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void setCenter(HSORASPRITE h, float32 x, float32 y) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setCenter(x, y);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		float32 getCenterX(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getCenterX();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0.f;
		}

		float32 getCenterY(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getCenterY();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0.f;
		}

		void setFlip(HSORASPRITE h, bool hf, bool vf) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setFlip(hf, vf);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		bool getVFlip(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getVFlip();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return false;
		}

		bool getHFlip(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getHFlip();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return false;
		}

		int32 getTextureWidth(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getTextureWidth();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		int32 getTextureHeight(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getTextureHeight();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		int32 getSpriteWidth(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getSpriteWidth();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		int32 getSpriteHeight(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getSpriteHeight();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void setScale(HSORASPRITE h, float32 vs, float32 hs) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setScale(vs, hs);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		float32 getVScale(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getVScale();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0.f;
		}

		float32 getHScale(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getHScale();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0.f;
		}

		void setRotation(HSORASPRITE h, float32 r) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setRotation(r);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		float32 getRotation(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getRotation();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0.f;
		}

		ulong32 getPixel(HSORASPRITE h, int32 x, int32 y) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getPixel(x, y);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void setPixel(HSORASPRITE h, ulong32 c, int32 x, int32 y) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setPixel(c, x, y);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		HSORASPRITE gaussBlurNew(HSORASPRITE h, double32 sigma, int32 radius) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return (HSORASPRITE)SoraGraphicEffect::gaussBlurNew(p, sigma, radius);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void gaussBlur(HSORASPRITE h, double32 sigma, int32 radius) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) SoraGraphicEffect::gaussBlur(p, sigma, radius);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		void setBlendMode(HSORASPRITE h, int32 mode) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) p->setBlendMode(mode);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		int32 getBlendMode(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return p->getBlendMode();
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		HSORASPRITE grayNew(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return (HSORASPRITE)SoraGraphicEffect::grayNew(p);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void gray(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) SoraGraphicEffect::gray(p);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		HSORASPRITE reverseNew(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) return (HSORASPRITE)SoraGraphicEffect::reverseNew(p);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void reverse(HSORASPRITE h) {
			SoraSprite* p = (SoraSprite*)(h);
			if(p) SoraGraphicEffect::reverse(p);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		HSORASPRITE alphaMixNew(HSORASPRITE h1, HSORASPRITE h2, double32 a, int32 posx, int32 posy) {
			SoraSprite* p = (SoraSprite*)h1;
			SoraSprite* p2 = (SoraSprite*)h2;
			if(p && p2) return (HSORASPRITE)SoraGraphicEffect::alphaMixNew(p, p2, a, posx, posy);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
			return 0;
		}

		void alphaMix(HSORASPRITE h1, HSORASPRITE h2, double32 a, int32 posx, int32 posy) {
			SoraSprite* p = (SoraSprite*)h1;
			SoraSprite* p2 = (SoraSprite*)h2;
			if(p && p2) SoraGraphicEffect::alphaMix(p, p2, a, posx, posy);
			else
				throw SORA_EXCEPTION("Invalid sprite arg");
		}

		void release(HSORASPRITE h, bool bDeleteTexture) {
			SoraSprite* p = (SoraSprite*)h;
			if(p) {
				delete p;
				p = 0;
			}
		}
	} // namespace spritewrapper

	/*
	namespace guiwrapper {
		inline gcn::Widget* getWidgetByHandle(ulong32 h) {
			return (gcn::Widget*)h;
		}
		inline ulong32 getWidgetHandleByWidget(gcn::Widget* w) {
			return (ulong32)w;
		}

		void setWidgetFrameSize(ulong32 h, uint32 framesize) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setFrameSize(framesize);
		}
		
		uint32 getWidgetFrameSize(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getFrameSize();
			return 0;
		}
		
		ulong32 getWidgetParent(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return getWidgetHandleByWidget(w->getParent());
			return 0;
		}

		void setWidgetWidth(ulong32 h, int32 wd) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setWidth(wd);
		}

		void setWidgetHeight(ulong32 h, int32 ht) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setHeight(ht);
		}

		void setWidgetSize(ulong32 h, int32 wd, int32 ht) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setSize(wd, ht);
		}

		int32 getWidgetWidth(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getWidth();
			return 0;
		}

		int32 getWidgetHeight(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getHeight();
			return 0;
		}

		void setWidgetX(ulong32 h, int32 x) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setX(x);
		}

		void setWidgetY(ulong32 h, int32 y) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setY(y);
		}

		void setWidgetPos(ulong32 h, int32 x, int32 y) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setPosition(x, y);
		}

		int32 getWidgetX(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getX();
			return 0;
		}

		int32 getWidgetY(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getY();
			return 0;
		}

		void setWidgetFocusable(ulong32 h, bool focusable) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setFocusable(focusable);
		}

		bool isWidgetFocusable(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isFocusable();
			return false;
		}

		bool isWidgetFocused(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isFocused();
			return false;
		}

		void setWidgetVisible(ulong32 h, bool visible) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setVisible(visible);
		}

		bool isWidgetVisible(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isVisible();
			return false;
		}

		void setWidgetBaseColor(ulong32 h, int32 c) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setBaseColor(gcn::Color(c));
		}

		void setWidgetForegroundColor(ulong32 h, int32 c) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setForegroundColor(gcn::Color(c));
		}

		void setWidgetBackgroundColor(ulong32 h, int32 c) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setBackgroundColor(gcn::Color(c));
		}

		void setWidgetSelectionColor(ulong32 h, int32 c) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setSelectionColor(gcn::Color(c));
		}

		void setWidgetFont(ulong32 h, const SoraWString& fontName, int32 fontSize) {
			gcn::Widget* w = getWidgetByHandle(h);
		}

		void requestWidgetModalFocus(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->requestModalFocus();
		}

		void requestWidgetModalMouseInputFocus(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->requestModalMouseInputFocus();
		}

		void requestWidgetFocus(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->requestFocus();
		}

		void releaseWidgetModalFocus(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->releaseModalFocus();
		}

		void releaseWidgetModalMouseInputFocus(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->releaseModalMouseInputFocus();
		}

		bool isWidgetModalFocused(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isModalFocused();
			return false;
		}

		bool isWidgetModalMouseInputFocused(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isModalMouseInputFocused();
			return false;
		}
		
		ulong32 getWidgetAt(ulong32 h, int32 x, int32 y) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return getWidgetHandleByWidget(w->getWidgetAt(x, y));
			return 0;
		}

		void setWidgetTabInEnabled(ulong32 h, bool flag) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setTabInEnabled(flag);
		}

		void setWidgetTabOutEnabled(ulong32 h, bool flag) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setTabOutEnabled(flag);
		}

		bool isWidgetTabInEnabled(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isTabInEnabled();
			return false;
		}

		bool isWidgetTabOutEnabled(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->isTabOutEnabled();
			return false;
		}

		void setWidgetId(ulong32 h, const SoraString& str) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->setId(str);
		}

		SoraString getWidgetId(ulong32 h) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) return w->getId();
			return "\0";
		}

		void showWidgetPart(ulong32 h, int32 x, int32 y, int32 wd, int32 ht) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) w->showPart(gcn::Rectangle(x, y, wd, ht));
		}

		bool canWidgetSetCaption(gcn::Widget* widget, const SoraString& caption) {
			gcn::Button* b = dynamic_cast<gcn::Button*>(widget);
			if(b) {
				b->setCaption(caption);
				return true;
			}

			gcn::Window* w = dynamic_cast<gcn::Window*>(widget);
			if(w) {
				w->setCaption(caption);
				return true;
			}

			gcn::CheckBox* cb = dynamic_cast<gcn::CheckBox*>(widget);
			if(cb) {
				cb->setCaption(caption);
				return true;
			}

			gcn::Label* l = dynamic_cast<gcn::Label*>(widget);
			if(l) {
				l->setCaption(SoraStringConverter::string2WideString(caption));
				return true;
			}

			return false;
		}

		bool canWidgetSetValue(gcn::Widget* widget, float32 val) {
			gcn::Slider* s = dynamic_cast<gcn::Slider*>(widget);
			if(s) {
				s->setValue(val);
				return true;
			}

			return false;
		}

		bool canWidgetSetOpaque(gcn::Widget* widget, bool flag) {
			gcn::Container* c = dynamic_cast<gcn::Container*>(widget);
			if(c) {
				c->setOpaque(flag);
				return true;
			}

			gcn::Window* w = dynamic_cast<gcn::Window*>(widget);
			if(w) {
				w->setOpaque(flag);
				return true;
			}

			gcn::TextBox* t = dynamic_cast<gcn::TextBox*>(widget);
			if(t) {
				t->setOpaque(flag);
				return true;
			}

			return false;
		}

		void setWidgetOpaque(ulong32 h, bool flag) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) canWidgetSetOpaque(w, flag);
		}

		bool isWidgetOpaque(ulong32 h) {
			return false;
		}
		
		void setWidgetCaption(ulong32 h, const SoraString& caption) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) canWidgetSetCaption(w, caption);
		}

		SoraString getWidgetCaption(ulong32 h) {
			return "\0";
		}

		void setWidgetValue(ulong32 h, float32 val) {
			gcn::Widget* w = getWidgetByHandle(h);
			if(w) canWidgetSetValue(w, val);
		}
	} // namespace guiwrapper
	*/
	namespace imageeffectwrapper {

		ulong32 makeEffect_fade(float32 src, float32 dst, float32 time, int32 mode) {
			return (ulong32)(new SoraImageEffectFade(src, dst, time, mode));
		}
		ulong32 makeEffect_transitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 time, int32 mode) {
			return (ulong32)(new SoraImageEffectTransitions(sx, sy, dx, dy, time, mode));
		}
		ulong32 makeEffect_scale_tensile(float32 sv, float32 dv, float32 sh, float32 dh, float32 time, int32 mode) {
			return (ulong32)(new SoraImageEffectScale(sv, dv, sh, dh, time, mode));
		}
		ulong32 makeEffect_scale(float32 s, float32 d, float32 time, int32 mode) {
			return (ulong32)(new SoraImageEffectScale(s, d, time, mode));
		}
	}
} // namespace sora