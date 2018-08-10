#pragma once
#include "d3dapp.h"

struct CUSTOMVERTEX
{
    D3DXVECTOR3 vPos;
	float tu, tv;            
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

class CGameEdu01 : public CD3DApp
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXMATRIX m_matScale[2], m_matTranslate[2];

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;	

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

