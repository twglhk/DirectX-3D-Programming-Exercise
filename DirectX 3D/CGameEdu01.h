#pragma once
#include "CD3DApp.h"

class CGameEdu01 : public CD3DApp
{
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01();
	~CGameEdu01();
};

