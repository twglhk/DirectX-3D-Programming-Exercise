#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include <D3dx9shape.h>

struct BOX_PROPERTY
{
	D3DXVECTOR3 vTans; 
	float fScaling;	
	D3DXVECTOR3 vMin, vMax;
};

class CGameEdu01 : public CD3DApp
{
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;	

	BOX_PROPERTY m_Box[2];
	D3DXVECTOR3 m_vMin, m_vMax;
	BOOL m_bIsCollision;	
	LPD3DXMESH m_pMesh;

	// ÆùÆ®
	LPD3DXFONT m_pFont;	
	DWORD m_dwElapsedTime;
public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

