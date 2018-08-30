#pragma once
#include <d3d9.h>

struct CUBEVERTEX
{
	D3DXVECTOR3 vertex;
	D3DCOLOR color;
};
#define D3DFVF_CUBEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CCube
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIndexB;

public:
	CCube();
	~CCube();

	void OnInit(LPDIRECT3DDEVICE9 pDevice);
	void OnRender();
	void OnRelease();
};

