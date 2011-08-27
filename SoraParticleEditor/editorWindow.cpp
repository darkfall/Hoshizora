#include "editorWindow.h"

#include "peMainWindow.h"

gcn::Window* pSpeedPanel;
gcn::Window* pPropPanel;
gcn::Window* pLifetimePanel;
gcn::Window* pStartColorPanel;
gcn::Window* pEndColorPanel;

std::wstring sCurrParticleFile = L"Default.sps";

bool bOpenFinished = false;
bool bMouseDown = false;
bool bShowBoundingBox = false;

sora::SoraSprite* pspr;
sora::SoraParticleSystem* peffect;

sora::SoraSprite* pbgSpr = NULL;


#include "SoraGUIChan/xmlgui.h"
sora::XmlGui* pXmlParser;

gcn::Slider* slMinSpeed;
gcn::Slider* slMaxSpeed;
gcn::Slider* slMinLinearAccel;
gcn::Slider* slMaxLinearAccel;
gcn::Slider* slMinTrigAccel;
gcn::Slider* slMaxTrigAccel;
gcn::Slider* slMinLifetime;
gcn::Slider* slMaxLifetime;
gcn::Slider* slMinSpin;
gcn::Slider* slMaxSpin;
gcn::Slider* slMinGravity;
gcn::Slider* slMaxGravity;
gcn::Slider* slMinStartScale;
gcn::Slider* slMaxStartScale;
gcn::Slider* slMinEndScale;
gcn::Slider* slMaxEndScale;

gcn::Slider* slMinStartColorR;
gcn::Slider* slMinStartColorG;
gcn::Slider* slMinStartColorB;
gcn::Slider* slMinStartColorA;
gcn::Slider* slMaxStartColorR;
gcn::Slider* slMaxStartColorG;
gcn::Slider* slMaxStartColorB;
gcn::Slider* slMaxStartColorA;
gcn::Slider* slMinEndColorR;
gcn::Slider* slMinEndColorG;
gcn::Slider* slMinEndColorB;
gcn::Slider* slMinEndColorA;
gcn::Slider* slMaxEndColorR;
gcn::Slider* slMaxEndColorG;
gcn::Slider* slMaxEndColorB;
gcn::Slider* slMaxEndColorA;

gcn::Slider* slSpreadRangeX;
gcn::Slider* slSpreadRangeY;
gcn::Slider* slSpreadRangeZ;
gcn::Slider* slEmissionTime;
gcn::Slider* slEmissionSpeed;
gcn::Slider* slTextureRect;

gcn::Slider* slEmitPointX;
gcn::Slider* slEmitPointY;
gcn::Slider* slEmitPointZ;

gcn::CheckBox* cbSpeedPanel;
gcn::CheckBox* cbPropPanel;
gcn::CheckBox* cbStartColorPanel;
gcn::CheckBox* cbEndColorPanel;
gcn::CheckBox* cbLifetimePanel;
gcn::CheckBox* cbBlendMode;


gcn::Slider* slMinDirectionX;
gcn::Slider* slMinDirectionY;
gcn::Slider* slMinDirectionZ;

gcn::Slider* slMaxDirectionX;
gcn::Slider* slMaxDirectionY;
gcn::Slider* slMaxDirectionZ;

gcn::Slider* slMinAngle;
gcn::Slider* slMaxAngle;

void restoreValue();


void labelApplyValue(gcn::Label* l, const std::wstring& str, float32 val) {
	l->setCaption(sora::ws2s(str)+"("+sora::fp_to_str(val)+")");
}
void labelLoadValue();

typedef gcn::Slider* PSLIDER;
typedef gcn::Label* PLABEL;

using namespace sora;


#include "cmd/SoraConsole.h"
#include "SoraGUIChan/guichansetup.h"

class PEConsoleCmdHandler: public SoraEventHandler {
public:
	PEConsoleCmdHandler() {
		registerEventFunc(this, &PEConsoleCmdHandler::onConsoleEvent);
		sora::SoraConsole::Instance()->registerCmdHandler(this, "showbox");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "showgui");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "respawn");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "moveto");
	}
	
	void onConsoleEvent(SoraConsoleEvent* ev) {
		if(ev->getCmd().compare("showbox") == 0) {
			std::cout<<ev->getParams()<<std::endl;
			if(ev->getParams().compare("on") == 0) {
				bShowBoundingBox = true;
				ev->pushResult("bouding box show enabled");
			}
			else if(ev->getParams().compare("off") == 0) {
				bShowBoundingBox = false;
				ev->pushResult("bouding box show disabled");
			}
			
		} else if(ev->getCmd().compare("showgui") == 0) {
			if(ev->getParams().compare("on") == 0) {
				sora::GCN_GLOBAL->getTop()->setEnabled(true);
				sora::GCN_GLOBAL->getTop()->setVisible(true);
				ev->pushResult("gui show enabled");

			} else if(ev->getParams().compare("off") == 0) {
				sora::GCN_GLOBAL->getTop()->setEnabled(false);
				sora::GCN_GLOBAL->getTop()->setVisible(false);
				
				ev->pushResult("gui show disabled");
			}
		} else if(ev->getCmd().compare("moveto") == 0) {
			std::vector<std::string> params;
			sora::deliStr(params, ev->getParams(), ' ');
			if(params.size() == 2) {
				peffect->moveTo(atoi(params[0].c_str()), atoi(params[1].c_str()), 0.f);
			} else if(params.size() == 3) {
				peffect->moveTo(atoi(params[0].c_str()), atoi(params[1].c_str()), atoi(params[2].c_str()));
			}
		} else if(ev->getCmd().compare("respawn") == 0) {
			peffect->fire();
		} else if(ev->getCmd().compare("save") == 0) {
			if(ev->getParams().size() != 0) {
				peffect->saveScript(sora::s2ws(ev->getParams()));
			}
		}
	}
};


class SpeedPanelResponser: public SoraGUIResponser {
	void action() {
		PSLIDER p = (gcn::Slider*)getSource();
		if(getID().compare("MinSpeed") == 0) {
			peffect->pheader.fMinSpeed = p->getValue();
		} 
		else if(getID().compare("MaxSpeed") == 0) {
			peffect->pheader.fMaxSpeed = p->getValue();
		} 
		else if(getID().compare("MinLinearAccel") == 0) {
			peffect->pheader.fMinLinearAcc = p->getValue();
		} 
		else if(getID().compare("MaxLinearAccel") == 0) {
			peffect->pheader.fMaxLinearAcc = p->getValue();
		} 
		else if(getID().compare("MinTrigAccel") == 0) {
			peffect->pheader.fMinTrigAcc = p->getValue();
		} 
		else if(getID().compare("MaxTrigAccel") == 0) {
			peffect->pheader.fMaxTrigAcc = p->getValue();
		}
	}
};

class LifetimePanelResponser: public SoraGUIResponser {
	void action() {
		gcn::Slider* p = (gcn::Slider*)getSource();
		if(getID().compare("MinLifetime") == 0)
			peffect->pheader.fMinLifetime = p->getValue();
		else if(getID().compare("MaxLifetime") == 0)
			peffect->pheader.fMaxLifetime = p->getValue();
		else if(getID().compare("EmissionTime") == 0)
			peffect->pheader.fEmitLifetime = p->getValue();
		else if(getID().compare("EmissionSpeed") == 0)
			peffect->pheader.nEmitNum = p->getValue();
//		labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
	}
};

class PropPanelResponser: public SoraGUIResponser {
	void action() {
		gcn::Slider* p = (gcn::Slider*)getSource();
		if(getID().compare("MinDirectionX") == 0)
			peffect->pheader.minDirection.x = p->getValue();
		else if(getID().compare("MinDirectionY") == 0)
			peffect->pheader.minDirection.y = p->getValue();
		else if(getID().compare("MinDirectionZ") == 0)
			peffect->pheader.minDirection.z = p->getValue();
		else if(getID().compare("MaxDirectionX") == 0)
			peffect->pheader.maxDirection.x = p->getValue();
		else if(getID().compare("MaxDirectionY") == 0)
			peffect->pheader.maxDirection.y = p->getValue();
		else if(getID().compare("MaxDirectionZ") == 0)
			peffect->pheader.maxDirection.z = p->getValue();
		else if(getID().compare("MinSpin") == 0)
			peffect->pheader.fMinSpin = p->getValue();
		else if(getID().compare("MaxSpin") == 0)
			peffect->pheader.fMaxSpin = p->getValue();
		else if(getID().compare("MinGravity") == 0)
			peffect->pheader.fMinGravity = p->getValue();
		else if(getID().compare("MaxGravity") == 0)
			peffect->pheader.fMaxGravity = p->getValue();
		else if(getID().compare("MinStartScale") == 0)
			peffect->pheader.fMinStartScale = p->getValue();
		else if(getID().compare("MaxStartScale") == 0)
			peffect->pheader.fMaxStartScale = p->getValue();
		else if(getID().compare("MinEndScale") == 0)
			peffect->pheader.fMinEndScale = p->getValue();
		else if(getID().compare("MaxEndScale") == 0)
			peffect->pheader.fMaxEndScale = p->getValue();
		else if(getID().compare("EmitRangeX") == 0)
			peffect->pheader.emitRange.x = p->getValue();
		else if(getID().compare("EmitRangeY") == 0)
			peffect->pheader.emitRange.y = p->getValue();
		else if(getID().compare("EmitRangeZ") == 0)
			peffect->pheader.emitRange.z = p->getValue();
		else if(getID().compare("EmitPointX") == 0)
			peffect->pheader.emitPos.x = p->getValue();
		else if(getID().compare("EmitPointY") == 0)
			peffect->pheader.emitPos.y = p->getValue();
		else if(getID().compare("EmitPointZ") == 0)
			peffect->pheader.emitPos.z = p->getValue();
		else if(getID().compare("MinAngle") == 0)
			peffect->pheader.fMinAngle = p->getValue();
		else if(getID().compare("MaxAngle") == 0)
			peffect->pheader.fMaxAngle = p->getValue();
	//	labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
	}
};

class ColorPanelResponser: public SoraGUIResponser {
	void action() {
		gcn::Slider* p = (gcn::Slider*)getSource();
		if(getID().compare("SMinR") == 0)
			peffect->pheader.dwMinStartColor.r = p->getValue();
		else if(getID().compare("SMinG") == 0)
			peffect->pheader.dwMinStartColor.g = p->getValue();
		else if(getID().compare("SMinB") == 0)
			peffect->pheader.dwMinStartColor.b = p->getValue();
		else if(getID().compare("SMinA") == 0)
			peffect->pheader.dwMinStartColor.a = p->getValue();
		else if(getID().compare("SMaxR") == 0)
			peffect->pheader.dwMaxStartColor.r = p->getValue();
		else if(getID().compare("SMaxG") == 0)
			peffect->pheader.dwMaxStartColor.g = p->getValue();
		else if(getID().compare("SMaxB") == 0)
			peffect->pheader.dwMaxStartColor.b = p->getValue();
		else if(getID().compare("SMaxA") == 0)
			peffect->pheader.dwMaxStartColor.a = p->getValue();
		
		else if(getID().compare("EMinR") == 0)
			peffect->pheader.dwMinEndColor.r = p->getValue();
		else if(getID().compare("EMinG") == 0)
			peffect->pheader.dwMinEndColor.g = p->getValue();
		else if(getID().compare("EMinB") == 0)
			peffect->pheader.dwMinEndColor.b = p->getValue();
		else if(getID().compare("EMinA") == 0)
			peffect->pheader.dwMinEndColor.a = p->getValue();
		else if(getID().compare("EMaxR") == 0)
			peffect->pheader.dwMaxEndColor.r = p->getValue();
		else if(getID().compare("EMaxG") == 0)
			peffect->pheader.dwMaxEndColor.g = p->getValue();
		else if(getID().compare("EMaxB") == 0)
			peffect->pheader.dwMaxEndColor.b = p->getValue();
		else if(getID().compare("EMaxA") == 0)
			peffect->pheader.dwMaxEndColor.a = p->getValue();
		//labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
	}
};

class OptionPanelResponser: public SoraGUIResponser {
	void action() {
			gcn::CheckBox* pcheck = (gcn::CheckBox*)getSource();
			if(getID().compare("SpeedPanelCheck") == 0) {
				if(!pcheck->isSelected()) {
					pSpeedPanel->setVisible(false);
					pSpeedPanel->setEnabled(false);
				} else {
					pSpeedPanel->setVisible(true);
					pSpeedPanel->setEnabled(true);
				}
			}
			else if(getID().compare("PropPanelCheck") == 0) {
				if(!pcheck->isSelected()) {
					pPropPanel->setVisible(false);
					pPropPanel->setEnabled(false);
				} else {
					pPropPanel->setVisible(true);
					pPropPanel->setEnabled(true);
				} 
			} else if(getID().compare("LifetimePanelCheck") == 0) {
				if(!pcheck->isSelected()) {
					pLifetimePanel->setVisible(false);
					pLifetimePanel->setEnabled(false);
				} else {
					pLifetimePanel->setVisible(true);
					pLifetimePanel->setEnabled(true);
				}
			} else if(getID().compare("StartColorPanelCheck") == 0) {
				if(!pcheck->isSelected()) {
					pStartColorPanel->setVisible(false);
					pStartColorPanel->setEnabled(false);
				} else {
					pStartColorPanel->setVisible(true);
					pStartColorPanel->setEnabled(true);
				}
			} else if(getID().compare("EndColorPanelCheck") == 0) {
				if(!pcheck->isSelected()) {
					pEndColorPanel->setVisible(false);
					pEndColorPanel->setEnabled(false);
				} else {
					pEndColorPanel->setVisible(true);
					pEndColorPanel->setEnabled(true);
				}
			} else if(getID().compare("BlendModeCheck") == 0) {
				if(!pcheck->isSelected()) {
					pcheck->setCaption("AlphaAdd");
					peffect->setBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE);
				} else {
					pcheck->setCaption("AlphaBlend");
					peffect->setBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE);
				}
			}
	}
};



void restoreValue() {
	slMinSpeed->setValue(peffect->pheader.fMinSpeed);
	slMaxSpeed->setValue(peffect->pheader.fMaxSpeed);
	slMinLinearAccel->setValue(peffect->pheader.fMinLinearAcc);
	slMaxLinearAccel->setValue(peffect->pheader.fMaxLinearAcc);
	slMinTrigAccel->setValue(peffect->pheader.fMinTrigAcc);
	slMaxTrigAccel->setValue(peffect->pheader.fMaxTrigAcc);
	slMinLifetime->setValue(peffect->pheader.fMinLifetime);
	slMaxLifetime->setValue(peffect->pheader.fMaxLifetime);

	slMinSpin->setValue(peffect->pheader.fMinSpin);
	slMaxSpin->setValue(peffect->pheader.fMaxSpin);
	slMinGravity->setValue(peffect->pheader.fMinGravity);
	slMaxGravity->setValue(peffect->pheader.fMaxGravity);
	slMinStartScale->setValue(peffect->pheader.fMinStartScale);
	slMaxStartScale->setValue(peffect->pheader.fMaxStartScale);
	slMinEndScale->setValue(peffect->pheader.fMaxStartScale);
	slMaxEndScale->setValue(peffect->pheader.fMaxEndScale);

	slMinStartColorR->setValue(peffect->pheader.dwMinStartColor.r);
	slMinStartColorG->setValue(peffect->pheader.dwMinStartColor.g);
	slMinStartColorB->setValue(peffect->pheader.dwMinStartColor.b);
	slMinStartColorA->setValue(peffect->pheader.dwMinStartColor.a);
	slMaxStartColorR->setValue(peffect->pheader.dwMaxStartColor.r);
	slMaxStartColorG->setValue(peffect->pheader.dwMaxStartColor.g);
	slMaxStartColorB->setValue(peffect->pheader.dwMaxStartColor.b);
	slMaxStartColorA->setValue(peffect->pheader.dwMaxStartColor.a);
	slMinEndColorR->setValue(peffect->pheader.dwMinEndColor.r);
	slMinEndColorG->setValue(peffect->pheader.dwMinEndColor.g);
	slMinEndColorB->setValue(peffect->pheader.dwMinEndColor.b);
	slMinEndColorA->setValue(peffect->pheader.dwMinEndColor.a);
	slMaxEndColorR->setValue(peffect->pheader.dwMaxEndColor.r);
	slMaxEndColorG->setValue(peffect->pheader.dwMaxEndColor.g);
	slMaxEndColorB->setValue(peffect->pheader.dwMaxEndColor.b);
	slMaxEndColorA->setValue(peffect->pheader.dwMaxEndColor.a);

	slSpreadRangeX->setValue(peffect->pheader.emitRange.x);
	slSpreadRangeY->setValue(peffect->pheader.emitRange.y);
	slSpreadRangeZ->setValue(peffect->pheader.emitRange.z);
	slEmissionTime->setValue(peffect->pheader.fEmitLifetime);
	slEmissionSpeed->setValue(peffect->pheader.nEmitNum);

	slEmitPointX->setValue(peffect->pheader.emitPos.x);
	slEmitPointY->setValue(peffect->pheader.emitPos.y);
	slEmitPointZ->setValue(peffect->pheader.emitPos.z);

	slMinDirectionX->setValue(peffect->pheader.minDirection.x);
	slMinDirectionY->setValue(peffect->pheader.minDirection.y);
	slMinDirectionZ->setValue(peffect->pheader.minDirection.z);

	slMaxDirectionX->setValue(peffect->pheader.maxDirection.x);
	slMaxDirectionY->setValue(peffect->pheader.maxDirection.y);
	slMaxDirectionZ->setValue(peffect->pheader.maxDirection.z);

	slMinAngle->setValue(peffect->pheader.fMinAngle);
	slMaxAngle->setValue(peffect->pheader.fMaxAngle);

	slTextureRect->setValue(5);

	labelLoadValue();
}

void labelLoadValue() {
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinSpeedLabel"), L"MinSpeed", slMinSpeed->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxSpeedLabel"), L"MaxSpeed", slMaxSpeed->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinLinearAccelLabel"), L"MinLinearAccel", slMinLinearAccel->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxLinearAccelLabel"), L"MaxLinearAccel", slMaxLinearAccel->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinTrigAccelLabel"), L"MinTrigAccel", slMinTrigAccel->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxTrigAccelLabel"), L"MaxTrigAccel", slMaxTrigAccel->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("MinLifetimeLabel"), L"MinLifetime", slMinLifetime->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxLifetimeLabel"), L"MaxLifetime", slMaxLifetime->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmissionTimeLabel"), L"EmissionTime", slEmissionTime->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmissionSpeedLabel"), L"EmissionSpeed", slEmissionSpeed->getValue());

	
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinSpinLabel"), L"MinSpin", slMinSpin->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxSpinLabel"), L"MaxSpin", slMaxSpin->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinGravityLabel"), L"MinGravity", slMinGravity->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxGravityLabel"), L"MaxGravity", slMaxGravity->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinStartScaleLabel"), L"MinStaScale", slMinStartScale->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxStartScaleLabel"), L"MaxStaScale", slMaxStartScale->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinEndScaleLabel"), L"MinEndScale", slMinEndScale->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxEndScaleLabel"), L"MaxEndScale", slMaxEndScale->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitRangeXLabel"), L"EmitRangeX", slSpreadRangeX->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitRangeYLabel"), L"EmitRangeY", slSpreadRangeY->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitRangeZLabel"), L"EmitRangeZ", slSpreadRangeZ->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitPointXLabel"), L"EmitPointX", slEmitPointX->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitPointYLabel"), L"EmitPointY", slEmitPointY->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EmitPointZLabel"), L"EmitPointZ", slEmitPointZ->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("MinDirectionXLabel"), L"MinDireciontX", slMinDirectionX->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinDirectionYLabel"), L"MinDirectionY", slMinDirectionY->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MinDirectionZLabel"), L"MinDirectionZ", slMinDirectionZ->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxDirectionXLabel"), L"MaxDireciontX", slMaxDirectionX->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxDirectionYLabel"), L"MaxDirectionY", slMaxDirectionY->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxDirectionZLabel"), L"MaxDirectionZ", slMaxDirectionZ->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("MinAngleLabel"), L"MinAngle", slMinAngle->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("MaxAngleLabel"), L"MaxAngle", slMaxAngle->getValue());
/*
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMinRLabel"), L"R", slMinStartColorR->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMinGLabel"), L"G", slMinStartColorG->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMinBLabel"), L"B", slMinStartColorB->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMinALabel"), L"A", slMinStartColorA->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMaxRLabel"), L"R", slMaxStartColorR->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMaxGLabel"), L"G", slMaxStartColorG->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMaxBLabel"), L"B", slMaxStartColorB->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("SMaxALabel"), L"A", slMaxStartColorA->getValue());

	labelApplyValue((PLABEL)pXmlParser->getWidget("EMinRLabel"), L"R", slMinEndColorR->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMinGLabel"), L"G", slMinEndColorG->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMinBLabel"), L"B", slMinEndColorB->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMinALabel"), L"A", slMinEndColorA->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMaxRLabel"), L"R", slMaxEndColorR->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMaxGLabel"), L"G", slMaxEndColorG->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMaxBLabel"), L"B", slMaxEndColorB->getValue());
	labelApplyValue((PLABEL)pXmlParser->getWidget("EMaxALabel"), L"A", slMaxEndColorA->getValue());*/

}

#include "SoraFileUtility.h"

void setTextureRectSlider(SoraSprite* pspr) {
	slTextureRect->setValue(0);
	pspr->setTextureRect(0.f, 0.f, 32.f, 32.f);
	slTextureRect->setScaleEnd((pspr->getTextureWidth()*pspr->getTextureHeight())/(32*32));
	SORA->log("Set Max TextureRect Slider Value:"+int_to_str(slTextureRect->getScaleEnd()));
}

void loadParticleSprite(const SoraWString& path) {
	delete pspr;
	pspr = SORA->createSprite(path);
	if(pspr == NULL) {
		SORA->messageBoxW(L"Error loading sprite "+path, L"Error", MB_OK | MB_ICONERROR);
		pspr = SORA->createSprite(L"pics/particles.png");
	}

	peffect->setSprite(pspr);
	peffect->restart();
}

void loadBGSprite(const SoraWString& path) {
	if(pbgSpr) 
		delete pbgSpr;
	pbgSpr = SORA->createSprite(path);
	if(pbgSpr == NULL) {
		SORA->messageBoxW(L"Error loading sprite "+path, L"Error", MB_OK | MB_ICONERROR);
	}
	
	/*float32 scale1 = 1.0, scale2 = 1.0;
	if(pbgSpr->getSpriteWidth() > 1024)
		scale1 = pbgSpr->getSpriteWidth() / 1024.f;
	if(pbgSpr->getSpriteHeight() > 768)
		scale2 = pbgSpr->getSpriteHeight() / 768.f;
	pbgSpr->setScale(scale1<scale2?scale1:scale2, scale1<scale2?scale1:scale2);*/
//	pbgSpr->setCenter(pbgSpr->getSpriteWidth()/2, pbgSpr->getSpriteHeight()/2);
}

class OptionPanelButtonResponser: public SoraGUIResponser {
	void action() {
        if(getID().compare("RestoreDefault") == 0) {
            peffect->killAll();
            peffect->emit(sCurrParticleFile, pspr);
            
            restoreValue();
        } else if(getID().compare("Open") == 0) {
            if(bOpenFinished) {
                peffect->restart();
                bOpenFinished = false;
                return;
            }
            
            
            SoraWString particleFile = sora::SORA->fileOpenDialog("sps", sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
            
            if(particleFile.size() != 0) {				
                delete peffect;
                peffect = new SoraParticleSystem;
                peffect->emit(particleFile, pspr);
                bOpenFinished = true;
                
                restoreValue();
                peffect->fire();
                return;
                
            }
        } else if(getID().compare("Save") == 0) {
            peffect->saveScript(sCurrParticleFile);
        } else if(getID().compare("SaveAs") == 0) {
            
            SoraWString particleFile = sora::SORA->fileSaveDialog("sps", sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str(), "sps");
            
            if(!particleFile.empty()) {
                peffect->saveScript(particleFile);
            }
            
        }  else if(getID().compare("FPS") == 0) {
            gcn::Slider* ps = (gcn::Slider*)getSource();
            sora::SORA->setFPS(ps->getValue());
            
        } else if(getID().compare("TextureRect") == 0) {
            gcn::Slider* ps = (gcn::Slider*)getSource();
            int v = (int)ps->getValue() % peMainWindowLoader::Instance()->getRow();
            int h = (int)ps->getValue() / peMainWindowLoader::Instance()->getRow();
            peffect->pheader.texX = v*peMainWindowLoader::Instance()->getTexWidth();
            peffect->pheader.texY = h*peMainWindowLoader::Instance()->getTexHeight();
            peffect->pheader.texW = peMainWindowLoader::Instance()->getTexWidth();
            peffect->pheader.texH = peMainWindowLoader::Instance()->getTexHeight();
            
        } else if(getID().compare("ParticleSprite") == 0) {
            
            SoraWString imageFile = sora::SORA->fileOpenDialog("jpg;png;bmp;jpeg", sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
            
            loadParticleSprite(imageFile);
            
        } else if(getID().compare("BGSprite") == 0) {
            SoraWString imageFile = sora::SORA->fileOpenDialog("jpg;png;bmp;jpeg", sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
            
            loadBGSprite(imageFile);
        }
    }
};

#include "SoraGUIChan/guichansetup.h"
   

editorWindow::editorWindow() {
	SoraGUIResponserMap::Instance()->registerResponser("SpeedPanel", new SpeedPanelResponser);
	SoraGUIResponserMap::Instance()->registerResponser("LifetimePanel", new LifetimePanelResponser);
	SoraGUIResponserMap::Instance()->registerResponser("PropPanel", new PropPanelResponser);
	SoraGUIResponserMap::Instance()->registerResponser("ColorPanel", new ColorPanelResponser);
	SoraGUIResponserMap::Instance()->registerResponser("OptionPanel", new OptionPanelResponser);
	SoraGUIResponserMap::Instance()->registerResponser("OptionPanelButton", new OptionPanelButtonResponser);

	pFont = SORA->createFont(L"cour.ttf", 26);
	if(!pFont)
		SORA->messageBox("Error creating font", "Error", MB_OK | MB_ICONERROR);
	pFont->setColor(0xFFFFFFFF);

	peffect = new SoraParticleSystem();
	pspr = SORA->createSprite(sora::s2ws(peMainWindowLoader::Instance()->getDefaultParticleSprite()));
	peffect->emit(L"Default.sps", pspr);
	peffect->pheader.fEmitDuration = 10.f;
	peffect->pheader.fEmitLifetime = 10.f;
	
	peffect->fire();
	
	mBGPosX = 0.f;
	mBGPosY = 0.f;
	
	registerEventFunc(this, &editorWindow::onInputEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
	
}

void editorWindow::update() {
	peffect->update(SoraCore::Instance()->getDelta());
	if(peffect->getLiveParticle() == 0)
		peffect->restart();
	if(bOpenFinished) {
		peffect->restart();
		bOpenFinished = false;
	}
	
	if(SORA->keyDown(SORA_KEY_LEFT)) {
		if(mBGPosX > 0.f)
			mBGPosX -= 1.f;
		else mBGPosX = 0.f;
	} else if(SORA->keyDown(SORA_KEY_RIGHT)) {
		if(mBGPosX < SORA->getScreenWidth())
			mBGPosX += 1.f;
		else mBGPosX = SORA->getScreenWidth();
	}
	if(SORA->keyDown(SORA_KEY_UP)) {
		if(mBGPosY > 0.f)
			mBGPosY -= 1.f;
		else mBGPosY = 0.f;
	} else if(SORA->keyDown(SORA_KEY_DOWN)) {
		if(mBGPosY < SORA->getScreenHeight())
			mBGPosY += 1.f;
		else mBGPosY = SORA->getScreenHeight();
	}
	
	if(sora::SORA->keyDown(SORA_KEY_LBUTTON)) {
		float32 x, y;
		sora::SORA->getMousePos(&x, &y);
		if(peffect->isActive() && peffect->getBoundingBox().TestPoint(x, y)) {
			bMouseDown = true;
		}
	} else {
		bMouseDown = false;
	}

	
	if(bMouseDown) {
		float32 x, y;
		sora::SORA->getMousePos(&x, &y);
		peffect->moveTo(x, y, peffect->pheader.emitPos.z);
		
		slEmitPointX->setValue(x);
		slEmitPointY->setValue(y);
	}
}

void editorWindow::render() {
	if(pbgSpr) {
		pbgSpr->render(mBGPosX, mBGPosY);
	}
	
	peffect->render();
	if(bShowBoundingBox) {
		hgeRect box = peffect->getBoundingBox();
		sora::SORA->renderBox(box.x1, box.y1, box.x2, box.y2, 0xFFFFFFFF);
	}
	
	if(pFont) {
		int32 mWidth = sora::SORA->getScreenWidth();
		int32 mHeight = sora::SORA->getScreenHeight();
		pFont->print(mWidth-100, mHeight-70, sora::FONT_ALIGNMENT_CENTER, L"FPS: %.2f", sora::SORA->getFPS());
		pFont->render(mWidth-100, mHeight-50, s2ws("Particles: "+int_to_str(peffect->getLiveParticle())).c_str(), true);
		pFont->render(mWidth-100, mHeight-30, L"Rev 0x35 \0", true);
	}
}

#include "SoraGUIChan/guichan/AnimationFadeIn.h"

gcn::Widget* editorWindow::loadXML(const SoraWString& xmlPath) {
	gcn::Widget* pprev = GCN_GLOBAL->findWidget("SoraWindow");
	if(pprev != NULL) {
		GCN_GLOBAL->removeWidget(pprev);
		delete pprev;
		pprev = NULL;
	}
	
	pXmlParser = new XmlGui;

	ulong32 size;
	void* pdata = SORA->getResourceFile(xmlPath, size);
	if(!pdata) {
		SORA->messageBox("Cannot load window configuration file", "Fatal error", MB_OK | MB_ICONERROR);
		SORA->shutDown();
	}

	if(pXmlParser->parse((const char*)pdata, size)) {
		gcn::Widget* pWindow = pXmlParser->getWidget("SoraWindow");
		pWindow->setDimension(gcn::Rectangle(0, 0, SORA->getScreenWidth(), SORA->getScreenHeight()));
        pWindow->addModifier(new gcn::AnimationFadeIn(0, 255, 120));
		GCN_GLOBAL->getTop()->add( pWindow );

	//	pspr = SORA->createSprite(L"pics/particles.png");
	//	peffect = new SoraParticleSystem(sCurrParticleFile, pspr);
	//	peffect->pheader.blendMode = BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE;
	
		pSpeedPanel = (gcn::Window*)pXmlParser->getWidget("SpeedPanel");
		pPropPanel = (gcn::Window*)pXmlParser->getWidget("PropPanel");
		pLifetimePanel = (gcn::Window*)pXmlParser->getWidget("LifetimePanel");
		pStartColorPanel = (gcn::Window*)pXmlParser->getWidget("StartColorPanel");
		pEndColorPanel = (gcn::Window*)pXmlParser->getWidget("EndColorPanel");

		slMinSpeed = (gcn::Slider*)pXmlParser->getWidget("MinSpeed");
		slMaxSpeed = (gcn::Slider*)pXmlParser->getWidget("MaxSpeed");
		slMinLinearAccel = (gcn::Slider*)pXmlParser->getWidget("MinLinearAccel");
		slMaxLinearAccel = (gcn::Slider*)pXmlParser->getWidget("MaxLinearAccel");
		slMinTrigAccel = (gcn::Slider*)pXmlParser->getWidget("MinTrigAccel");
		slMaxTrigAccel = (gcn::Slider*)pXmlParser->getWidget("MaxTrigAccel");
		slMinLifetime = (gcn::Slider*)pXmlParser->getWidget("MinLifetime");
		slMaxLifetime = (gcn::Slider*)pXmlParser->getWidget("MaxLifetime");
		
		slMinSpin = (gcn::Slider*)pXmlParser->getWidget("MinSpin");
		slMaxSpin = (gcn::Slider*)pXmlParser->getWidget("MaxSpin");
		slMinGravity = (gcn::Slider*)pXmlParser->getWidget("MinGravity");
		slMaxGravity = (gcn::Slider*)pXmlParser->getWidget("MaxGravity");
		slMinStartScale = (gcn::Slider*)pXmlParser->getWidget("MinStartScale");
		slMaxStartScale = (gcn::Slider*)pXmlParser->getWidget("MaxStartScale");
		slMinEndScale = (gcn::Slider*)pXmlParser->getWidget("MinEndScale");
		slMaxEndScale = (gcn::Slider*)pXmlParser->getWidget("MaxEndScale");
		
		slMinStartColorR = (gcn::Slider*)pXmlParser->getWidget("SMinR");
		slMinStartColorG = (gcn::Slider*)pXmlParser->getWidget("SMinG");
		slMinStartColorB = (gcn::Slider*)pXmlParser->getWidget("SMinB");
		slMinStartColorA = (gcn::Slider*)pXmlParser->getWidget("SMinA");
		slMaxStartColorR = (gcn::Slider*)pXmlParser->getWidget("SMaxR");
		slMaxStartColorG = (gcn::Slider*)pXmlParser->getWidget("SMaxG");
		slMaxStartColorB = (gcn::Slider*)pXmlParser->getWidget("SMaxB");
		slMaxStartColorA = (gcn::Slider*)pXmlParser->getWidget("SMaxA");
		slMinEndColorR = (gcn::Slider*)pXmlParser->getWidget("EMinR");
		slMinEndColorG = (gcn::Slider*)pXmlParser->getWidget("EMinG");
		slMinEndColorB = (gcn::Slider*)pXmlParser->getWidget("EMinB");
		slMinEndColorA = (gcn::Slider*)pXmlParser->getWidget("EMinA");
		slMaxEndColorR = (gcn::Slider*)pXmlParser->getWidget("EMaxR");
		slMaxEndColorG = (gcn::Slider*)pXmlParser->getWidget("EMaxG");
		slMaxEndColorB = (gcn::Slider*)pXmlParser->getWidget("EMaxB");
		slMaxEndColorA = (gcn::Slider*)pXmlParser->getWidget("EMaxA");

		slSpreadRangeX = (gcn::Slider*)pXmlParser->getWidget("EmitRangeX");
		slSpreadRangeY = (gcn::Slider*)pXmlParser->getWidget("EmitRangeY");
		slSpreadRangeZ = (gcn::Slider*)pXmlParser->getWidget("EmitRangeZ");

		slEmissionTime = (gcn::Slider*)pXmlParser->getWidget("EmissionTime");
		slEmissionSpeed = (gcn::Slider*)pXmlParser->getWidget("EmissionSpeed");

		slEmitPointX = (gcn::Slider*)pXmlParser->getWidget("EmitPointX");
		slEmitPointY = (gcn::Slider*)pXmlParser->getWidget("EmitPointY");
		slEmitPointZ = (gcn::Slider*)pXmlParser->getWidget("EmitPointZ");

		slMinDirectionX = (gcn::Slider*)pXmlParser->getWidget("MinDirectionX");
		slMinDirectionY = (gcn::Slider*)pXmlParser->getWidget("MinDirectionY");
		slMinDirectionZ = (gcn::Slider*)pXmlParser->getWidget("MinDirectionZ");

		slMaxDirectionX = (gcn::Slider*)pXmlParser->getWidget("MaxDirectionX");
		slMaxDirectionY = (gcn::Slider*)pXmlParser->getWidget("MaxDirectionY");
		slMaxDirectionZ = (gcn::Slider*)pXmlParser->getWidget("MaxDirectionZ");

		cbSpeedPanel = (gcn::CheckBox*)pXmlParser->getWidget("SpeedPanelCheck");
		cbLifetimePanel = (gcn::CheckBox*)pXmlParser->getWidget("LifetimePanelCheck");
		cbStartColorPanel = (gcn::CheckBox*)pXmlParser->getWidget("StartColorPanelCheck");
		cbEndColorPanel = (gcn::CheckBox*)pXmlParser->getWidget("EndColorPanelCheck");
		cbPropPanel = (gcn::CheckBox*)pXmlParser->getWidget("PropPanelCheck");
		cbBlendMode = (gcn::CheckBox*)pXmlParser->getWidget("BlendModeCheck");

		slMinAngle = (gcn::Slider*)pXmlParser->getWidget("MinAngle");
		slMaxAngle = (gcn::Slider*)pXmlParser->getWidget("MaxAngle");

		slTextureRect = (gcn::Slider*)pXmlParser->getWidget("TextureRect");
		if(slTextureRect) {
			slTextureRect->setScale(0.0, peMainWindowLoader::Instance()->getRow()*peMainWindowLoader::Instance()->getCol());
			slTextureRect->setStepLength(1.0);
			
			std::cout<<peMainWindowLoader::Instance()->getRow()*peMainWindowLoader::Instance()->getCol()<<std::endl;
		}

		cbSpeedPanel->setSelected(true);
		cbLifetimePanel->setSelected(true);
		cbStartColorPanel->setSelected(true);
		cbEndColorPanel->setSelected(true);
		cbPropPanel->setSelected(true);
		cbBlendMode->setSelected(peffect->getBlendMode()==(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE)?false:true);
		cbBlendMode->setCaption(peffect->getBlendMode()==(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE)?"AlphaAdd":"AlphaBlend");

		labelLoadValue();
		restoreValue();
		
		new PEConsoleCmdHandler;

		return pWindow;

	} else {
		SORA->messageBox("Cannot parse window configuration file", "Fatal error", MB_OK | MB_ICONERROR);
		SORA->shutDown();
	}
	return 0;
}

	void editorWindow::onInputEvent(sora::SoraKeyEvent* kev) {

	}







