#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include <D3dx9shape.h>
#include "Axis.h"


struct TEAPOT_PROPERTY
{
	float fScaling;
	float fRotX, fRotY, fRotZ;
	D3DXVECTOR3 vTranslation;
	TEAPOT_PROPERTY() : fScaling(1.0f){}
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
	CAxis m_Axis;
	
	TEAPOT_PROPERTY m_TeapotProperty[3];
	LPD3DXMESH m_pTeapotMesh;	
	int m_nAngle;
public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

