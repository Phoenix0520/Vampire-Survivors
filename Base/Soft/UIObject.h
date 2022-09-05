#pragma once
//#include "GameObject.h"
class UIObject	: public GameObject// �߻�Ŭ����
{
public:	// ������, �Ҹ���
	UIObject() 
	{
		for (int i = 0; i < 4; i++)
			tabGroup[i] = nullptr;   // LEFT,RIGHT,TOP,BOTTOM
	}
	~UIObject() {}

public:
	void SetHover(bool val) { hover = val; }
	void SetClick(bool val) { click = val; }
	void SetLeftTabGroup(UIObject   *pObject) { tabGroup[0] = pObject; }
	void SetRightTabGroup(UIObject  *pObject) { tabGroup[1] = pObject; }
	void SetTopTabGroup(UIObject    *pObject) { tabGroup[2] = pObject; }
	void SetBottomTabGroup(UIObject *pObject) { tabGroup[3] = pObject; }
	void SetCallback(function<void(int)> cbFunction) { enterCallBack = cbFunction; }
public:	 // Getter
	bool GetHover() { if (!active) return false; return hover; }
	bool IsClick() { if (!active) return false; return click; }

protected:
	function<void(int)>  enterCallBack = nullptr;
	bool hover = false;
	bool click = false;
	int  id = 0;
	UIObject* tabGroup[4];    // 4���� �������� ���� �ִ� Button �߰�
};