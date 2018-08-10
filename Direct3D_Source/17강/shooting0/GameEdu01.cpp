#include "StdAfx.h"
#include "GameEdu01.h"
#include <D3dx9math.h>
#include <stdio.h>
#include <string>
using namespace std;

string g_strBGSound[] = { "init.ogg", "war.mp3", "gameover.mp3" };
string g_strEffectSound[] = { "story.mp3", "stage.ogg", "play_fire.wav", "enemy_fire.wav", "collision.wav" };

extern HWND g_hWnd;

CGameEdu01::CGameEdu01(void)
{
	m_nGameState = INIT;
}


CGameEdu01::~CGameEdu01(void)
{
}

BOOL CGameEdu01::CheckCubeIntersection( D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1, D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2 )
{
	if( vMin1->x < vMax2->x && vMax1->x > vMin2->x &&
		vMin1->y < vMax2->y && vMax1->y > vMin2->y && 
		vMin1->z < vMax2->z && vMax1->z > vMin2->z )
		return TRUE;

	return FALSE;
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

			 // Note: 메쉬 생성
			D3DXCreateTeapot( m_pd3dDevice,  &m_pTeapotMesh, NULL );	
			D3DXCreateCylinder( m_pd3dDevice, 2.0f, 2.0f, 10.0f, 15, 10, &m_pPlayerBulletMesh, NULL);
			D3DXCreateBox( m_pd3dDevice, 1.0f, 1.0f, 1.0f, &m_pEnemyBoxMesh, NULL );
			D3DXCreateSphere( m_pd3dDevice, 0.2f, 20, 10, &m_pEnemyBulletMesh, NULL );	
			m_Ground.Create( m_pd3dDevice, 20, 6, 2.0f );

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
			
			m_FMODSound.CreateBGSound( 3, g_strBGSound );			
			m_FMODSound.PlaySoundBG( 0 );
			m_dwGameStartTime = GetTickCount();
			break;

	case LOADING:		  			
			m_FMODSound.StopSoundBG(0);

			m_FMODSound.CreateEffectSound( 5, g_strEffectSound );
			// 주인공의 최대 최소 정점 값 <= 모델 좌표
			D3DXVECTOR3 *pVertices;
			m_pTeapotMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
			D3DXComputeBoundingBox( pVertices, m_pTeapotMesh->GetNumVertices(), D3DXGetFVFVertexSize( m_pTeapotMesh->GetFVF()), &m_sPlayer.vMin, &m_sPlayer.vMax );
			m_pTeapotMesh->UnlockVertexBuffer();	


			// 주인공 미사일 최대 최소정점 구하기
			m_pPlayerBulletMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
			D3DXComputeBoundingBox( pVertices, m_pPlayerBulletMesh->GetNumVertices(), D3DXGetFVFVertexSize( m_pPlayerBulletMesh->GetFVF()), &m_sPlayerBulletProperty.vMin, &m_sPlayerBulletProperty.vMax );
			m_pPlayerBulletMesh->UnlockVertexBuffer();
						
			// 적 충돌 좌표 	
			m_pEnemyBoxMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
			D3DXComputeBoundingBox( pVertices, m_pEnemyBoxMesh->GetNumVertices(), D3DXGetFVFVertexSize( m_pEnemyBoxMesh->GetFVF()), &m_EnemyProperty.vMin, &m_EnemyProperty.vMax );
			m_pEnemyBoxMesh->UnlockVertexBuffer();

			// 적 총알 
			m_pEnemyBulletMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
			D3DXComputeBoundingBox( pVertices, m_pEnemyBoxMesh->GetNumVertices(), D3DXGetFVFVertexSize( m_pEnemyBulletMesh->GetFVF()), &m_EnemyBulletProperty.vMin, &m_EnemyBulletProperty.vMax );
			m_pEnemyBulletMesh->UnlockVertexBuffer();				
			break;
	case READY:			
			m_FMODSound.PlaySoundEffect( 1 );

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
			m_sPlayer.fScale = 0.7f;
			m_sPlayer.fRotationY = -D3DXToRadian( 90 );
			m_sPlayer.vPos = D3DXVECTOR3( 0.0f, 0.0f, -9 * 2.0f );
			m_sPlayer.fVelocity = 0.005f;	
			m_sPlayer.dwBulletFireTime = 400;
			m_sPlayer.dwOldBulletFireTime = GetTickCount();		
			for( i = 0 ; i < 10 ;  i++ )
				m_sPlayerBullet[i].nLife = 0;
	
			D3DXMatrixScaling( &m_sPlayer.matScale, m_sPlayer.fScale, m_sPlayer.fScale, m_sPlayer.fScale );
			D3DXMatrixTranslation(  &m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z );
			D3DXMatrixRotationY( &m_sPlayer.matRotationY, m_sPlayer.fRotationY );

			// Note: 주인공 총알 설정
			m_sPlayerBulletProperty.fBulletVelcoty = 0.01f;
			m_sPlayerBulletProperty.fScale = 0.08f;
			D3DXMatrixScaling( &m_sPlayerBulletProperty.matScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale );

			// Note: 적 캐릭터 설정
			m_EnemyProperty.fScale = 1.0f;
			m_EnemyProperty.fMoveVelcoty = 0.003f;
			m_EnemyProperty.dwFireTime = 1600;	
			D3DXMatrixScaling( &m_EnemyProperty.matScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale );

			int nRandomNum;
			for( i = 0 ; i < 100 ; i++ )
			{
				m_Enemy[i].nLife = 0;
				m_Enemy[i].dwAppearTime = 2000 + (i*800 ) ;
				m_Enemy[i].dwOldFireTime = GetTickCount();
				nRandomNum = rand() % 7;
				if( nRandomNum % 2 )
					m_Enemy[i].vPos.x = -1.0f * nRandomNum;
				else
					m_Enemy[i].vPos.x = (float)nRandomNum;

				m_Enemy[i].vPos.y = 0.0f;
				m_Enemy[i].vPos.z = 20.0f;

				D3DXMatrixTranslation( &m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z );		
			}		

			// Note: 적 총알 설정
			m_EnemyBulletProperty.fBulletVelcoty = 0.01f;	
			for( i = 0 ; i < 100 ; i++)
				m_EnemyBullet[i].nLife = 0;

			m_nGrade = 0; // 점수 초기화
			m_sPlayer.nLife = 3;	
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
		m_nEnemyCount = 100;
		break;

	case RUN:
		m_Ground.OnRender();	

		// 주인공 총알
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

		// 적 총알 
		for( i = 0 ; i < 100 ; i++ )
		{
			if( m_EnemyBullet[i].nLife > 0 )
			{
				matWorld = m_EnemyBullet[i].matTranslation;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
				m_pEnemyBulletMesh->DrawSubset(0);
			}
		}
	
		// 적 캐릭터		
		for( i = 0 ; i < m_nEnemyIndex ; i++ )
		{
			if( m_Enemy[i].nLife )
			{
				matWorld = m_EnemyProperty.matScale * m_Enemy[i].matTranslation;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
				m_pEnemyBoxMesh->DrawSubset(0);			
			}
		}

		// 주인공 	
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
		SetRect( &rt, 220, 130, 0, 0 );
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

	m_FMODSound.Update();
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
			m_FMODSound.PlaySoundBG(1); // 본 게임 배경음
		}
		break;
	case RUN:		
		if( GetAsyncKeyState( VK_LEFT ) < 0 )
		{
			if( m_sPlayer.vPos.x - (m_dwElapsedTime * m_sPlayer.fVelocity) >= -6.0f )
				m_sPlayer.vPos.x -= m_dwElapsedTime * m_sPlayer.fVelocity;
		}

		if( GetAsyncKeyState( VK_RIGHT ) < 0 )
		{
			if( m_sPlayer.vPos.x + (m_dwElapsedTime * m_sPlayer.fVelocity) <= 6.0f )
				m_sPlayer.vPos.x += m_dwElapsedTime * m_sPlayer.fVelocity;
		}

		if( GetAsyncKeyState( VK_UP ) < 0 )
		{
			if( m_sPlayer.vPos.z + (m_dwElapsedTime * m_sPlayer.fVelocity ) <= 20.0f )
				m_sPlayer.vPos.z += m_dwElapsedTime * m_sPlayer.fVelocity;
		}
		if( GetAsyncKeyState( VK_DOWN ) < 0 )
		{
			if( m_sPlayer.vPos.z - (m_dwElapsedTime * m_sPlayer.fVelocity ) >= -19.0f )
				m_sPlayer.vPos.z -= m_dwElapsedTime * m_sPlayer.fVelocity;
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

						m_FMODSound.PlaySoundEffect(2);
						break;
					}
				}
			}
		}

		// 적 캐릭터 출현
		for( i = m_nEnemyIndex ; i < 100 ; i++ )
		{
			if( m_Enemy[i].dwAppearTime <= m_dwGameElapsedTime )
			{
					m_Enemy[i].nLife = 1;
					m_nEnemyIndex++;
			}else{
					break;
			}		
		}

		// 적 캐릭터 이동과 적 캐릭터 총알 발사
		for( i = 0 ; i < m_nEnemyIndex ; i++)
		{
			if( m_Enemy[i].nLife )
			{
				m_Enemy[i].vPos.z -=  m_dwElapsedTime * m_EnemyProperty.fMoveVelcoty;
				if( m_Enemy[i].vPos.z <= -20.0f ) // 경계 영역
				{
					m_Enemy[i].nLife = 0; 
					m_nEnemyCount--;    // 적 캐릭터의 개수 0 이되면 종료 
					if( m_nEnemyCount == 0 )
					{
						m_nGameState = STOP;
					}
					continue;
				}

				D3DXMatrixTranslation( &m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z );

				// Note: 살아 있는 적 캐릭터만이 총알을 발사할 수 있으므로
				if( dwCurTime - m_Enemy[i].dwOldFireTime >= m_EnemyProperty.dwFireTime )
				{
					m_Enemy[i].dwOldFireTime = dwCurTime;
					for( j = 0 ; j < 100 ; j++ )
					{
						if( !m_EnemyBullet[j].nLife )
						{
							m_EnemyBullet[j].nLife = 1;
							m_EnemyBullet[j].vPos = m_Enemy[i].vPos;
							m_EnemyBullet[j].vPos.z -= 0.5f; // 약간 앞에서 총알 발사가 되도록 하기 위해									
							break; // <-- 이게 중요하다. 총알 하나만 살리는 부분의 끝 
						}
					}
				}
			}
		}

		// 적 캐릭터 총알 이동
		for( i = 0 ; i < 100 ; i++ )
		{
			if( m_EnemyBullet[i].nLife > 0 )
			{
				m_EnemyBullet[i].vPos.z -= m_dwElapsedTime * m_EnemyBulletProperty.fBulletVelcoty;
				if( m_EnemyBullet[i].vPos.z <= -20.0f ) // 경계 영역 충돌
					m_EnemyBullet[i].nLife = 0;
				else
					D3DXMatrixTranslation( &m_EnemyBullet[i].matTranslation, m_EnemyBullet[i].vPos.x, m_EnemyBullet[i].vPos.y, m_EnemyBullet[i].vPos.z );
			}
		}

		// Note: 주인공 총알 이동
		for( i = 0 ; i < 10 ; i++ )
		{
			if( m_sPlayerBullet[i].nLife > 0 )
			{
				m_sPlayerBullet[i].vPos.z += m_dwElapsedTime * m_sPlayerBulletProperty.fBulletVelcoty;
				if( m_sPlayerBullet[i].vPos.z >= 20.0f ) // 경계 영역 충돌
					m_sPlayerBullet[i].nLife = 0;
				else
					D3DXMatrixTranslation( &m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z );
			}
		}		

		// Note: 주인공과 적 캐릭터 충돌
		D3DXMatrixTranslation(  &m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z );	
	
		// 주인공의 충돌 박스
		matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation;	
		D3DXVec3TransformCoord( &vMin1, &m_sPlayer.vMin, &matWorld);	
		D3DXVec3TransformCoord( &vMax1, &m_sPlayer.vMax, &matWorld);

	//	vMin1.x = -1*vMin1.x;
	//	vMax1.x = -1*vMax1.x;

		if( vMin1.x > vMax1.x )  // 회전이 있으므로 최소의 값을 재계산한다.
		{
			fTemp = vMin1.x;
			vMin1.x = vMax1.x;
			vMax1.x = fTemp;
		}

		if( vMin1.y > vMax1.y )
		{
			fTemp = vMin1.y;
			vMin1.y = vMax1.y;
			vMax1.y = fTemp;
		}

		if( vMin1.z > vMax1.z )
		{
			fTemp = vMin1.z;
			vMin1.z = vMax1.z;
			vMax1.z = fTemp;
		}

		for( i = 0 ; i < m_nEnemyIndex ; i++ )
		{
			if( m_Enemy[i].nLife > 0 )
			{
				D3DXMatrixTranslation( &m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z );
				matWorld = m_EnemyProperty.matScale * m_Enemy[i].matTranslation;

				D3DXVec3TransformCoord( &vMin2, &m_EnemyProperty.vMin, &matWorld);
				D3DXVec3TransformCoord( &vMax2, &m_EnemyProperty.vMax, &matWorld);			
			
				if( CheckCubeIntersection( &vMin1, &vMax1, &vMin2, &vMax2 ) )
				{
					m_Enemy[i].nLife = 0;
					m_sPlayer.nLife--;		
					m_nEnemyCount--;    // 적 캐릭터의 개수 0 이되면 종료 
					m_nGrade += 10; // 주인공이 충돌하면서 죽은 것이므로 점수 가산
					m_FMODSound.PlaySoundEffect(4);

					if( m_sPlayer.nLife == 0 )
					{
						m_nGameState = STOP;
					}					
					
					if( m_nEnemyCount == 0 )
					{
						m_nGameState = STOP;
					}
				}
			}
		}

		// Note: 주인공과 적 총알 충돌
		for( i = 0 ; i < 100 ; i++ )
		{
			if( m_EnemyBullet[i].nLife > 0 )
			{
				D3DXMatrixTranslation( &m_EnemyBullet[i].matTranslation, m_EnemyBullet[i].vPos.x, m_EnemyBullet[i].vPos.y, m_EnemyBullet[i].vPos.z );
				matWorld = m_EnemyBullet[i].matTranslation;
				D3DXVec3TransformCoord( &vMin2, &m_EnemyBulletProperty.vMin, &matWorld);
				D3DXVec3TransformCoord( &vMax2, &m_EnemyBulletProperty.vMax, &matWorld);	

				if( CheckCubeIntersection( &vMin1, &vMax1, &vMin2, &vMax2 ) )
				{
					m_EnemyBullet[i].nLife = 0;
					m_sPlayer.nLife--;		
					m_FMODSound.PlaySoundEffect(4);
					if( m_sPlayer.nLife == 0 )
					{
						m_nGameState = STOP;
					}				
				}
			}
		}

		// Note: 주인공 총알과 적 캐릭터 충돌
		for( i = 0 ; i < 10 ; i++ )
		{
			if( m_sPlayerBullet[i].nLife > 0 )
			{
				D3DXMatrixTranslation( &m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z );
				matWorld = m_sPlayerBulletProperty.matScale * m_sPlayerBullet[i].matTranslation;
				D3DXVec3TransformCoord( &vMin1, &m_sPlayerBulletProperty.vMin, &matWorld);	
				D3DXVec3TransformCoord( &vMax1, &m_sPlayerBulletProperty.vMax, &matWorld);
		
				// Note: 적 캐릭터와 충돌 체크
				for( j = 0 ; j < m_nEnemyIndex ; j++ )
				{
					if( m_Enemy[j].nLife > 0 )
					{
						D3DXMatrixTranslation( &m_Enemy[j].matTranslation, m_Enemy[j].vPos.x, m_Enemy[j].vPos.y, m_Enemy[j].vPos.z );
						matWorld = m_EnemyProperty.matScale * m_Enemy[j].matTranslation;

						D3DXVec3TransformCoord( &vMin2, &m_EnemyProperty.vMin, &matWorld);
						D3DXVec3TransformCoord( &vMax2, &m_EnemyProperty.vMax, &matWorld);			
			
						if( CheckCubeIntersection( &vMin1, &vMax1, &vMin2, &vMax2 ) )
						{
							m_Enemy[j].nLife = 0;
							m_sPlayerBullet[i].nLife = 0;		
							m_nGrade += 10; // 점수 가산

							m_nEnemyCount--;    // 적 캐릭터의 개수 0 이되면 종료 
							m_FMODSound.PlaySoundEffect(4);
							if( m_nEnemyCount == 0 )
							{
								m_nGameState = STOP;
							}
							break;
						}
					}
				}

				// Note: 주인공 총알과 적 총알 충돌 체크
				if( m_sPlayerBullet[i].nLife > 0 )
				{
					for( j = 0 ; j < 100 ; j++ )
					{
						if( m_EnemyBullet[j].nLife > 0 )
						{
							D3DXMatrixTranslation( &m_EnemyBullet[j].matTranslation, m_EnemyBullet[j].vPos.x, m_EnemyBullet[j].vPos.y, m_EnemyBullet[j].vPos.z );
							matWorld = m_EnemyBullet[j].matTranslation;
							D3DXVec3TransformCoord( &vMin2, &m_EnemyBulletProperty.vMin, &matWorld);
							D3DXVec3TransformCoord( &vMax2, &m_EnemyBulletProperty.vMax, &matWorld);	

							if( CheckCubeIntersection( &vMin1, &vMax1, &vMin2, &vMax2 ) )
							{
								m_EnemyBullet[j].nLife = 0;
								m_sPlayerBullet[i].nLife = 0;		
								m_FMODSound.PlaySoundEffect(4);
								break;
							}
						}
					}
				}
			}
		}

		D3DXMatrixTranslation(  &m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z );
		m_sPlayer.matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation;	
		break;

	case STOP:
		m_FMODSound.StopSoundBG(1);
		if( m_sPlayer.nLife < 0 || m_nEnemyCount > 0 )
		{			
			m_nGameState = FAILED;
			m_FMODSound.PlaySoundBG(2);
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
			OnInit();
		}
		break;
		
	case FAILED:
		if( GetAsyncKeyState( 'Y' ) < 0 )
		{
			m_nGameState = READY;
			m_nGrade = 0;
			m_FMODSound.StopSoundBG(2);
			dwReadyTime = 0;
			OnInit();
			break;
		} 

		if( GetAsyncKeyState( 'N' ) < 0 )
		{
			m_nGameState = END;	
			dwReadyTime = 0;
			m_nTotalGrade += m_nGrade;		
			m_FMODSound.StopSoundBG(2);
			break;
		} 	
		break;
	}	
}

void CGameEdu01::OnRelease()
{	
	m_FMODSound.ReleaseSound();
	m_pFont->Release();
	m_pFont2->Release();
	m_pFont3->Release();
	m_pEnemyBoxMesh->Release();
	m_pEnemyBulletMesh->Release();
	m_pTeapotMesh->Release();
	m_pPlayerBulletMesh->Release();
	m_Ground.OnRelease();
}