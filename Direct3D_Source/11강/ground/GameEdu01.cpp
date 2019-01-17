#include "StdAfx.h"
#include "GameEdu01.h"
#include <D3dx9math.h>


CGameEdu01::CGameEdu01(void)
{
}


CGameEdu01::~CGameEdu01(void)
{
}

void CGameEdu01::OnInit()
{
	int i;

	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect( m_hWnd, &rect );

	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = rect.right - rect.left;  // ÁÂÇ¥°³³äÀÌ ¾Æ´Ï¶ó Å©±â °³³ä 800, 600 °è»êµÊ
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 1.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 10.0f;
	m_Eye.z = 0.0f;	

	m_At.x = 0.0f;
	m_At.y = -3.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;	
	
	D3DXMatrixLookAtLH( &m_matView, &m_Eye, &m_At, &m_Up );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );	
    
    D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 10.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
	m_pd3dDevice->SetViewport( &vp );
	
	m_Ground.Create( m_pd3dDevice, 40, 40, 2.0f );	
}

void CGameEdu01::OnRender()
{		
	m_Ground.OnRender();		
}

void CGameEdu01::OnUpdate()
{	
		
}

void CGameEdu01::OnRelease()
{		
	m_Ground.OnRelease();
}