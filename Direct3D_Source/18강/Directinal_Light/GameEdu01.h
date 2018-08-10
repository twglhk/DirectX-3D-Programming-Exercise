#pragma once
#include "d3dapp.h"

struct CUSTOMVERTEX
{
    D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;     
};

struct CROSSVERTEX
{
    D3DXVECTOR3 vPos;   
    DWORD dwDiffuse;        
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)
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
	D3DMATERIAL9 m_Material;
	D3DLIGHT9 m_Light;
	void SetMaterial( float r, float g, float b );
	void SetDirectionalLight();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

