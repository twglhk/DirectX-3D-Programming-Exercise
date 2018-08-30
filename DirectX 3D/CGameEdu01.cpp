#include "stdafx.h"
#include "CGameEdu01.h"


CGameEdu01::CGameEdu01()
{

}


CGameEdu01::~CGameEdu01()
{
	
}


void CGameEdu01::OnInit()
{
	RECT rect;
	D3DVIEWPORT9 vp;

	// Init view trans param
	m_Eye.x = 5.0f;
	m_Eye.y = 8.0f;
	m_Eye.z = -10.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;

	// Viewport param setup
	GetClientRect(m_hWnd, &rect);
	
	vp.X = 0;
	vp.Y = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	// View transformation
	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// Projection transformation
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	// Viewport setup 
	m_pd3dDevice->SetViewport(&vp);

	// Object Setup
	//triangle->OnInit(m_pd3dDevice);
	axis.OnInit(m_pd3dDevice);
	cube.OnInit(m_pd3dDevice);
}

void CGameEdu01::OnRender()
{
	//triangle->OnRender();
	axis.OnRender();
	cube.OnRender();
}

void CGameEdu01::OnUpdate()
{
	// Changing of vertex position
}

void CGameEdu01::OnRelease()
{
	//triangle->OnRelease();
	axis.OnRelease();
	cube.OnRelease();
}
