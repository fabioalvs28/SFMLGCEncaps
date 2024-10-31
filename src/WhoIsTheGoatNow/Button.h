#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(Button)

public :
	void OnClick();
	void SetType(int type) { m_buttonType = type; }
	void SetPlayScene(GCScene* pScene) { m_pPlayScene = pScene; }
	enum ButtonType
	{
		Play, Quit
	};
protected:
	GCScene* m_pPlayScene;
	int m_buttonType;

CREATE_SCRIPT_END