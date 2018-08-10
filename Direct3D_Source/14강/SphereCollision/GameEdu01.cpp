#include "StdAfx.h"
#include "GameEdu01.h"
#include <D3dx9math.h>
#include <stdio.h>

BOOL CheckSphereIntersect( D3DXVECTOR3 vCenter1, float fRadius1, 
                              D3DXVECTOR3  vCenter2, float fRadius2 )
{
	float fDistance;
	D3DXVECTOR3 vDiff;

	vDiff = vCenter2 - vCenter1;
	fDistance = D3DXVec3Length( &vDiff );

	if( fDistance <= (fRadius1 + fRadius2 ) )
		return TRUE;  // 충돌
	return FALSE; // 비 충돌
}


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
	vp.Width  = rect.right - rect.left;  // 좌표개념이 아니라 크기 개념 800, 600 계산됨
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 5.0f;
	m_Eye.z = -20.0f;	

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
	
	D3DXCreateFont( m_pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
          OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
           "돋음체", &m_pFont );	

	D3DXCreateSphere( m_pd3dDevice, 1.0f, 20, 20, &m_pMesh, NULL );

	m_Sphere[0].fRadius = 1.0f;
	m_Sphere[0].fScaling = 1.0f;
	m_Sphere[0].vTans = D3DXVECTOR3( 0, 0, 0 );

	m_Sphere[1].fRadius = 1.0f;
	m_Sphere[1].fScaling = 2.0f;
	m_Sphere[1].vTans = D3DXVECTOR3( 5, 0, 0 );

	m_bIsCollision = FALSE;

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
}

void CGameEdu01::OnRender()
{	
	char string[100];
	RECT rt;
	D3DXMATRIX matWorld, matScale, matTrans;

	for( int i = 0 ; i < 2 ; i++ )
	{
		D3DXMatrixTranslation( &matTrans, m_Sphere[i].vTans.x, m_Sphere[i].vTans.y, m_Sphere[i].vTans.z );
		D3DXMatrixScaling( &matScale, m_Sphere[i].fScaling, m_Sphere[i].fScaling, m_Sphere[i].fScaling );
		matWorld = matScale * matTrans;
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		m_pMesh->DrawSubset(0);
	}

	SetRect( &rt, 10, 10, 0, 0 );

	if( m_bIsCollision )
		m_pFont->DrawText( NULL,  "충돌", -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	else
		m_pFont->DrawText( NULL,  "비충돌", -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );	
}

void CGameEdu01::OnUpdate()
{	
	DWORD dwCurTime = GetTickCount();
	static DWORD dwOldTime = GetTickCount();
	m_dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;

	if( GetAsyncKeyState( VK_LEFT ) )
		m_Sphere[0].vTans.x -= m_dwElapsedTime * 0.003f;

	if( GetAsyncKeyState( VK_RIGHT ) )
		m_Sphere[0].vTans.x += m_dwElapsedTime * 0.003f;

	m_bIsCollision = CheckSphereIntersect( m_Sphere[0].vTans, m_Sphere[0].fRadius * m_Sphere[0].fScaling, 
		m_Sphere[1].vTans, m_Sphere[1].fRadius * m_Sphere[1].fScaling );
}

void CGameEdu01::OnRelease()
{		
	m_pMesh->Release();
	m_pFont->Release();
}
	