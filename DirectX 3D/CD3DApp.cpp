#include "stdafx.h"
#include "CD3DApp.h"


CD3DApp::CD3DApp()
{
}


CD3DApp::~CD3DApp()
{
}


HRESULT CD3DApp::InitD3D(HWND hWnd)
{
	// System Creation
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	m_hWnd = hWnd;

	// Struct setup
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Z buffer setup
	d3dpp.EnableAutoDepthStencil = TRUE; // DirectX device 생성시 깊이 버퍼를 자동으로 생성해줌.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // 깊이 버퍼가 데이터를 저장하는 형식. 24bit를 많이 사용한다. 
	
	
	// Create D3D Device
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE); // Device 내에서 zbuffer를 사용한다는 것을 설정해야함.

	i = 0;

	// callback
	OnInit();

	return S_OK;
}

void CD3DApp::Render()
{

	if (NULL == m_pd3dDevice)
		return;

	// Clear the backbuffer to a blue color
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		hdc = GetDC(m_hWnd);
		if (GetAsyncKeyState('A') < 0)	
			i++;
		
		// Rendering of scene objects can happen here
		OnRender();
		

		sprintf_s(str, "%d", i);
		TextOut(hdc, 10, 10, (LPCWSTR)str, strlen(str));
		ReleaseDC(m_hWnd, hdc);

		// End the scene
		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	//std::cout << "Render" << std::endl;
}

void CD3DApp::Update()
{
	OnUpdate();
}

void CD3DApp::CleanUp()
{
	OnRelease();

	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();
}