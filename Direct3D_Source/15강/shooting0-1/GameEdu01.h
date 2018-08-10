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
};

struct PLAYER
{
	int nLife;
	D3DXVECTOR3 vPos;
	float fScale;
	float fRotationY;

	float fVelocity;
	
	// 총알에 대한 속성	
	DWORD dwBulletFireTime;
	DWORD dwOldBulletFireTime;

	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotationY;
	D3DXMATRIX matTranslation;		
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
	LPD3DXMESH m_pPlayerBulletMesh; // 주인공 총알 (원기둥)
	
	// 주인공 
	PLAYER m_sPlayer;
	BULLET_PROPERTY m_sPlayerBulletProperty;
	BULLET m_sPlayerBullet[10];

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

