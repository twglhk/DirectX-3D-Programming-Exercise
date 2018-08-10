#pragma once
#include <d3d9.h>

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class CTriangle
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DDEVICE9  m_pd3dDevice;

public:
	void OnInit( LPDIRECT3DDEVICE9  pd3dDevice );
	void OnRender();
	void OnRelease();

public:
	CTriangle(void);
	~CTriangle(void);
};

