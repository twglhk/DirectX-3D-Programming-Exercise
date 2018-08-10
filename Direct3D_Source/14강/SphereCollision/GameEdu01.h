#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include <D3dx9shape.h>

struct SPHERE_PROPERTY
{
	D3DXVECTOR3 vTans; // 중심점과 같은 역할
	float fScaling;
	float fRadius;	
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

	SPHERE_PROPERTY m_Sphere[2];
	BOOL m_bIsCollision;
	LPD3DXMESH m_pMesh;

	// 폰트
	LPD3DXFONT m_pFont;
	

	DWORD m_dwElapsedTime;

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

