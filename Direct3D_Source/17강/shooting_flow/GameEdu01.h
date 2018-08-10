#pragma once
#include "d3dapp.h"
#include <d3dx9math.h>
#include <D3dx9shape.h>

enum GAME_STATE { INIT, LOADING, READY, RUN, STOP, SUCCESS, FAILED, END };

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

	// 주인공 충돌 박스
	D3DXVECTOR3 vMin, vMax;
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

	LPD3DXMESH m_pTeapotMesh; // 주인공	
	LPD3DXMESH m_pPlayerBulletMesh; // 주인공 총알 (원기둥)
	LPD3DXMESH m_pEnemyBoxMesh;
	LPD3DXMESH m_pEnemyBulletMesh;

	PLAYER m_sPlayer;

	// 폰트
	LPD3DXFONT m_pFont, m_pFont2, m_pFont3;

	
	DWORD m_dwElapsedTime;

	// 스테이지 정보
	DWORD m_dwGameStartTime;
	DWORD m_dwGameElapsedTime;	

	int m_nStage;
	GAME_STATE m_nGameState;
	int m_nLoadingCount;
	int m_nGrade;
	int m_nTotalGrade;

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

