#include "StdAfx.h"
#include "GameEdu01.h"
#include <D3dx9math.h>
#include <stdio.h>

BOOL CheckCubeIntersection( D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1, 
                             D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2 )
{
	if( vMin1->x <= vMax2->x && vMax1->x >= vMin2->x &&
	   vMin1->y <= vMax2->y && vMax1->y >= vMin2->y && 
	   vMin1->z <= vMax2->z && vMax1->z >= vMin2->z )
	   return TRUE;
	return FALSE;
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
	vp.Width  = rect.right - rect.left;  // ÁÂÇ¥°³³äÀÌ ¾Æ´Ï¶ó Å©±â °³³ä 800, 600 °è»êµÊ
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
           "µ¸À½Ã¼", &m_pFont );	

	D3DXCreateBox( m_pd3dDevice, 1.0f, 1.0f, 1.0f, &m_pMesh, NULL );
	D3DXVECTOR3 *pVertices;

	m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
	D3DXComputeBoundingBox( pVertices, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &m_vMin, &m_vMax );
	m_pMesh->UnlockVertexBuffer();	

	D3DXMATRIX matScale, matTrans, matWorld;
	m_Box[0].fScaling = 2.0f;
	m_Box[0].vTans = D3DXVECTOR3( 5.0f, 0.0f, 0.0f );
	D3DXMatrixTranslation( &matTrans, m_Box[0].vTans.x, m_Box[0].vTans.y, m_Box[0].vTans.z );
	D3DXMatrixScaling( &matScale, m_Box[0].fScaling, m_Box[0].fScaling, m_Box[0].fScaling );
	matWorld = matScale * matTrans;		
	D3DXVec3TransformCoord( &m_Box[0].vMin, &m_vMin, &matWorld );
	D3DXVec3TransformCoord( &m_Box[0].vMax, &m_vMax, &matWorld );
	
	m_Box[1].fScaling = 1.0f;
	m_Box[1].vTans = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXMatrixTranslation( &matTrans, m_Box[1].vTans.x, m_Box[1].vTans.y, m_Box[1].vTans.z );
	D3DXMatrixScaling( &matScale, m_Box[1].fScaling, m_Box[1].fScaling, m_Box[1].fScaling );
	matWorld = matScale * matTrans;		
	D3DXVec3TransformCoord( &m_Box[1].vMin, &m_vMin, &matWorld );
	D3DXVec3TransformCoord( &m_Box[1].vMax, &m_vMax, &matWorld );

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
		D3DXMatrixTranslation( &matTrans, m_Box[i].vTans.x, m_Box[i].vTans.y, m_Box[i].vTans.z );
		D3DXMatrixScaling( &matScale, m_Box[i].fScaling, m_Box[i].fScaling, m_Box[i].fScaling );
		matWorld = matScale * matTrans;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pMesh->DrawSubset(0);
	}

	SetRect( &rt, 10, 10, 0, 0 );

	if( m_bIsCollision )
		m_pFont->DrawText( NULL,  "Ãæµ¹", -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	else
		m_pFont->DrawText( NULL,  "ºñÃæµ¹", -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );	
}

void CGameEdu01::OnUpdate()
{	
	DWORD dwCurTime = GetTickCount();
	static DWORD dwOldTime = GetTickCount();
	m_dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;

	if( GetAsyncKeyState( VK_LEFT ) )
		m_Box[0].vTans.x -= m_dwElapsedTime * 0.003f;

	if( GetAsyncKeyState( VK_RIGHT ) )
		m_Box[0].vTans.x += m_dwElapsedTime * 0.003f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixTranslation( &matTrans, m_Box[0].vTans.x, m_Box[0].vTans.y, m_Box[0].vTans.z );
	D3DXMatrixScaling( &matScale, m_Box[0].fScaling, m_Box[0].fScaling, m_Box[0].fScaling );
	matWorld = matScale * matTrans;		
	D3DXVec3TransformCoord( &m_Box[0].vMin, &m_vMin, &matWorld );
	D3DXVec3TransformCoord( &m_Box[0].vMax, &m_vMax, &matWorld );

	m_bIsCollision = CheckCubeIntersection( &m_Box[0].vMin, &m_Box[0].vMax, &m_Box[1].vMin, &m_Box[1].vMax );
}

void CGameEdu01::OnRelease()
{		
	m_pMesh->Release();
	m_pFont->Release();
}
	