#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include <D3dx9shape.h>
#include "Ground.h"

struct BULLET
{
	int nLife;
	D3DXVECTOR3 vPos;
	D3DXMATRIX matTranslation;
};

struct BULLET_PROPERTY
{
	float fBulletVelcoty;
	float fScale;		
	D3DXMATRIX matScale;	

	// 충돌 좌표
	D3DXVECTOR3 vMin, vMax;
};

struct PLAYER
{
	int nLife;
	D3DXVECTOR3 vPos;
	float fScale;
	float fRotationY;
	float fVelocity;
	
	// 미사일에 대한 속성	
	DWORD dwBulletFireTime;
	DWORD dwOldBulletFireTime;

	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotationY;
	D3DXMATRIX matTranslation;	

	// 주인공 충돌 박스
	D3DXVECTOR3 vMin, vMax;
};


struct ENEMY
{
	int nLife;
	DWORD dwAppearTime;
	D3DXVECTOR3 vPos;

	DWORD dwOldFireTime; // 미사일 발사 이전 시간
	D3DXMATRIX matTranslation;
};

struct ENEMY_PROPERTY
{
	float fMoveVelcoty;
	float fScale;		
	
	DWORD dwFireTime; // 미사일 발사 시간 간격
	D3DXMATRIX matScale;	

	D3DXVECTOR3 vMin, vMax; // 적 미사일 충돌 박스
};


class CGameEdu01 : public CD3DApp
{
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;
	
	CGround m_Ground;

	LPD3DXMESH m_pTeapotMesh; // 주인공	
	LPD3DXMESH m_pPlayerBulletMesh; // 주인공 미사일 (원기둥)
	LPD3DXMESH m_pEnemyBoxMesh;
	LPD3DXMESH m_pEnemyBulletMesh;

	// 주인공 
	PLAYER m_sPlayer;
	BULLET_PROPERTY m_sPlayerBulletProperty;
	BULLET m_sPlayerBullet[10];

	// 적 캐릭터 
	ENEMY_PROPERTY m_EnemyProperty;
	ENEMY m_Enemy[100];   //적 캐릭터 100개 
	BULLET_PROPERTY m_EnemyBulletProperty;
	BULLET m_EnemyBullet[100];
	int m_nEnemyIndex;
	
	DWORD m_dwElapsedTime;

	// 스테이지 정보
	DWORD m_dwGameStartTime;
	DWORD m_dwGameElapsedTime;	

	BOOL CheckCubeIntersection( D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1, D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2 );
public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

