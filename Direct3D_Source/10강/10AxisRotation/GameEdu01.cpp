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
	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect( m_hWnd, &rect );

	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = rect.right - rect.left;  // ÁÂÇ¥°³³äÀÌ ¾Æ´Ï¶ó Å©±â °³³ä 800, 600 °è»êµÊ
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 0.0f;
	m_Eye.z = -8.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;
	
	D3DXMatrixLookAtLH( &m_matView, &m_Eye, &m_At, &m_Up );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );	
    
    D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
	m_pd3dDevice->SetViewport( &vp );

	m_Axis.OnInit( m_pd3dDevice );		
	D3DXCreateTeapot( m_pd3dDevice,  &m_pTeapotMesh, NULL );	

}

void CGameEdu01::OnRender()
{	
	D3DXMATRIX matRotation;
	//D3DXVECTOR3 v1( 1.0f, 1.0f, 0.0f );
	D3DXVECTOR3 v1( 0.0f, 1.0f, 0.0f );

	m_Axis.OnRender();

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DXMatrixRotationAxis( &matRotation, &v1, GetTickCount() * 0.004f );

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matRotation );
	m_pTeapotMesh->DrawSubset(0);	
}

void CGameEdu01::OnUpdate()
{		
		
}

void CGameEdu01::OnRelease()
{	
	m_pTeapotMesh->Release();
	m_Axis.OnRelease();
}