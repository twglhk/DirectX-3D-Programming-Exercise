#pragma once
#include "d3dapp.h"
#include "Triangle.h"

class CGameEdu01 : public CD3DApp
{
private:
	CTriangle m_Triangle;

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

