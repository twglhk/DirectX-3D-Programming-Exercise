#pragma once
#include "d3dapp.h"

struct CUSTOMVERTEX
{
    D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
    DWORD dwDiffuse;        
};

struct CROSSVERTEX
{
    D3DXVECTOR3 vPos;   
    DWORD dwDiffuse;        
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)
#define D3DFVF_CROSSVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class CGameEdu01 : public CD3DApp
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DVERTEXBUFFER9 m_pCrossVB;

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

