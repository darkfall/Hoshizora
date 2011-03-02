#ifndef SORA_GUI_LISTENER_H
#define SORA_GUI_LISTENER_H

#include <guichan.hpp>

#include "SoraCore.h"

#include "SoraGUIResponserMap.h"

namespace sora {

	class SoraGUIActionListener: public gcn::ActionListener {
	public:
		void action(const gcn::ActionEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->action();
			}
		}
	};
	class SoraLuaGUIActionListener: public gcn::ActionListener {
	public:
		void action(const gcn::ActionEvent& ev) {
			/*int result = SoraCore::Instance()->doLuaFile(SoraStringConverter::string2WideString(
				SoraGUIResponserMap::Instance()->getLuaResponser((ulong32)ev.getSource()))
				);
			if(result == -1) throw SORA_EXCEPTION("Error loading lua file: "+SoraGUIResponserMap::Instance()->getLuaResponser((ulong32)ev.getSource()));
			LuaObject o = SoraCore::Instance()->getMainLuaState()->GetGlobals()["onClick"];
			if(o.IsFunction()) {
				LuaFunction<void> func = o;
				func(ev.getSource()->getId());
			}*/
		}
	};
	class SoraGUIKeyListener: public gcn::KeyListener {
		void keyPressed(const gcn::KeyEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->keyDown(ev);
			}
		}
   
		void keyReleased(const gcn::KeyEvent& ev) {
		    SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->keyUp(ev);
			}
		}
	};
	class SoraGUIDeathListener: public gcn::DeathListener {
		void death(const gcn::Event& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->death();
			}
		}
	};
	class SoraGUIMouseListener: public gcn::MouseListener {
		void mousePressed(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mousePressed(ev);
			}
		}
		void mouseReleased(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseReleased(ev);
			}
		}
		void mouseClicked(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseClicked(ev);
			}
		}
		void mouseMoved(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseMoved(ev);
			}
		}
		void mouseEntered(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseEntered(ev);
			}
		}
		void mouseExited(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseExited(ev);
			}
		}
		void mouseWheelMovedDown(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseWheelMovedDown(ev);
			}
		}
		void mouseWheelMovedUp(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseWheelMovedUp(ev);
			}
		}
		void mouseDragged(const gcn::MouseEvent& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->mouseDragged(ev);
			}
		}
	};
	class SoraGUIFocusListener: public gcn::FocusListener {
		void focusGained(const gcn::Event& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->focusGained();
			}
		}

		void focusLost(const gcn::Event& ev) {
			SoraGUIResponser* pResponser = SoraGUIResponserMap::Instance()->getResponser(ev.getSource()->getId());
			if(pResponser) {
				pResponser->setID(ev.getSource()->getId());
				pResponser->setSource(ev.getSource());
				pResponser->focusLost();
			}
		}
	};

} // namespace sora

#endif