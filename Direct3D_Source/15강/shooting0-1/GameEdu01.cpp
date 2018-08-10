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
	vp.Width  = rect.right - rect.left;  // 좌표개념이 아니라 크기 개념 800, 600 계산됨
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 10.0f;
	m_Eye.z =-32.0f;	

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

	D3DXCreateTeapot( m_pd3dDevice,  &m_pTeapotMesh, NULL );	
	D3DXCreateCylinder( m_pd3dDevice, 2.0f, 2.0f, 10.0f, 15, 10, &m_pPlayerBulletMesh, NULL);
	m_Ground.Create( m_pd3dDevice, 20, 6, 2.0f );

	// Note: 주인공 설정
	m_sPlayer.nLife = 10;
	m_sPlayer.fScale = 0.7f;
	m_sPlayer.fRotationY = -D3DXToRadian( 90 );
	m_sPlayer.vPos = D3DXVECTOR3( 0.0f, 0.0f, -9 * 2.0f );
	m_sPlayer.fVelocity = 0.005f;	
	m_sPlayer.dwBulletFireTime = 400;
	m_sPlayer.dwOldBulletFireTime = GetTickCount();
	
	D3DXMatrixScaling( &m_sPlayer.matScale, m_sPlayer.fScale, m_sPlayer.fScale, m_sPlayer.fScale );
	D3DXMatrixTranslation(  &m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z );
	D3DXMatrixRotationY( &m_sPlayer.matRotationY, m_sPlayer.fRotationY );

	// Note: 주인공 총알 설정
	m_sPlayerBulletProperty.fBulletVelcoty = 0.01f;
	m_sPlayerBulletProperty.fScale = 0.08f;
	D3DXMatrixScaling( &m_sPlayerBulletProperty.matScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale );
}

void CGameEdu01::OnUpdate()
{	
	int i, j;
	
	static DWORD OldTime;
	DWORD dwCurTime = GetTickCount();
	DWORD dwElapsedTime = dwCurTime - OldTime;
	OldTime = dwCurTime;
		
	if( GetAsyncKeyState( VK_LEFT ) < 0 )
	{
		if( m_sPlayer.vPos.x - (dwElapsedTime * m_sPlayer.fVelocity) >= -6.0f )
			m_sPlayer.vPos.x -= dwElapsedTime * m_sPlayer.fVelocity;
	}

	if( GetAsyncKeyState( VK_RIGHT ) < 0 )
	{
		if( m_sPlayer.vPos.x + (dwElapsedTime * m_sPlayer.fVelocity) <= 6.0f )
			m_sPlayer.vPos.x += dwElapsedTime * m_sPlayer.fVelocity;
	}

	if( GetAsyncKeyState( VK_UP ) < 0 )
	{
		if( m_sPlayer.vPos.z + (dwElapsedTime * m_sPlayer.fVelocity ) <= 20.0f )
			m_sPlayer.vPos.z += dwElapsedTime * m_sPlayer.fVelocity;
	}
	if( GetAsyncKeyState( VK_DOWN ) < 0 )
	{
		if( m_sPlayer.vPos.z - (dwElapsedTime * m_sPlayer.fVelocity ) >= -19.0f )
			m_sPlayer.vPos.z -= dwElapsedTime * m_sPlayer.fVelocity;
	}

	if( GetAsyncKeyState( 'S' ) < 0 )  // 주인공 미사일 발사
	{
		if( dwCurTime - m_sPlayer.dwOldBulletFireTime >= m_sPlayer.dwBulletFireTime )
		{
			m_sPlayer.dwOldBulletFireTime = dwCurTime;

			for( i = 0 ; i < 10 ; i++ )
			{
				if( m_sPlayerBullet[i].nLife <= 0 )
				{
					m_sPlayerBullet[i].nLife = 1;
					m_sPlayerBullet[i].vPos = m_sPlayer.vPos;
					m_sPlayerBullet[i].vPos.z += 1.0f;
					break;
				}
			}
		}
	}	

	// Note: 주인공 총알 이동
	for( i = 0 ; i < 10 ; i++ )
	{
		if( m_sPlayerBullet[i].nLife > 0 )
		{
			m_sPlayerBullet[i].vPos.z += dwElapsedTime * m_sPlayerBulletProperty.fBulletVelcoty;
			if( m_sPlayerBullet[i].vPos.z >= 20.0f ) // 경계 영역 충돌
				m_sPlayerBullet[i].nLife = 0;
			else
				D3DXMatrixTranslation( &m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z );
		}
	}

	D3DXMatrixTranslation(  &m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z );
	m_sPlayer.matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation;		
}

void CGameEdu01::OnRender()
{		
	int i;
	D3DXMATRIX matWorld;
	m_Ground.OnRender();	

	// 주인공 미사일
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for( i = 0 ; i < 10 ; i++ )
	{
		if( m_sPlayerBullet[i].nLife > 0 )
		{
			matWorld = m_sPlayerBulletProperty.matScale * m_sPlayerBullet[i].matTranslation;
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			m_pPlayerBulletMesh->DrawSubset(0);
		}
	}

	// 주인공 		
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_sPlayer.matWorld );	
	m_pTeapotMesh->DrawSubset(0);			
}

void CGameEdu01::OnRelease()
{	
	m_pTeapotMesh->Release();
	m_pPlayerBulletMesh->Release();
	m_Ground.OnRelease();
}