#include "editorWindow.h"
#include "Darklib/FileDlgOsx.h"

gcn::Window* pSpeedPanel;
gcn::Window* pPropPanel;
gcn::Window* pLifetimePanel;
gcn::Window* pStartColorPanel;
gcn::Window* pEndColorPanel;

std::wstring sCurrParticleFile = L"Default.sps";

#if defined(OS_OSX) || defined(OS_WIN32)
FileDlg* fileDlg;
#endif

bool bOpenFinished = false;

sora::SoraSprite* pspr;
sora::SoraParticleSystem* peffect;


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
		labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
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
		labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
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
		labelApplyValue((PLABEL)pXmlParser->getWidget(getID()+"Label"), s2ws(getID()), p->getValue());
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

void loadParticleSprite(const SoraWString path) {
	delete pspr;
	pspr = SORA->createSprite(path);
	if(pspr == NULL)
		pspr = SORA->createSprite(L"pics/particles.png");

	peffect->restart();
}

void applyFilterSps() {
#ifdef WIN32	
	fileDlg->SetFilter(TEXT("SoraParticleFile (*.sps)\0*.sps\0\0"));
	fileDlg->SetDefaultExt(L".sps\0");	
#else	
	fileDlg->SetFilter("sps;");
	fileDlg->SetDefaultExt("sps");	
#endif
}
void applyFilterImage() {
#ifdef WIN32	
	fileDlg->SetFilter(TEXT("ImageFiles (*.png,*.bmp,*.jpg)\0*.png;*.bmp;*.jpg\0\0"));
	fileDlg->SetDefaultExt(L".png\0");	
#else	
	fileDlg->SetFilter("png;bmp;jpg");
	fileDlg->SetDefaultExt("png");	
#endif
}

class OptionPanelButtonResponser: public SoraGUIResponser {
	void action() {
	if(getID().compare("RestoreDefault") == 0) {
		peffect->killAll();
		peffect->emit(sCurrParticleFile, pspr);
	
		restoreValue();
	} else if(getID().compare("Open") == 0) {
#if defined(OS_OSX) || defined(OS_WIN32)

		if(bOpenFinished) {
			peffect->restart();
			bOpenFinished = false;
			return;
		}

#ifdef WIN32
		wchar_t fileTitleBuffer[512];
		wchar_t filePathBuffer[512];

		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';
		
		applyFilterSps();
		if(fileDlg->FileOpenDlg((HWND)SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {
			sCurrParticleFile = filePathBuffer;
			
			delete peffect;
			peffect = new SoraParticleSystem;
			peffect->emit(sCurrParticleFile, pspr);
			bOpenFinished = true;
			
			restoreValue();
			peffect->fire();
			return;
			
#elif defined(OS_OSX)
		char fileTitleBuffer[512];
		char filePathBuffer[512];
			
		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';
			
		applyFilterSps();
		if(fileDlg->FileOpenDlg(SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {	
			sCurrParticleFile = s2ws(filePathBuffer);
			
			delete peffect;
			peffect = new SoraParticleSystem;
			peffect->emit(sCurrParticleFile, pspr);
			bOpenFinished = true;
			
			restoreValue();
			peffect->fire();
			return;
#endif
			
			
		}
#endif
	} else if(getID().compare("Save") == 0) {
		peffect->saveScript(sCurrParticleFile);
	} else if(getID().compare("SaveAs") == 0) {
		
#ifdef WIN32
		wchar_t fileTitleBuffer[512];
		wchar_t filePathBuffer[512];
		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';

		applyFilterSps();
		if(fileDlg->FileSaveDlg((HWND)SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {
			sCurrParticleFile = filePathBuffer;
		}
		
#elif defined(OS_OSX)
		char fileTitleBuffer[512];
		char filePathBuffer[512];
		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';
		
		applyFilterSps();
		if(fileDlg->FileSaveDlg(SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {
			sCurrParticleFile = s2ws(filePathBuffer);
		}
#endif
		
		peffect->saveScript(sCurrParticleFile);

	} else if(getID().compare("TextureRect") == 0) {
		gcn::Slider* ps = (gcn::Slider*)getSource();
		int v = (int)ps->getValue() % 4;
		int h = (int)ps->getValue() / 4;
		peffect->pheader.texX = v*32;
		peffect->pheader.texY = h*32;
		peffect->pheader.texW = 32;
		peffect->pheader.texH = 32;
	} else if(getID().compare("ParticleSprite") == 0) {
#ifdef WIN32
		wchar_t fileTitleBuffer[512];
		wchar_t filePathBuffer[512];
		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';
		
		applyFilterImage();
		if(fileDlg->FileOpenDlg((HWND)SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {
			loadParticleSprite(filePathBuffer);
		}
#elif defined(OS_OSX)
		char fileTitleBuffer[512];
		char filePathBuffer[512];
		fileTitleBuffer[0] = '\0';
		filePathBuffer[0] = '\0';
		
		applyFilterImage();
		if(fileDlg->FileOpenDlg(SoraCore::Instance()->getMainWindowHandle(), filePathBuffer, fileTitleBuffer)) {
			loadParticleSprite(s2ws(filePathBuffer));
		}
#endif
		
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
	pspr = SORA->createSprite(L"pics/particles.png");
	peffect->emit(L"Default.sps", pspr);
	peffect->pheader.fEmitDuration = 10.f;
	peffect->pheader.fEmitLifetime = 10.f;
	
	peffect->fire();
}

void editorWindow::update() {
	peffect->update(SoraCore::Instance()->getDelta());
	if(peffect->getLiveParticle() == 0)
		peffect->restart();
	if(bOpenFinished) {
		peffect->restart();
		bOpenFinished = false;
	}
}

void editorWindow::render() {
	peffect->render();
	
	if(pFont) {
		pFont->render(100, 690, s2ws(fp_to_str(SoraCore::Instance()->getFPS())).c_str(), true);
		pFont->render(100, 720, s2ws("Particles: "+int_to_str(peffect->getLiveParticle())).c_str(), true, true);
		pFont->render(100, 740, L"Rev 0x10 \0", true, true);
	}
}

gcn::Widget* editorWindow::loadXML(const SoraWString& xmlPath) {
	pXmlParser = new XmlGui;

	ulong32 size;
	void* pdata = SORA->getResourceFile(xmlPath, size);
	if(!pdata) {
		SORA->messageBox("Cannot load window configuration file", "Fatal error", MB_OK | MB_ICONERROR);
		SORA->shutDown();
	}

	if(pXmlParser->parse((const char*)pdata, size)) {
		gcn::Widget* pWindow = pXmlParser->getWidget("SoraWindow");
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

		cbSpeedPanel = (gcn::CheckBox*)pXmlParser->getWidget("CSpeedPanel");
		cbLifetimePanel = (gcn::CheckBox*)pXmlParser->getWidget("CLifetimePanel");
		cbStartColorPanel = (gcn::CheckBox*)pXmlParser->getWidget("CStartColorPanel");
		cbEndColorPanel = (gcn::CheckBox*)pXmlParser->getWidget("CEndColorPanel");
		cbPropPanel = (gcn::CheckBox*)pXmlParser->getWidget("CPropPanel");
		cbBlendMode = (gcn::CheckBox*)pXmlParser->getWidget("CBlendMode");

		slMinAngle = (gcn::Slider*)pXmlParser->getWidget("MinAngle");
		slMaxAngle = (gcn::Slider*)pXmlParser->getWidget("MaxAngle");

		slTextureRect = (gcn::Slider*)pXmlParser->getWidget("TextureRect");

		cbSpeedPanel->setSelected(true);
		cbLifetimePanel->setSelected(true);
		cbStartColorPanel->setSelected(true);
		cbEndColorPanel->setSelected(true);
		cbPropPanel->setSelected(true);
		cbBlendMode->setSelected(peffect->getBlendMode()==(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE)?false:true);
		cbBlendMode->setCaption(peffect->getBlendMode()==(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE)?"AlphaAdd":"AlphaBlend");

		labelLoadValue();
		restoreValue();
		
#if defined(OS_OSX) || defined(OS_WIN32)
		fileDlg = new FileDlg();
#ifdef OS_OSX
		fileDlg->SetDefaultPath(ws2s(SoraFileUtility::getApplicationPath()).c_str());
#else
		fileDlg->SetDefaultPath(SoraFileUtility::getApplicationPath().c_str());
#endif
#endif

		return pWindow;

	} else {
		SORA->messageBox("Cannot parse window configuration file", "Fatal error", MB_OK | MB_ICONERROR);
		SORA->shutDown();
	}
	return 0;
}