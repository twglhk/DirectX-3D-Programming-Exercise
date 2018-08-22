#pragma once
#include <d3d9.h>

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

class CD3DApp
{
protected:
	LPDIRECT3D9 m_pD3D;						// Direct3D system
	LPDIRECT3DDEVICE9 m_pd3dDevice;			// Direct3D device
	HWND m_hWnd;
	LPDIRECT3DVERTEXBUFFER9 pVB;	// Vertex Buffer
	VOID* pVertices;
	CUSTOMVERTEX vertices[3] =
	{
	{ 150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, },
	{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
	{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	
	CD3DApp();
	~CD3DApp();

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnRelease() = 0;

public:
	HRESULT InitD3D(HWND hWnd);
	void Render();
	void Update();
	void CleanUp();
};



