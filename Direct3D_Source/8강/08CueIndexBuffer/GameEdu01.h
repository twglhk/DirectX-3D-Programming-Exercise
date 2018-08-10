#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include "Axis.h"
#include "Cube.h"

class CGameEdu01 : public CD3DApp
{
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;
	CAxis m_Axis;
	CCube m_Cube;

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

