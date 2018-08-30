#pragma once
#include <d3d9.h>
#include <D3DX10math.h>
#include "CTriangle.h"
#include "CAxis.h"
#include "CCube.h"

class CD3DApp
{
protected:
	LPDIRECT3D9 m_pD3D;						// Direct3D system
	LPDIRECT3DDEVICE9 m_pd3dDevice;			// Direct3D device
	HWND m_hWnd;

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



