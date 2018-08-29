#pragma once
#include <d3d9.h>

struct AXISVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};
#define D3DFVF_AXISVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

class CAxis
{
private:
	LPDIRECT3DVERTEXBUFFER9 pVB;			// Vertex Buffer
	LPDIRECT3DDEVICE9 m_pd3dDevice;			// Direct3D device

public:
	CAxis();
	~CAxis();

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnRelease();
};

