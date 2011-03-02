/*
 *  GUI.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 4/28/10.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */


class GUIBase {
public:
	GUIBase() {}
	virtual ~GUIBase() {}
	
	virtual void Draw() = 0;
	virtual void Update() = 0;
	
	virtual void OnKeyPress(int iKey, int iAlt, int iCtrl, int iShift);
	virtual void OnMouseOver();

private:
	
};

class GUIMessageBox: public GUIBase {
	//..... implements.
};

class GUIManager: public Singleton {
public:
	void AddGUIControl(GUIBase* pCtrl);
};

template<typename T>
class Singleton {
protected:
	Singleton() {}
	virtual Singleton() {}

public:
	static T* GetInstance() {
		if(!pInstance) pInstance = new T;
		return pInstance;
	}

	static void Destroy() {
		if(pInstance) delete pInstance;
	}

private:
	Singleton(const Singleton& s) {}
	Singleton& operator=(const Singleton& s) {}
	
	static T* pInstance;
};