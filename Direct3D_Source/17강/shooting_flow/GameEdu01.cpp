#include "StdAfx.h"
#include "GameEdu01.h"
#include <D3dx9math.h>
#include <stdio.h>
#include <string>
using namespace std;

extern HWND g_hWnd;

CGameEdu01::CGameEdu01(void)
{
	m_nGameState = INIT;
}


CGameEdu01::~CGameEdu01(void)
{
}

void CGameEdu01::OnInit()
{
	int i;

	RECT rect;
	D3DVIEWPORT9 vp;
	
	switch( m_nGameState )
	{
	case INIT :
			GetClientRect( m_hWnd, &rect );

			vp.X      = 0;
			vp.Y      = 0;
			vp.Width  = rect.right - rect.left;  // 좌표개념이 아니라 크기 개념 800, 600 계산됨
			vp.Height = rect.bottom - rect.top;
			vp.MinZ   = 0.0f;
			vp.MaxZ   = 1.0f;

			m_Eye.x = 0.0f;
			m_Eye.y = 0.0f;
			m_Eye.z = -12.0f;	

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

			 // Note: 캐릭터 메쉬 생성
			D3DXCreateTeapot( m_pd3dDevice,  &m_pTeapotMesh, NULL );	
			D3DXCreateCylinder( m_pd3dDevice, 2.0f, 2.0f, 10.0f, 15, 10, &m_pPlayerBulletMesh, NULL);
			D3DXCreateBox( m_pd3dDevice, 1.0f, 1.0f, 1.0f, &m_pEnemyBoxMesh, NULL );
			D3DXCreateSphere( m_pd3dDevice, 0.2f, 20, 10, &m_pEnemyBulletMesh, NULL );	
				

			// 폰트
			D3DXCreateFont( m_pd3dDevice, 50, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
									  OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
									  "Arial", &m_pFont );

			D3DXCreateFont( m_pd3dDevice, 30, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
									  OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
									  "Arial", &m_pFont2 );
			D3DXCreateFont( m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
									  OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
									  "Arial", &m_pFont3 );				
			
			m_dwGameStartTime = GetTickCount();
			break;

	case LOADING:		 		
			
			// 주인공의 최대 최소 정점 값 <= 모델 좌표
			
			// 주인공 미사일 최대 최소정점 구하기	
						
			// 적 충돌 좌표 			

			// 적 총알 
					
			break;

	case READY:	
			// 게임 뷰로 설정
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

			// Note: 주인공 설정			
		
			// Note: 주인공 총알 설정			

			// Note: 적 캐릭터 설정

			// Note: 적 총알 설정	

			m_sPlayer.nLife = 3;	
			break;
	}	
}

void CGameEdu01::OnUpdate()
{	
	int i, j;
	
	static DWORD OldTime;
	DWORD dwCurTime = GetTickCount();
	m_dwElapsedTime = dwCurTime - OldTime;
	OldTime = dwCurTime;
	D3DXVECTOR3 vMin1, vMax1, vMin2, vMax2;
	D3DXMATRIX matWorld;
	float fTemp;
	static DWORD dwReadyTime; // 준비 경과 시간을 누적시키기 위해서
		
	// 게임 경과 시간 체크 부분
	m_dwGameElapsedTime = dwCurTime - m_dwGameStartTime;

	switch( m_nGameState )
	{
	case INIT:
		if( GetAsyncKeyState( VK_SPACE ) < 0 )
		{
			m_nGameState = LOADING;
			OnInit();
		}
		break;
	case LOADING:
		Sleep( 100 );
		m_nLoadingCount++;
		if( m_nLoadingCount == 50 )
		{
			m_nGameState = READY;
			m_nStage++;
			dwReadyTime = 0;
			OnInit();
		}
		break;
	case READY:
		dwReadyTime += m_dwElapsedTime;
		if( dwReadyTime >= 2000 ) //3초면 게임으로 진행
		{
			m_nGameState = RUN;					
		}
		break;
	case RUN:	

		// 키보드 처리 
		if( GetAsyncKeyState( 'D' ) < 0 )
		{
			 m_sPlayer.nLife = 0; 
			 m_nGameState = STOP;			 
		}

		if( GetAsyncKeyState( 'S' ) < 0 )
		{
			 m_sPlayer.nLife = 3; 
			 m_nGameState = STOP;			
		}

		break;

	case STOP:	
		if( m_sPlayer.nLife <= 0 )
		{			
			m_nGameState = FAILED;			
		}else{
			m_nGameState = SUCCESS;			
		}

		dwReadyTime = 0;
		break;

	case SUCCESS:
		dwReadyTime += m_dwElapsedTime;
		if( dwReadyTime >= 3000 ) //3초면 게임으로 진행
		{
			m_nGameState = READY;
			m_nStage++;
			m_nTotalGrade += m_nGrade;
			dwReadyTime = 0;
			OnInit();
		}
		break;
		
	case FAILED:
		if( GetAsyncKeyState( 'Y' ) < 0 )
		{
			m_nGameState = READY;			
			dwReadyTime = 0;
			OnInit();
			break;
		} 

		if( GetAsyncKeyState( 'N' ) < 0 )
		{
			m_nGameState = END;	
			dwReadyTime = 0;
			m_nTotalGrade += m_nGrade;			
			break;
		} 	
		break;
	}	
}

void CGameEdu01::OnRender()
{		
	int i;
	D3DXMATRIX matWorld, matScale, matTrans , matRotation;
	RECT rt;
	char string[100];

	switch( m_nGameState )
	{
	case INIT:
		D3DXMatrixScaling( &matScale, 2.0f, 2.0f, 2.0f );
		D3DXMatrixTranslation( &matTrans, 0.0f, -1.0f, 0.0f );
		D3DXMatrixRotationY( &matRotation, GetTickCount() * 0.001f );

		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		matWorld = matScale * matRotation * matTrans;
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pTeapotMesh->DrawSubset(0);	

		D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 1.0f );
		D3DXMatrixTranslation( &matTrans, 3.0f, 1.0f, 0.0f );
		D3DXMatrixRotationX( &matRotation, GetTickCount() * 0.002f );
		matWorld = matScale * matRotation * matTrans;
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pPlayerBulletMesh->DrawSubset(0);	
				
		SetRect( &rt, 220, 130, 0, 0 );
		m_pFont->DrawText( NULL, "3D  Sogo   Game", -1, &rt, DT_NOCLIP,
								   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		SetRect( &rt, 250, 500, 0, 0 );
		m_pFont2->DrawText( NULL, "Space 키를 눌러 주세요 !!", -1, &rt, DT_NOCLIP,
								   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		break;
	case LOADING:
		SetRect( &rt, 100, 430, 0, 0 );
		m_pFont->DrawText( NULL, "Loading ...", -1, &rt, DT_NOCLIP,
								   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		for( i = 0 ; i < m_nLoadingCount ; i++ )
		{
			SetRect( &rt, 150 + i*10, 500, 0, 0 );
			m_pFont2->DrawText( NULL, "■", -1, &rt, DT_NOCLIP,
									   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		}
		break;

	case READY:
		SetRect( &rt, 300, 250, 0, 0 );
		sprintf( string, "Stage %d", m_nStage );
		m_pFont->DrawText( NULL, string, -1, &rt, DT_NOCLIP,
								   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		
		break;

	case RUN:
		

		// 주인공 총알 렌더링
		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
		// 적 총알 렌더링	
	
		// 적 캐릭터 렌더링

		// 주인공 렌더링
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);	
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_sPlayer.matWorld );	
		m_pTeapotMesh->DrawSubset(0);		
		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		SetRect( &rt, 10, 20, 0, 0 );
		sprintf( string, "Total Score : %d   Score :  %d", m_nTotalGrade, m_nGrade );
		m_pFont3->DrawText( NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

		SetRect( &rt, 680, 20, 0, 0 );
		sprintf( string, "%s", "♥" );
		for( i = 0 ; i < m_sPlayer.nLife ; i++ )
		{
			rt.left = 680 + i * 15;
			m_pFont2->DrawText( NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		}
		break;
	case SUCCESS:
		SetRect( &rt, 260, 280, 0, 0 );
		m_pFont->DrawText( NULL, "SUCCESS", -1, &rt, DT_NOCLIP,
								   D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		break;
	case FAILED:
		SetRect( &rt, 220, 230, 0, 0 );
		m_pFont2->DrawText( NULL, "다시 하시겠습니까? (Yes/No)", -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		break;
	case END:		
		SetRect( &rt, 260, 280, 0, 0 );
		sprintf( string, "Total Score : %d", m_nTotalGrade );
		m_pFont2->DrawText( NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );			
		break;
	}	
}



void CGameEdu01::OnRelease()
{	

	m_pFont->Release();
	m_pFont2->Release();
	m_pFont3->Release();

	m_pEnemyBoxMesh->Release();
	m_pEnemyBulletMesh->Release();
	m_pTeapotMesh->Release();
	m_pPlayerBulletMesh->Release();
}

