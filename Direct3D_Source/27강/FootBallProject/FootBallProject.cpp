//--------------------------------------------------------------------------------------
// File: FootBallProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"
#include <fmod.h>
#include <Windowsx.h>

#define MAX_LOADSTRING	100
#define GAME_WIDTH		800
#define GAME_HEIGHT		600
#define BALL_X_GAP      20

// 주인공은 LEFT, RIGHT는 좌우이동 동작, DOWN은 실패했을 때 동작, TOP는 슛 동작, STOP은 IDEL 동작으로 되어 있다.
typedef enum _DIRECTION { LEFT, RIGHT, TOP, DOWN, STOP } DIRECTION;
typedef enum _ACTION { PLAYER_MOVE, PLAYER_READY, PLAYER_IDLE, PLAYER_SHOOT } ACTION;  // MOVE는 현재 이동상태, READY는 MOVE가 끝나고 IDLE 또는 MOVE로 넘어가기 위한 전단계, IDLE는 아이들....
typedef enum _BALL_STATE { BALL_SHOOT, BALL_MOVE, BALL_READY, BALL_STOP }  BALL_STATE;
typedef enum _GAME_STATE { GAME_INIT, GAME_LOADING, GAME_READY, GAME_RUN, GAME_STOP, 
	                       GAME_SUCCEED, GAME_FAILED, GAME_RESULT, GAME_EXIT } GAME_STATE;
typedef struct _CUT_INFO
{
	char strFileName[100];
	DWORD dwTransparentColor; // 투명 컬러값
	int  nCount; // 자른 좌표 계수
	RECT *pCutRect;
} CUT_INFO;

typedef struct _POS_INFO
{
	int nCenterX, nCenterY;
	int nCollisionCount;
	RECT *pCollisionRect;
} POS_INFO;

typedef struct _SPRITE 
{
	// cut 정보
	char strFileName[50]; // 이미지 파일명
	DWORD dwTransparentColor; // 투명 컬러값
	int  nCount; // 자른 좌표 계수
	RECT *pCutRect;

	// pos 정보
	POS_INFO *pPosInfo;

	// ani 정보
	DWORD *pAniTime; // 에니메이션 데이터

	// 이동 인덱스 정보	
	DIRECTION nMoveRange[5][2]; // LEFT, RIGHT, TOP, DOWN, STOP 일때 스프라이트 시작 인덱스와 끝 인덱스 저장 배열
} SPRITE;

typedef struct _GEN_SPRITE
{
	// cut 정보
	char strFileName[50]; // 이미지 파일명
	DWORD dwTransparentColor; // 투명 컬러값
	int  nCount; // 자른 좌표 계수
	RECT *pCutRect;

	// pos 정보
	POS_INFO *pPosInfo;

	// ani 정보
	DWORD *pAniTime; // 에니메이션 데이터
} GEN_SPRITE;

// 에니메이션이 없는 스프라이트의 로딩 정보
typedef struct _NO_ANI_SPRITE
{
	// cut 정보
	char strFileName[50]; // 이미지 파일명
	DWORD dwTransparentColor; // 투명 컬러값
	int  nCount; // 자른 좌표 계수
	RECT *pCutRect;

	// pos 정보
	POS_INFO *pPosInfo;
} NO_ANI_SPRITE;  

typedef struct _MOVE_PROPERTY
{
	DIRECTION nDirection; // 방향
	float fVelocity; //이동 속도
	int nAllDistance; // 이동 전체 거리
	DWORD dwDelayTime; // STOP 대기시간
	int nDestX, nDestY; // 최종 좌표
} MOVE_PROPERTY;

typedef struct _PLAYER
{
	int nX, nY;  // 출력 좌표 	
	int nAniSprIndex; // 애니메이션할 스프라이트의 인덱스	
	DWORD dwOldAniTime; // 스프라이트의 변경은 경과시간으로 하므로		
	DIRECTION nDirection; // 이동방향	
	ACTION nAction;	//주인공 상태	
	
	DWORD dwMoveTime;
	DWORD dwOldMoveTIme;
	int   nDistance;
} PLAYER;

// 공은 주인공의 이동에 따라 영향을 받는다.좌우로 이동할 때는 약간 앞에서 공 애니를 시작한다.
typedef struct _BALL
{
	int nX, nY;
	int nAniSprIndex;
	DWORD dwOldAniTime; 
	int nDistance;	
	BOOL bIsShoot;     // 슛팅이 끝나는 때는 골인 또는 노골이 판정되었을 때이다.
	BALL_STATE nBallState;
	DWORD dwOldMoveTime;
	DWORD dwMoveTime;
	DWORD dwAniTime;   // 고정한다.
} BALL;

typedef struct _PATTERN_PROPERTY
{
	int nCount; // 이동 속성 개수
	int nX, nY; // 초기 위치
	MOVE_PROPERTY *pMove; // 이동 개수
} PATTERN_PROPERTY;

typedef struct _ENEMY_PROPERTY
{
	int nX, nY;  // 출력 좌표 <- 패턴에서 결정
	int nTypeIndex; // 선수유형 인덱스  --> SPRITE 인덱스와 연결
	int nAniSprIndex; // 애니메이션할 스프라이트의 인덱스	
	DWORD dwOldAniTime; // 스프라이트의 변경은 경과시간으로 하므로

	int nPatternIndex; // 패턴 인덱스   --> 패턴 링크드 리스트의 인덱스와 연결
	int nMoveIndex;  // 현재 이동경로 인덱스 --> 유동적, 패턴 안에서 이동경로의 변경
	DWORD dwStopElapsedTime;	
	int nAddDistance;
	DWORD dwOldMoveTime; // 이동 시간.....
} ENEMY_PROPERTY;

/*-----------------------------------------------------------------
 스프라이트 정보는 고정했으므로 별도의 스테이지 데이터 정보에 포함시키지 
 않았다. 만약 더 다양하게 적 캐릭터를 출력한다면 이 부분도 스테이지
 정보에 포함시켜 스테이지가 실행되기 전에 매번 로딩하여 사용해야 한다.
----------------------------------------------------------------*/
typedef struct _STAGE_NFO
{
	DWORD dwLimitedTime; // 스테이지 제한 시간
	int   nGoalCount; // 골인 개수
	DWORD dwItemTime; // 아이템 출력 시간, 렌덤하게 출력

	// 패턴 데이터 
	int nPatternCount;
	char (*strPatternFileName)[20]; // 최대 10개까지 저장 가능, 이중포인터로 해도 된다.	

	// 설정된 상대방 선수와 패턴 데이터
	int nEnemyCount;
	int (*nEnemyTypePatIndex)[2]; // [0] 선수 유형 인덱스 [1] 이동패턴 인덱스 
} STAGE_INFO;

typedef struct _EFFECT
{	
	DWORD dwEffectTime;
	DWORD dwStartEffectTime;
	BOOL bLife;
	int nX, nY;
} EFFECT;

DWORD g_dwItemTime; // 아이템 출현 시간 변수
STAGE_INFO g_sStageInfo; // 스테이지 
PATTERN_PROPERTY *g_pPattern;

EFFECT g_EffectText;

SPRITE *g_pSprite[4];
char *g_strSpriteFile[4] = { "punk.spr", "senior.spr", "vampire.spr", "damaka.spr" };
char *g_strDirectionFile[4] = {"punk.dir", "senior.dir", "vampire.dir", "damaka.dir"};
int g_nCurPatternCount;
HDC g_hSpriteDC[4]; // 스프라이트가 4개로 고정되었기에 HDC를 고정하여 선언함
HDC g_hMainDC, g_hMainBallDC, g_hButtonDC;
CUT_INFO g_sItemCutInfo;
POINT g_ptItemPos[3] = { {90, -18}, {180, -17}, {280, -18} }; // Win32 에서 - 좌표는 클리핑된다.

ENEMY_PROPERTY *g_pEnemyProperty;
int g_nEnemyCount; // 상대방 선수 수

char *g_strStageFileName[] = { "1.stg", "2.stg", "3.stg", "4.stg", "5.stg" };
int g_nAdjustX, g_nAdjustY; // 화면에 대한 보정

IDirect3DTexture9 *g_pBackGroundTexture, *g_pPlayerTexture, *g_pBallTexture, 
				  *g_pGoalPostTexture, *g_pTextTexture, *g_pMainTexture, 
				  *g_pButtonTexture, *g_pEnemyTexture[4], *g_pMainBallTexture;
ID3DXSprite *g_pSpriteObject;

SPRITE *g_pPlayerSprite;
PLAYER g_sPlayer;

CUT_INFO g_sBackGroundCutInfo, g_sMainBallCutInfo, g_sButtonCutInfo, g_sTextCutInfo;
int g_nBackGroundIndex[] = { 0, 1, 2, 3, 2, 1, 0, 3, 2 };  // 배경 출력 인덱스 배열이다. 
int g_nBGIndex, g_nBGWidth,  g_nBGHeight;
BALL g_sBall;
GEN_SPRITE *g_pBallSprite;
NO_ANI_SPRITE *g_pGoalPostSprite;
POINT g_ptGoalPost = { 330, 100 };
int g_nDeadLineY, g_nStage = -1;
GAME_STATE g_nGameState = GAME_INIT;

DWORD g_dwOldLoadingBarTime;
int g_nLoadingBarCount;
DWORD g_dwLoadingBarTime = 100;

int g_nMainBallAniIndex = 0;

POINT g_ptMouse;
DWORD g_dwOldReadyTime, g_dwReadyTime = 2000;
int g_nGrade = 0;
int g_nTotalGrade = 0;

typedef struct _GAME_BUTTON
{
	int  nX, nY;
	int  nIndex[2]; // Off / On 인덱스 저장
	BOOL bIsPush;  // 위의 인덱스로 사용 0 off, 1 on
} GAME_BUTTON;

GAME_BUTTON g_sGameButton[5];

DWORD g_dwLimitedTime; // 스테이지 제한 시간
int   g_nGoalCount;    // 골인 개수

DWORD g_dwOldKeyInputTime; // 키 입력 이전시각
DWORD g_dwSucceedTime, g_dwSucceedStartTime;

// Note: 사운드 관련
FMOD_SYSTEM *g_pSystem;
FMOD_SOUND *g_pEffectSound[5], *g_pLoopSound[4];
FMOD_CHANNEL *g_pLoopChannel[4];
char *g_EffectSoundList[] = { "are_you_ready.wav", "goal1.wav", "shoot.wav", "success2.wav", "ubs.wav"};
char *g_LoopSoundList[] = { "main_crowd.wav", "run.wav", "fail.wav", "restart.wav" };

// Note: 사운드 초기화 함수
//       사운드는 www.freesound.org에서 다운 받을 수 있습니다. 
//       org 이므로 비영리 단체이므로 회원가입 후에 다양한 사운드 사용 가능합니다. ^^ !
void SoundInit()
{
	int i;

	// Note: FMOD 사운드 객체 생성과 초기화
	FMOD_System_Create(&g_pSystem);
	FMOD_System_Init( g_pSystem, 32, FMOD_INIT_NORMAL, NULL );

	// Note: 이펙트 사운드 로딩
	for( i = 0 ; i < 5 ; i++ )
		FMOD_System_CreateSound( g_pSystem, g_EffectSoundList[i], FMOD_DEFAULT, 0, &g_pEffectSound[i] );

	// Note: BGM 사운드 로딩
	for( i = 0 ; i < 4 ; i++ )
		FMOD_System_CreateSound( g_pSystem, g_LoopSoundList[i], FMOD_LOOP_NORMAL, 0, &g_pLoopSound[i] );
}

void SoundRelease()
{
	int i;

	for( i = 0 ; i < 5 ; i++ )
		FMOD_Sound_Release( g_pEffectSound[i] );
	for( i = 0 ; i < 4 ; i++ )
		FMOD_Sound_Release( g_pLoopSound[i] );

	FMOD_System_Close( g_pSystem );
	FMOD_System_Release( g_pSystem );
}

void LoadCutInfo(char *strFileName, CUT_INFO *pCutInfo)
{
	int i;
	FILE *fp;
	fp = fopen(strFileName, "r");						
	fscanf( fp, "%s\n", pCutInfo->strFileName );
	fscanf( fp, "%d\n", &pCutInfo->dwTransparentColor );
	fscanf( fp, "%d\n", &pCutInfo->nCount );						 
	pCutInfo->pCutRect = (RECT*)malloc( sizeof( RECT ) * pCutInfo->nCount );

	for( i = 0 ; i < pCutInfo->nCount ; i++ )
	{
		fscanf( fp, "%d %d %d %d\n", &pCutInfo->pCutRect[i].left, &pCutInfo->pCutRect[i].top, 
					&pCutInfo->pCutRect[i].right, &pCutInfo->pCutRect[i].bottom );
	}

	fclose( fp );
}

void ReleaseSprite(SPRITE *pSprite)
{
	int i;

	free(pSprite->pCutRect);
	for(i = 0; i < pSprite->nCount; i++)
		free(pSprite->pPosInfo[i].pCollisionRect);
	free(pSprite->pPosInfo);
	free(pSprite->pAniTime);
	free(pSprite);	
}

void ReleaseGenSprite(GEN_SPRITE *pSprite)
{
	int i;

	free(pSprite->pCutRect);
	for(i = 0; i < pSprite->nCount; i++)
		free(pSprite->pPosInfo[i].pCollisionRect);
	free(pSprite->pPosInfo);
	free(pSprite->pAniTime);
	free(pSprite);	
}

/*----------------------------------------------------------------------------------------
   일반적인 스프라이트 로딩 함수이다. .spr만 로딩한다.
-----------------------------------------------------------------------------------------*/
GEN_SPRITE* LoadGeneralSprite(char *strSprite)
{
	FILE *fp;
	int i, j;
	RECT rect;

	GEN_SPRITE *pGenSprite = (GEN_SPRITE*)malloc(sizeof(GEN_SPRITE));
	fp = fopen( strSprite, "rt" );

	// cut 데이터 로딩
	fscanf(fp, "%s\n", pGenSprite->strFileName ); 
	fscanf(fp, "%d\n", &pGenSprite->dwTransparentColor );
	fscanf(fp, "%d\n", &pGenSprite->nCount );
	pGenSprite->pCutRect = (RECT*)malloc(sizeof(RECT)*pGenSprite->nCount);
	for( i = 0 ; i < pGenSprite->nCount  ; i++ )
		fscanf( fp, "%d %d %d %d\n", &pGenSprite->pCutRect[i].left, 
					                 &pGenSprite->pCutRect[i].top, 
									 &pGenSprite->pCutRect[i].right, 
									 &pGenSprite->pCutRect[i].bottom );

	// pos 데이터 로딩
	pGenSprite->pPosInfo = (POS_INFO*)malloc( sizeof(POS_INFO)*pGenSprite->nCount );
	for( i = 0 ; i < pGenSprite->nCount ; i++ )
	{
		fscanf( fp, "중심좌표: %d %d\n", &pGenSprite->pPosInfo[i].nCenterX, 
						                    &pGenSprite->pPosInfo[i].nCenterY );
		fscanf( fp, "충돌개수: %d\n", &pGenSprite->pPosInfo[i].nCollisionCount);

		pGenSprite->pPosInfo[i].pCollisionRect = (RECT*)malloc(sizeof(RECT)*pGenSprite->pPosInfo[i].nCollisionCount);
		for( j = 0 ; j < pGenSprite->pPosInfo[i].nCollisionCount ; j++ )
		{			                                                                                                                      					
			fscanf( fp, "충돌좌표: %d %d %d %d\n", &rect.left, &rect.top, 
							                       &rect.right, &rect.bottom );
			pGenSprite->pPosInfo[i].pCollisionRect[j] = rect;
		}
	}

	// ani 데이터 로딩
	pGenSprite->pAniTime = (DWORD*)malloc(sizeof(DWORD)*pGenSprite->nCount);
	for( i = 0 ; i < pGenSprite->nCount ; i++ )
	{
		fscanf( fp, "%d\n", &pGenSprite->pAniTime[i] );
	}
	
	fclose(fp);

	return pGenSprite;
}

NO_ANI_SPRITE* LoadNoAniSprite(char *strSprite)
{
	FILE *fp;
	int i, j;
	RECT rect;

	NO_ANI_SPRITE *pSprite = (NO_ANI_SPRITE*)malloc(sizeof(NO_ANI_SPRITE));
	fp = fopen( strSprite, "rt" );

	// cut 데이터 로딩
	fscanf(fp, "%s\n", pSprite->strFileName ); 
	fscanf(fp, "%d\n", &pSprite->dwTransparentColor );
	fscanf(fp, "%d\n", &pSprite->nCount );
	pSprite->pCutRect = (RECT*)malloc(sizeof(RECT)*pSprite->nCount);
	for( i = 0 ; i < pSprite->nCount  ; i++ )
		fscanf( fp, "%d %d %d %d\n", &pSprite->pCutRect[i].left, 
					                 &pSprite->pCutRect[i].top, 
									 &pSprite->pCutRect[i].right, 
									 &pSprite->pCutRect[i].bottom );

	// pos 데이터 로딩
	pSprite->pPosInfo = (POS_INFO*)malloc( sizeof(POS_INFO)*pSprite->nCount );
	for( i = 0 ; i < pSprite->nCount ; i++ )
	{
		fscanf( fp, "중심좌표: %d %d\n", &pSprite->pPosInfo[i].nCenterX, 
						                    &pSprite->pPosInfo[i].nCenterY );
		fscanf( fp, "충돌개수: %d\n", &pSprite->pPosInfo[i].nCollisionCount);

		pSprite->pPosInfo[i].pCollisionRect = (RECT*)malloc(sizeof(RECT)*pSprite->pPosInfo[i].nCollisionCount);
		for( j = 0 ; j < pSprite->pPosInfo[i].nCollisionCount ; j++ )
		{			                                                                                                                      					
			fscanf( fp, "충돌좌표: %d %d %d %d\n", &rect.left, &rect.top, 
							                       &rect.right, &rect.bottom );
			pSprite->pPosInfo[i].pCollisionRect[j] = rect;
		}
	}
	
	fclose(fp);

	return pSprite;
}

// spr 파일과 dir 파일을 읽어 하나의 스프라이트 데이터를 만든다.
// 스테이지 디자인 툴에서 만들었던 함수이다.
SPRITE* LoadSprite(char *strSprite, char *strMoveRange)
{
	FILE *fp;
	int i, j;
	RECT rect;

	SPRITE *pSprite = (SPRITE*)malloc(sizeof(SPRITE));
	fp = fopen( strSprite, "rt" );

	// cut 데이터 로딩
	fscanf(fp, "%s\n", pSprite->strFileName ); 
	fscanf(fp, "%d\n", &pSprite->dwTransparentColor );
	fscanf(fp, "%d\n", &pSprite->nCount );
	pSprite->pCutRect = (RECT*)malloc(sizeof(RECT)*pSprite->nCount);
	for( i = 0 ; i < pSprite->nCount  ; i++ )
		fscanf( fp, "%d %d %d %d\n", &pSprite->pCutRect[i].left, 
					                 &pSprite->pCutRect[i].top, 
									 &pSprite->pCutRect[i].right, 
									 &pSprite->pCutRect[i].bottom );

	// pos 데이터 로딩
	pSprite->pPosInfo = (POS_INFO*)malloc( sizeof(POS_INFO)*pSprite->nCount );
	for( i = 0 ; i < pSprite->nCount ; i++ )
	{
		fscanf( fp, "중심좌표: %d %d\n", &pSprite->pPosInfo[i].nCenterX, 
						                    &pSprite->pPosInfo[i].nCenterY );
		fscanf( fp, "충돌개수: %d\n", &pSprite->pPosInfo[i].nCollisionCount);

		pSprite->pPosInfo[i].pCollisionRect = (RECT*)malloc(sizeof(RECT)*pSprite->pPosInfo[i].nCollisionCount);
		for( j = 0 ; j < pSprite->pPosInfo[i].nCollisionCount ; j++ )
		{			                                                                                                                      					
			fscanf( fp, "충돌좌표: %d %d %d %d\n", &rect.left, &rect.top, 
							                       &rect.right, &rect.bottom );
			pSprite->pPosInfo[i].pCollisionRect[j] = rect;
		}
	}

	// ani 데이터 로딩
	pSprite->pAniTime = (DWORD*)malloc(sizeof(DWORD)*pSprite->nCount);
	for( i = 0 ; i < pSprite->nCount ; i++ )
	{
		fscanf( fp, "%d\n", &pSprite->pAniTime[i] );
	}
	
	fclose(fp);

	// 이동범위 데이터 로딩
	fp = fopen(strMoveRange, "rt");
	for(i = 0; i < 5 ; i++)
	{	
		fscanf(fp, "%d %d\n", &pSprite->nMoveRange[i][0], &pSprite->nMoveRange[i][1]);							
	}
	fclose(fp);

	return pSprite;
}

void ResetEnemy()
{
	int nTypeIndex, nPatIndex, i;
	DIRECTION nDirection;

	for(i = 0 ; i < g_nEnemyCount ; i++)
	{					
		nTypeIndex = g_sStageInfo.nEnemyTypePatIndex[i][0];					
		nPatIndex = g_sStageInfo.nEnemyTypePatIndex[i][1];					

		g_pEnemyProperty[i].nTypeIndex = nTypeIndex;
		g_pEnemyProperty[i].nPatternIndex = nPatIndex;
		g_pEnemyProperty[i].nX = g_pPattern[nPatIndex].nX;
		g_pEnemyProperty[i].nY = g_pPattern[nPatIndex].nY;					
		nDirection = g_pPattern[nPatIndex].pMove[0].nDirection; // 패턴에 정의된 최초 방향
		g_pEnemyProperty[i].nAniSprIndex = g_pSprite[nTypeIndex]->nMoveRange[nDirection][0];		
		g_pEnemyProperty[i].dwOldAniTime = GetTickCount();	
		g_pEnemyProperty[i].nMoveIndex = 0;
		g_pEnemyProperty[i].nAddDistance = 0;
		g_pEnemyProperty[i].dwStopElapsedTime = 0;	
		g_pEnemyProperty[i].dwOldMoveTime = GetTickCount();
	}
}

void LoadStage( char *strStageFileName )
{
	FILE *fp;
	int i, j, nTypeIndex, nPatIndex;
	DIRECTION nDirection;

	fp = fopen(strStageFileName, "rt" );
	fscanf(fp, "limited time = %d\n", &g_sStageInfo.dwLimitedTime);				
	fscanf(fp, "goal count = %d\n",  &g_sStageInfo.nGoalCount);
	fscanf(fp, "item time = %d\n", &g_sStageInfo.dwItemTime);
				
	// 패턴 파일명 읽기				
	fscanf(fp, "pattern count = %d\n", &g_sStageInfo.nPatternCount);
	if( g_sStageInfo.strPatternFileName != NULL )
		free(g_sStageInfo.strPatternFileName);
	g_sStageInfo.strPatternFileName = (char(*)[20])malloc(sizeof(char[20])*g_sStageInfo.nPatternCount);
	for(i = 0 ; i < g_sStageInfo.nPatternCount ; i++ )
	{					
		fscanf(fp, "pattern file name = %s\n", g_sStageInfo.strPatternFileName[i]);
	}
	// 선수 타입 인덱스와 패턴 인덱스 읽기				
	fscanf(fp, "enemy count = %d\n", &g_sStageInfo.nEnemyCount);
	
	if( g_sStageInfo.nEnemyTypePatIndex != NULL )
		free(g_sStageInfo.nEnemyTypePatIndex);
	g_sStageInfo.nEnemyTypePatIndex = (int(*)[2])malloc(sizeof(int[2])*g_sStageInfo.nEnemyCount);
	for(i = 0 ; i < g_sStageInfo.nEnemyCount ; i++)
	{					
		fscanf(fp, "enemy index = %d pattern index = %d\n", &g_sStageInfo.nEnemyTypePatIndex[i][0], 
					&g_sStageInfo.nEnemyTypePatIndex[i][1]);
	}
	fclose(fp);

	//-------------------- 컨트롤과 각종 변수에 설정하는 부분 ------------------------------------//
	// 1) 패턴 데이터 읽기 -> 패턴 메모리에 생성 -> 리스트 컨트롤과 컨트롤에 설정
	if(g_pPattern != NULL) // 이전 패턴의 메모리 해제 부분
	{
		for(i = 0 ; i < g_nCurPatternCount ; i++ ) // 이전 패턴의 개수가 있으므로
		{
			if( g_pPattern[i].pMove != NULL )
				free(g_pPattern[i].pMove);					
		}

		free(g_pPattern);
	}	

	// 패턴 로딩 부분				
	g_nCurPatternCount = g_sStageInfo.nPatternCount;
	g_pPattern = (PATTERN_PROPERTY*)malloc(sizeof(PATTERN_PROPERTY) * g_nCurPatternCount);
	for(i = 0 ; i < g_nCurPatternCount ; i++)
	{								
		fp = fopen(g_sStageInfo.strPatternFileName[i], "rt");
		fscanf(fp, "pat count = %d\n", &g_pPattern[i].nCount);
		fscanf(fp, "init x = %d y = %d\n", &g_pPattern[i].nX, &g_pPattern[i].nY);
		g_pPattern[i].nX += g_nAdjustX; // 게임 화면에 따라 보정한다.
		g_pPattern[i].nY += g_nAdjustY;
		g_pPattern[i].pMove = (MOVE_PROPERTY*)malloc(g_pPattern[i].nCount*sizeof(MOVE_PROPERTY));				
		for(j = 0 ; j < g_pPattern[i].nCount ; j++)
		{
			fscanf(fp, "dir=%d vel=%f Dist=%d time=%d x=%d y=%d\n", 
								&g_pPattern[i].pMove[j].nDirection, 
								&g_pPattern[i].pMove[j].fVelocity,	
								&g_pPattern[i].pMove[j].nAllDistance, 
								&g_pPattern[i].pMove[j].dwDelayTime, 
								&g_pPattern[i].pMove[j].nDestX,
								&g_pPattern[i].pMove[j].nDestY);
			g_pPattern[i].pMove[j].nDestX += g_nAdjustX; // 게임 화면으로 보정하여 준다.
			g_pPattern[i].pMove[j].nDestY += g_nAdjustY;
		}
		fclose(fp);
	}			
				
	// 적 캐릭터 생성
	if(g_pEnemyProperty != NULL)
		free(g_pEnemyProperty);				
	g_pEnemyProperty = (ENEMY_PROPERTY*)malloc(sizeof(ENEMY_PROPERTY)*g_sStageInfo.nEnemyCount);
	g_nEnemyCount = g_sStageInfo.nEnemyCount;

	for(i = 0 ; i < g_nEnemyCount ; i++)
	{					
		nTypeIndex = g_sStageInfo.nEnemyTypePatIndex[i][0];					
		nPatIndex = g_sStageInfo.nEnemyTypePatIndex[i][1];					

		g_pEnemyProperty[i].nTypeIndex = nTypeIndex;
		g_pEnemyProperty[i].nPatternIndex = nPatIndex;
		g_pEnemyProperty[i].nX = g_pPattern[nPatIndex].nX;
		g_pEnemyProperty[i].nY = g_pPattern[nPatIndex].nY;					
		nDirection = g_pPattern[nPatIndex].pMove[0].nDirection; // 패턴에 정의된 최초 방향
		g_pEnemyProperty[i].nAniSprIndex = g_pSprite[nTypeIndex]->nMoveRange[nDirection][0];		
		g_pEnemyProperty[i].dwOldAniTime = GetTickCount();	
		g_pEnemyProperty[i].nMoveIndex = 0;
		g_pEnemyProperty[i].nAddDistance = 0;
		g_pEnemyProperty[i].dwStopElapsedTime = 0;	
		g_pEnemyProperty[i].dwOldMoveTime = GetTickCount();
	}
}

// INIT, LOADING, READY, RUN, STOP, SUCCESS, FAILED, RESULT, EXIT
void Init( IDirect3DDevice9* pd3dDevice )
{
	int i;	
	FMOD_CHANNEL *pChannel;	
	wchar_t wString[100];

	switch( g_nGameState )
	{
	case GAME_INIT:
		if( g_nStage == -1 ) // 최초 초기화 부분
		{
	// 메인 화면 로딩
	D3DXCreateTextureFromFileEx( pd3dDevice, L"main.bmp", 0, 0, 0, 0, D3DFMT_UNKNOWN, 
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0), 
		0, 0, &g_pMainTexture);
			
	// 메인에 사용될 축구공 로딩 
	LoadCutInfo("main_ball.cut", &g_sMainBallCutInfo);
	swprintf(wString, L"%S",  g_sMainBallCutInfo.strFileName );
	D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				D3DCOLOR_XRGB( GetRValue(g_sMainBallCutInfo.dwTransparentColor), 
								GetGValue( g_sMainBallCutInfo.dwTransparentColor), 
								GetBValue( g_sMainBallCutInfo.dwTransparentColor ) ),
								0, 0, &g_pMainBallTexture);			

			// 게임 전체에 사용하게 될 버튼 이미지 로딩
			LoadCutInfo("button.cut", &g_sButtonCutInfo);
			swprintf(wString, L"%S",  g_sButtonCutInfo.strFileName );
			D3DXCreateTextureFromFileEx( 
				pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				D3DCOLOR_XRGB( GetRValue( g_sButtonCutInfo.dwTransparentColor), 
							   GetGValue( g_sButtonCutInfo.dwTransparentColor), 
							   GetBValue( g_sButtonCutInfo.dwTransparentColor ) ), 
								0, 0, &g_pButtonTexture);					

			g_sGameButton[0].nX = 580;
			g_sGameButton[0].nY = 350;
			g_sGameButton[0].nIndex[0] = 0;
			g_sGameButton[0].nIndex[1] = 1;
			g_sGameButton[0].bIsPush = FALSE;

			g_sGameButton[1].nX = 580;
			g_sGameButton[1].nY = 410;
			g_sGameButton[1].nIndex[0] = 2;
			g_sGameButton[1].nIndex[1] = 3;
			g_sGameButton[1].bIsPush = FALSE;	

			g_sGameButton[2].nX = 318;
			g_sGameButton[2].nY = 430;
			g_sGameButton[2].nIndex[0] = 4;
			g_sGameButton[2].nIndex[1] = 5;
			g_sGameButton[2].bIsPush = FALSE;

			g_sGameButton[3].nX = 220;
			g_sGameButton[3].nY = 350;
			g_sGameButton[3].nIndex[0] = 6;
			g_sGameButton[3].nIndex[1] = 7;
			g_sGameButton[3].bIsPush = FALSE;

			g_sGameButton[4].nX = 410;
			g_sGameButton[4].nY = 350;
			g_sGameButton[4].nIndex[0] = 8;
			g_sGameButton[4].nIndex[1] = 9;
			g_sGameButton[4].bIsPush = FALSE;

			// 배경 로딩
			LoadCutInfo("background.cut", &g_sBackGroundCutInfo);
			swprintf(wString, L"%S",  g_sBackGroundCutInfo.strFileName );
			D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, g_sBackGroundCutInfo.dwTransparentColor, 
				0, 0, &g_pBackGroundTexture);		

			// 글자 로딩  
			LoadCutInfo("text.cut", &g_sTextCutInfo);
			swprintf(wString, L"%S",  g_sTextCutInfo.strFileName );
			D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, g_sTextCutInfo.dwTransparentColor, 
				0, 0, &g_pTextTexture);					

			/*------------------------- 주인공 초기화 ----------------------------------------*/	
			g_pPlayerSprite = LoadSprite("player.spr", "player.dir");
			swprintf(wString, L"%S",  g_pPlayerSprite->strFileName );
			D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				D3DCOLOR_XRGB( GetRValue(g_pPlayerSprite->dwTransparentColor), 
							   GetGValue( g_pPlayerSprite->dwTransparentColor), 
							   GetBValue( g_pPlayerSprite->dwTransparentColor ) ),
							   0, 0, &g_pPlayerTexture);				
			
			/*---------------------------------  공 초기화 --------------------------------------*/
			g_pBallSprite = LoadGeneralSprite("ball.spr");
			swprintf(wString, L"%S",  g_pBallSprite->strFileName );
			D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				D3DCOLOR_XRGB( GetRValue( g_pBallSprite->dwTransparentColor ), 
							   GetGValue( g_pBallSprite->dwTransparentColor ), 
							   GetBValue( g_pBallSprite->dwTransparentColor ) ),  
							   0, 0, &g_pBallTexture);	

			/*---------------------------------  골대 초기화 ----------------------------------*/
			g_pGoalPostSprite = LoadNoAniSprite("goalpost.spr");
			swprintf(wString, L"%S",  g_pGoalPostSprite->strFileName );
			D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
				D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				D3DCOLOR_XRGB( GetRValue( g_pGoalPostSprite->dwTransparentColor ), 
							   GetGValue( g_pGoalPostSprite->dwTransparentColor ), 
							   GetBValue( g_pGoalPostSprite->dwTransparentColor ) ),							    
								0, 0, &g_pGoalPostTexture);	
			
			/*-------------------------------  패턴과 적캐릭터 초기화 -------------------------*/
			// 적 캐릭터 스프라이트 로딩 => 4개로 고정
			for(i = 0 ; i < 4 ; i++)
			{
				g_pSprite[i] = LoadSprite( g_strSpriteFile[i], g_strDirectionFile[i] );
				swprintf( wString, L"%S",  g_pSprite[i]->strFileName );
				D3DXCreateTextureFromFileEx( pd3dDevice, wString, 0, 0, 0, 0, D3DFMT_UNKNOWN, 
							D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
							D3DCOLOR_XRGB(  GetRValue( g_pSprite[i]->dwTransparentColor ), 
											GetGValue( g_pSprite[i]->dwTransparentColor ), 
											GetBValue( g_pSprite[i]->dwTransparentColor ) ),							   
											0, 0, &g_pEnemyTexture[i]);	
			}	
			
			g_EffectText.dwEffectTime = 2000; // Note: 이펙트 초기화			
			g_dwSucceedTime = 2100;  // Note: Succeed 출력 시간 
			SoundInit();

			FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pLoopSound[0], 0, &g_pLoopChannel[0] ); // 메인 BGM 사운드
		}else{
			/*--------------------------------- 배경 화면에 대한 초기 위치 설정 -------------------*/
			// 게임 배경 크기 부분 
			g_nBGIndex = g_nBackGroundIndex[g_nStage % 4]; // 배경이 4개 이다.
			g_nBGWidth = g_sBackGroundCutInfo.pCutRect[g_nBGIndex].right - 
				           g_sBackGroundCutInfo.pCutRect[g_nBGIndex].left + 1;
			g_nBGHeight = g_sBackGroundCutInfo.pCutRect[g_nBGIndex].bottom - 
				           g_sBackGroundCutInfo.pCutRect[g_nBGIndex].top + 1;
			g_nAdjustX = (GAME_WIDTH - g_nBGWidth)/2 - 1;
			g_nAdjustY = (GAME_HEIGHT - g_nBGHeight)/2 - 1;
			g_nDeadLineY = g_nAdjustY + 30; // 배경에서 군중이 있는 Y 위치 부분, 공 충돌 좌표이다.
			
			// 스테이지 초기화
			LoadStage( g_strStageFileName[g_nStage] );							
		}
		break;
	case GAME_LOADING:
		FMOD_Channel_Stop( g_pLoopChannel[0] );
		ShowCursor(FALSE);
		g_nLoadingBarCount = 0;
		break;
	case GAME_READY:
		// 주인공 초기화 
		g_sPlayer.nX = GAME_WIDTH/2;
		g_sPlayer.nY = GAME_HEIGHT - 100;
		g_sPlayer.nDirection = STOP; // IDLE이다.
		g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][0];
		g_sPlayer.dwOldAniTime = 0;
		g_sPlayer.nAction = PLAYER_IDLE;
		g_sPlayer.dwMoveTime = 50;
		g_sPlayer.nDistance = 2;

		// 공 초기화 
		g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
		g_sBall.nY = g_sPlayer.nY + 5;
		g_sBall.dwAniTime = 100;
		g_sBall.dwMoveTime = 20;
		g_sBall.dwOldMoveTime = 0;
		g_sBall.nAniSprIndex = 0;
		g_sBall.dwOldAniTime = GetTickCount();
		g_sBall.nBallState = BALL_STOP;
		g_sBall.nDistance = 4;		

		g_dwItemTime = g_sStageInfo.dwItemTime;

		g_dwLimitedTime = g_sStageInfo.dwLimitedTime;
		g_nGoalCount = g_sStageInfo.nGoalCount;
		g_nGrade = 0;

		FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[0], 0, &pChannel ); // Ready 이펙트 사운드	
		break;	
	}			
}

// 주인공의 상태에 따라 공을 초기화 하는 함수이다.
void BallInit( DWORD dwCurTime)
{
	// 충돌 한 경우, 공을 주인공의 방향에 따라 초기화 한다. MOVE, READY, IDLE					
	switch( g_sPlayer.nAction )
	{
	case PLAYER_MOVE:
		g_sBall.nBallState = BALL_MOVE;
		g_sBall.dwOldMoveTime = g_sBall.dwOldAniTime = dwCurTime;	
		g_sBall.nAniSprIndex = 0;

		switch( g_sPlayer.nDirection ) // 볼에 대한 초기화 및 업그레이드
		{
		case RIGHT:
				g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;	
				g_sBall.nY = g_sPlayer.nY + 5;
				break;
		case LEFT:
				g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;	
				g_sBall.nY = g_sPlayer.nY + 5;
				break;
		}
		break;
	case PLAYER_READY: // READY일 떄는 이전 방향의 첫 프레임이 셋업되어 있으므로
		g_sBall.nBallState = BALL_READY;
		switch( g_sPlayer.nDirection )
		{
		case RIGHT:
				g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;
				g_sBall.nY = g_sPlayer.nY + 5;
				break;
		case LEFT:
				g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
				g_sBall.nY = g_sPlayer.nY + 5;
				break;
		}						
		break;					
	case PLAYER_IDLE:
		g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
		g_sBall.nY = g_sPlayer.nY + 5;
		g_sBall.nBallState = BALL_STOP;					
		break;
	}	
}

void ObjectCollision( DWORD dwCurTime )
{
	int i, j, m, nX, nY, nTypeIndex, nAniSprIndex;
	RECT rect1, rect2, rtTemp;
	POINT ptCenter;
	FMOD_CHANNEL *pChannel;

	// 볼이 슛 상태일 때만 충돌 검사를 한다.
	if( g_sBall.nBallState != BALL_SHOOT )
		return ;	
	
	// 공의 크기가 같으므로 충돌 좌표 하나만 구해 적용한다.		
	rect1.left = g_sBall.nX - g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].nCenterX +
					g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].left;						
	rect1.top = g_sBall.nY - g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].nCenterY +
					g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].top;
	rect1.right = rect1.left + g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].right -
					g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].left + 1;
	rect1.bottom = rect1.top + g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].bottom -
					g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].pCollisionRect[0].top + 1;

	// 공과 상대방 선수와의 충돌(스프라이트의 출력좌표를 기준으로)
	for( i = 0 ; i < g_sStageInfo.nEnemyCount ; i++ )
	{
		nTypeIndex = g_pEnemyProperty[i].nTypeIndex;
		nAniSprIndex = g_pEnemyProperty[i].nAniSprIndex;

		ptCenter.x = g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].nCenterX;
		ptCenter.y = g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].nCenterY;

		// 스프라이트 출력기준 좌표와 그 길이를 기준으로 하여 충돌영역을 구해서 적용한다.
		rect2.left = g_pEnemyProperty[i].nX - ptCenter.x;				
		rect2.top = g_pEnemyProperty[i].nY - ptCenter.y;	
		rect2.right = rect2.left +  g_pSprite[ nTypeIndex ]->pCutRect[nAniSprIndex].right -
						g_pSprite[ nTypeIndex ]->pCutRect[nAniSprIndex].left + 1;
		rect2.bottom = rect2.top + g_pSprite[ nTypeIndex ]->pCutRect[nAniSprIndex].bottom -
						g_pSprite[ nTypeIndex ]->pCutRect[ nAniSprIndex ].top + 1;		

		// 1 단계 충돌 체크
		if( IntersectRect(&rtTemp, &rect1, &rect2 ) != 0 ) 
		{ 			
			// 2 단계 충돌 체크 
			for( m = 0 ; m < g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].nCollisionCount ; m++ )
			{			
				rect2.left = g_pEnemyProperty[i].nX - ptCenter.x + 
								g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].left;				
				rect2.top = g_pEnemyProperty[i].nY - ptCenter.y +
								g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].top;	

				rect2.right = rect2.left + g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].right -
								g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].left + 1;				
				rect2.bottom = rect2.top + g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].bottom -
								g_pSprite[ nTypeIndex ]->pPosInfo[ nAniSprIndex ].pCollisionRect[m].top + 1;		
								
				if( IntersectRect(&rtTemp, &rect1, &rect2 ) != 0 ) // 2차 충돌이 된 경우
				{ 
					FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[4], 0, &pChannel ); // 웁스 이펙트 사운드
					BallInit( dwCurTime);	// 주인공의 상태에 따라 공 초기화
					return; // 공은 하나이므로 충돌하면 이 함수를 종료한다.
				}						
			}
		}		
	}

	// 공과 골대의 충돌, 골대에서 골인 영역은 충돌 [2]이다. 
	rect2.left = g_ptGoalPost.x + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].left;
	rect2.top = g_ptGoalPost.y + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].top;
	rect2.right = rect2.left + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].right - 
								g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].left + 1;
	rect2.bottom = rect2.top + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].bottom - 
								g_pGoalPostSprite->pPosInfo[0].pCollisionRect[2].top + 1;

	if( rect1.left >= rect2.left && rect1.right <= rect2.right
		&& rect1.top >= rect2.top && rect1.bottom <= rect2.bottom ) // 공이 골대 안으로 완전히 들어 왔을 때 골인으로 처리
	{
		// 골인 
		g_nGrade += 10; // 10점씩 가산한다.
		g_nGoalCount--;

		FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[1], 0, &pChannel ); // 골인 이펙트 사운드

		if( g_nGoalCount == 0 )
		{
			g_nGameState = GAME_STOP;
			return ;
		}

		g_EffectText.dwStartEffectTime = dwCurTime; // 효과 출력
		g_EffectText.bLife = TRUE;		
		BallInit( dwCurTime);	// 주인공의 상태에 따라 공 초기화	
		return; // 공은 하나이므로 충돌하면 이 함수를 종료한다.
	}else{  // 노골 영역은 [0]과 [1]의 경우이다. 
		for( i = 0 ; i < 2 ; i++) // 노골 판정 체크
		{
			rect2.left = g_ptGoalPost.x + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].left;
			rect2.top = g_ptGoalPost.y + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].top;
			rect2.right = rect2.left + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].right - 
										g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].left + 1;
			rect2.bottom = rect2.top + g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].bottom - 
										g_pGoalPostSprite->pPosInfo[0].pCollisionRect[i].top + 1;
			if( IntersectRect(&rtTemp, &rect1, &rect2 ) != 0 ) // 노골 판정
			{				
				BallInit( dwCurTime);	// 주인공의 상태에 따라 공 초기화
				return; // 공은 하나이므로 충돌하면 이 함수를 종료한다.
			}				
		}			
	}

	// 3차 충돌 부분으로 공이 관중석과 충돌한 경우이다.
	if( rect1.top <= g_nDeadLineY )
	{
		BallInit(dwCurTime);
		return ;
	}	
}
	

void Update( IDirect3DDevice9* pd3dDevice )
{
	DWORD dwAniTime;
	static DWORD dwOldTime = GetTickCount(), dwAddTime;
	static DWORD dwOldBallTime;
	static DWORD dwOldKeyDownTime;
	int nDistance, nButtonIndex;
	int i;
	RECT rtButton;
	int nTypeIndex, nPatIndex, nMoveIndex;
	DWORD dwDelayTime;
	DIRECTION nDirection;	
	FMOD_CHANNEL *pChannel;

	// 한프레임 경과시간을 측정하는 코드 
	DWORD dwCurTime = GetTickCount();
	DWORD dwElapsedTime = dwCurTime - dwOldTime; 	
	dwOldTime = dwCurTime; // <-- 요게 중요함

	switch( g_nGameState )
	{
	case GAME_INIT:
		if( g_nStage == -1 )
		{		
			if( dwCurTime - dwOldBallTime > 200 )
			{
				dwOldBallTime = dwCurTime;
				g_nMainBallAniIndex++;
				g_nMainBallAniIndex = g_nMainBallAniIndex % g_sMainBallCutInfo.nCount;
			}

			for( i = 0 ; i < 2 ; i++ )
			{
				nButtonIndex = g_sGameButton[i].nIndex[g_sGameButton[i].bIsPush];			
				rtButton.left = g_sGameButton[i].nX;
				rtButton.top = g_sGameButton[i].nY;
				rtButton.right = rtButton.left + g_sButtonCutInfo.pCutRect[nButtonIndex].right - 
								g_sButtonCutInfo.pCutRect[nButtonIndex].left + 1;
				rtButton.bottom = rtButton.top + g_sButtonCutInfo.pCutRect[nButtonIndex].bottom - 
								g_sButtonCutInfo.pCutRect[nButtonIndex].top + 1;

				if( PtInRect( &rtButton, g_ptMouse ) )
					g_sGameButton[i].bIsPush = TRUE;
				else
					g_sGameButton[i].bIsPush = FALSE;
			}
		}
		break;
	case GAME_LOADING:
		if( dwCurTime - g_dwOldLoadingBarTime >= g_dwLoadingBarTime )
		{
			g_dwOldLoadingBarTime = dwCurTime;
			g_nLoadingBarCount++;
			if( g_nLoadingBarCount == 27 )
			{
				g_nGameState = GAME_READY;
				Init( DXUTGetD3D9Device() );
				g_dwOldReadyTime = dwCurTime;
			}
		}
		break;
	case GAME_READY:
		if( dwCurTime - g_dwOldReadyTime >= g_dwReadyTime )
		{
			g_nGameState = GAME_RUN;
			FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pLoopSound[1], 0, &g_pLoopChannel[1] );
		}
		break;

	case GAME_RUN:			
		// 게임을 중단하는 요건이 가장 우선이 되므로 서두에 코딩
		g_dwLimitedTime -= dwElapsedTime;
		if( g_dwLimitedTime <= 1000 )
		{
			g_nGameState = GAME_STOP;
			return ;
		}

	//---------------------------------- 주인공 데이터 업데이트 -------------------------------------------------
		switch( g_sPlayer.nAction )
		{
		case PLAYER_MOVE:
			
			if( DXUTIsKeyDown( VK_RIGHT ) )
			{			
				dwOldKeyDownTime = dwCurTime;
				if( g_sPlayer.nDirection == RIGHT )
				{
					// Note: 이동 부분
					if( dwCurTime - g_sPlayer.dwOldMoveTIme >= g_sPlayer.dwMoveTime )
					{
						g_sPlayer.dwOldMoveTIme = dwCurTime;
						g_sPlayer.nX += g_sPlayer.nDistance;						
					}

					// Note: 애니메이션 부분
					if( dwCurTime - g_sPlayer.dwOldAniTime >=  g_pPlayerSprite->pAniTime[g_sPlayer.nAniSprIndex] )
					{
						g_sPlayer.dwOldAniTime = dwCurTime;
						g_sPlayer.nAniSprIndex++;
						if( g_sPlayer.nAniSprIndex > g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][1] )
							g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][0];
					}

				}else{ 
					// Note: 방향 전환이 되어 주인공을 초기화 됨
					g_sPlayer.nDirection = RIGHT;
					g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[RIGHT][0];
					g_sPlayer.dwOldAniTime = dwCurTime;							
				}

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			} else if( DXUTIsKeyDown(VK_LEFT) ) {

				dwOldKeyDownTime = dwCurTime;
				if( g_sPlayer.nDirection == LEFT )
				{
					// Note: 이동 부분
					if( dwCurTime - g_sPlayer.dwOldMoveTIme >= g_sPlayer.dwMoveTime )
					{
						g_sPlayer.dwOldMoveTIme = dwCurTime;
						g_sPlayer.nX -= g_sPlayer.nDistance;
					}

					if( dwCurTime - g_sPlayer.dwOldAniTime >= g_pPlayerSprite->pAniTime[g_sPlayer.nAniSprIndex] )
					{
						g_sPlayer.dwOldAniTime = dwCurTime;
						g_sPlayer.nAniSprIndex++;
						if( g_sPlayer.nAniSprIndex > g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][1] )
							g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][0];
					}
					
				} else {
					// 초기화 됨
					g_sPlayer.nDirection = LEFT;
					g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[LEFT][0];
					g_sPlayer.dwOldAniTime = dwCurTime;							
				}
				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			} else if( DXUTIsKeyDown('S') ) {  // 공이 없어도 슛 동작은 할 수 있게 한다.
				// Note: 볼 초기화
				if( g_sBall.nBallState != BALL_SHOOT )
				{
					g_sBall.nX = g_sPlayer.nX;
					g_sBall.nY = g_sPlayer.nY - 10;
					g_sBall.nBallState = BALL_SHOOT;
					g_sBall.dwOldMoveTime = g_sBall.dwOldAniTime = dwCurTime;
					g_sBall.nAniSprIndex = 0;			

					FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[2], 0, &pChannel ); // 슛 이펙트 사운드
				}

				// 주인공 슛 동작
				g_sPlayer.nDirection = TOP;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[TOP][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_SHOOT;
			} else {
				if( dwCurTime - dwOldKeyDownTime >= 50 ) // 키 눌림이 없는 것으로 판정
				{
					dwOldKeyDownTime = dwCurTime;
					g_sPlayer.nAction = PLAYER_READY; // 현재 방향의 마지막으로 셋팅
					g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][0];
					
					if( g_sBall.nBallState != BALL_SHOOT )
						g_sBall.nBallState = BALL_READY;
				}				
			}
			break;

		case PLAYER_SHOOT: // Note: 주인공의 슛 상태에서는 키 입력이 받지 않으므로 키 처리 부분이 없다.
			if( dwCurTime - g_sPlayer.dwOldAniTime > g_pPlayerSprite->pAniTime[g_sPlayer.nAniSprIndex] )
			{
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAniSprIndex++;
				// Note: 슛 동작이 끝나면 아이들 상태로 간다.
				if( g_sPlayer.nAniSprIndex > g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][1] )
				{
					g_sPlayer.nAction = PLAYER_IDLE;
					g_sPlayer.nDirection = STOP;
					g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[STOP][0];
					g_sPlayer.dwOldAniTime = dwCurTime;
				}
			}
			break;
				
		case PLAYER_READY:
		    if( DXUTIsKeyDown(VK_RIGHT) )
			{
				dwOldKeyDownTime = dwCurTime;
				// 초기화 됨
				g_sPlayer.nDirection = RIGHT;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[RIGHT][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_MOVE;

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			}else if(  DXUTIsKeyDown(VK_LEFT) ) {
				dwOldKeyDownTime = dwCurTime;
				// 초기화 됨
				g_sPlayer.nDirection = LEFT;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[LEFT][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_MOVE;

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			} else if( DXUTIsKeyDown('S') ) {  // 공이 없어도 슛 동작은 할 수 있게 한다.
				if( g_sBall.nBallState != BALL_SHOOT )
				{
					g_sBall.nX = g_sPlayer.nX;
					g_sBall.nY = g_sPlayer.nY - 10;
					g_sBall.nBallState = BALL_SHOOT;
					g_sBall.dwOldMoveTime = g_sBall.dwOldAniTime = dwCurTime;
					g_sBall.nAniSprIndex = 0;				

					FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[2], 0, &pChannel ); // 슛 이펙트 사운드
				}

				// 주인공 슛 동작
				g_sPlayer.nDirection = TOP;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[TOP][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_SHOOT;				

			}else if( dwCurTime - dwOldKeyDownTime >= 200 ) 	{
				g_sPlayer.nAction = PLAYER_IDLE;
				g_sPlayer.nDirection = STOP;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[STOP][0];
				g_sPlayer.dwOldAniTime = dwCurTime;

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_STOP;
			}
			break;

		case PLAYER_IDLE:
			if( DXUTIsKeyDown(VK_RIGHT) )
			{
				dwOldKeyDownTime = dwCurTime;
				g_sPlayer.nDirection = RIGHT;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[RIGHT][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_MOVE;

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			}else if( DXUTIsKeyDown(VK_LEFT) ){
				dwOldKeyDownTime = dwCurTime;
				g_sPlayer.nDirection = LEFT;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[LEFT][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_MOVE;

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_MOVE;

			} else if( DXUTIsKeyDown('S') ) {  // 공이 없어도 슛 동작은 할 수 있게 한다.
				if( g_sBall.nBallState != BALL_SHOOT )
				{
					g_sBall.nX = g_sPlayer.nX;
					g_sBall.nY = g_sPlayer.nY - 10;
					g_sBall.nBallState = BALL_SHOOT;
					g_sBall.dwOldMoveTime = g_sBall.dwOldAniTime = dwCurTime;
					g_sBall.nAniSprIndex = 0;	

					FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[2], 0, &pChannel ); // 슛 이펙트 사운드
				}

				// 주인공 슛 동작
				g_sPlayer.nDirection = TOP;
				g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[TOP][0];
				g_sPlayer.dwOldAniTime = dwCurTime;
				g_sPlayer.nAction = PLAYER_SHOOT;

			}else{ // 아이들 애니메이션
				if( dwCurTime - g_sPlayer.dwOldAniTime >=  g_pPlayerSprite->pAniTime[g_sPlayer.nAniSprIndex] )
				{
					g_sPlayer.dwOldAniTime = dwCurTime;
					g_sPlayer.nAniSprIndex++;
					if( g_sPlayer.nAniSprIndex > g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][1] )
						g_sPlayer.nAniSprIndex = g_pPlayerSprite->nMoveRange[g_sPlayer.nDirection][0];
				}

				if( g_sBall.nBallState != BALL_SHOOT )
					g_sBall.nBallState = BALL_STOP;
			}
			break;
		}
		
		// Note: 공에 대한 부분	
		switch( g_sBall.nBallState )
		{
		case BALL_SHOOT:
			// Note: 볼의 이동 부분
			if( dwCurTime - g_sBall.dwOldMoveTime >= g_sBall.dwMoveTime )
			{
				g_sBall.dwOldMoveTime = dwCurTime;
				g_sBall.nY -= g_sBall.nDistance;

				// 충돌 부분과 경계영역에 대한 처리가 있어야 한다. 임시적으로 
				if( g_sBall.nY <= 38 )
				{
					switch( g_sPlayer.nAction )
					{
					case PLAYER_MOVE:
						g_sBall.nBallState = BALL_MOVE;
						g_sBall.dwOldMoveTime = g_sBall.dwOldAniTime = dwCurTime;	
						g_sBall.nAniSprIndex = 0;

						switch( g_sPlayer.nDirection ) // 볼에 대한 초기화 및 업그레이드
						{
						case RIGHT:
								g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;	
								g_sBall.nY = g_sPlayer.nY + 5;
								break;
						case LEFT:
								g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;	
								g_sBall.nY = g_sPlayer.nY + 5;
								break;
						}
						break;
					case PLAYER_READY: // READY일 떄는 이전 방향의 첫 프레임이 셋업되어 있으므로
						g_sBall.nBallState = BALL_READY;
						switch( g_sPlayer.nDirection )
						{
						case RIGHT:
								g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;
								g_sBall.nY = g_sPlayer.nY + 5;
								break;
						case LEFT:
								g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
								g_sBall.nY = g_sPlayer.nY + 5;
								break;
						}						
						break;					
					case PLAYER_IDLE:
						g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
						g_sBall.nY = g_sPlayer.nY + 5;
						g_sBall.nBallState = BALL_STOP;					
						break;
					}
				}
			}

			// Note: 볼의 애니메이션 부분
			if( dwCurTime - g_sBall.dwOldAniTime >= g_sBall.dwAniTime )
			{
				g_sBall.dwOldAniTime = dwCurTime;
				g_sBall.nAniSprIndex++;
				g_sBall.nAniSprIndex = g_sBall.nAniSprIndex % g_pBallSprite->nCount;
			}		
			break;

		case BALL_READY: // Note: 에니메이션 없이 주인공 좌우에 있는 상태
			switch( g_sPlayer.nDirection )
			{
			case RIGHT:
					g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;
					break;
			case LEFT:
					g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
					break;
			}		
			break;

		case BALL_MOVE:
			switch( g_sPlayer.nDirection ) // 볼에 대한 초기화 및 업그레이드
			{
			case RIGHT:
					g_sBall.nX = g_sPlayer.nX  + BALL_X_GAP;			
					break;
			case LEFT:
					g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;			
					break;
			}

			// Note: 공의 애니메이션
			if( dwCurTime - g_sBall.dwOldAniTime > g_sBall.dwAniTime )
			{
				g_sBall.dwOldAniTime = dwCurTime;
				g_sBall.nAniSprIndex++;
				g_sBall.nAniSprIndex = g_sBall.nAniSprIndex % g_pBallSprite->nCount;
			}
			break;

		case BALL_STOP:
			g_sBall.nX = g_sPlayer.nX - BALL_X_GAP;
			g_sBall.dwOldAniTime = 0;
			g_sBall.dwOldMoveTime = 0;
			g_sBall.nAniSprIndex = 0;
			break;
		}	

		/*------------------------------------------------------------------------------------------------
			이 부분의 코드는 패턴 데이터 업데이트 부분으로 PATTERN_RENDER 부분을 복사하여 사용한다.
			코드가 같다.
		------------------------------------------------------------------------------------------------*/   
		for(i = 0 ; i < g_nEnemyCount; i++)
		{
			nTypeIndex = g_pEnemyProperty[i].nTypeIndex; // 변수명이 길어서 ^^; 고정
			nPatIndex = g_pEnemyProperty[i].nPatternIndex; // 변수명이 길어서 ^^; 고정
			nMoveIndex = g_pEnemyProperty[i].nMoveIndex;   // 변수명이 길어서 ^^; 유동적

			//----------------------------------- 패턴 부분 ---------------------------------------------------------
			nDirection = g_pPattern[nPatIndex].pMove[nMoveIndex].nDirection;

			if(nDirection == STOP)
			{
				g_pEnemyProperty[i].dwStopElapsedTime += dwElapsedTime; // 경과 시간 누적
				dwDelayTime = g_pPattern[nPatIndex].pMove[nMoveIndex].dwDelayTime;
				if(g_pEnemyProperty[i].dwStopElapsedTime >= dwDelayTime)
				{
					g_pEnemyProperty[i].nAddDistance = 0;	
					g_pEnemyProperty[i].dwStopElapsedTime = 0;						
					g_pEnemyProperty[i].nMoveIndex++;
					g_pEnemyProperty[i].nMoveIndex = g_pEnemyProperty[i].nMoveIndex % g_pPattern[nPatIndex].nCount;
					nMoveIndex = g_pEnemyProperty[i].nMoveIndex;				

					//// 에니메이션 초기화
					nDirection = g_pPattern[nPatIndex].pMove[nMoveIndex].nDirection;
					g_pEnemyProperty[i].nAniSprIndex = g_pSprite[nTypeIndex]->nMoveRange[nDirection][0];									
					g_pEnemyProperty[i].dwOldAniTime = dwCurTime;
				}
			}else{
				/*---------------------------------------------------------------------------------------------
				LEFT, RIGHT, TOP, DOWN 방향을 가진 경우만 실행되는 부분으로 이동거리로 이동방향을 결정한다.
				----------------------------------------------------------------------------------------------*/			
				nDistance = (int)(g_pPattern[nPatIndex].pMove[nMoveIndex].fVelocity * dwElapsedTime) ; // s = v*(t1-t0);		
				g_pEnemyProperty[i].nAddDistance += nDistance;	
			
				// 다음 경로로 넘어가는 거나 현재의 경로에서 이동을 하는 좌표 계산 부분이다.
				if( (int)g_pEnemyProperty[i].nAddDistance >= g_pPattern[nPatIndex].pMove[nMoveIndex].nAllDistance ) 
				{
					g_pEnemyProperty[i].nAddDistance = 0;
					g_pEnemyProperty[i].dwStopElapsedTime = 0;		
					g_pEnemyProperty[i].nX = g_pPattern[nPatIndex].pMove[nMoveIndex].nDestX;  // 결정된 좌표를 셋팅
					g_pEnemyProperty[i].nY = g_pPattern[nPatIndex].pMove[nMoveIndex].nDestY; 
				
					++g_pEnemyProperty[i].nMoveIndex; 
					g_pEnemyProperty[i].nMoveIndex = g_pEnemyProperty[i].nMoveIndex % g_pPattern[nPatIndex].nCount;		
			
					nMoveIndex = g_pEnemyProperty[i].nMoveIndex;					
								
					// 에니메이션 초기화
					nDirection = g_pPattern[nPatIndex].pMove[nMoveIndex].nDirection;
					g_pEnemyProperty[i].nAniSprIndex = g_pSprite[nTypeIndex]->nMoveRange[nDirection][0];									
					g_pEnemyProperty[i].dwOldAniTime = dwCurTime;
				}else{				
					nDirection = g_pPattern[nPatIndex].pMove[nMoveIndex].nDirection;
					switch(nDirection)
					{
					case LEFT:				
						g_pEnemyProperty[i].nX -= nDistance;														
						break;
					case RIGHT:						
						g_pEnemyProperty[i].nX += nDistance;									
						break;
					case TOP:							
						g_pEnemyProperty[i].nY -= nDistance;							
						break;
					case DOWN:					
						g_pEnemyProperty[i].nY += nDistance;							
						break;			
					}			
				}
			}

			//------------------------------------- 에니메이션 부분 -----------------------------------------------						
			dwAniTime = g_pSprite[nTypeIndex]->pAniTime[g_pEnemyProperty[i].nAniSprIndex];
			if( dwCurTime - g_pEnemyProperty[i].dwOldAniTime >= dwAniTime )
			{
				g_pEnemyProperty[i].dwOldAniTime = dwCurTime;
				g_pEnemyProperty[i].nAniSprIndex++;
				nDirection = g_pPattern[nPatIndex].pMove[nMoveIndex].nDirection;
				if( g_pEnemyProperty[i].nAniSprIndex > g_pSprite[nTypeIndex]->nMoveRange[nDirection][1] )
				{
					g_pEnemyProperty[i].nAniSprIndex = g_pSprite[nTypeIndex]->nMoveRange[nDirection][0];
				}				
			}
		}

		ObjectCollision( dwCurTime );

		if( g_EffectText.bLife )
		{
			if( dwCurTime - g_EffectText.dwStartEffectTime >= g_EffectText.dwEffectTime )
			{
				g_EffectText.bLife = FALSE;
			}
		}
		break;
	case GAME_STOP:
		FMOD_Channel_Stop( g_pLoopChannel[1] ); // 배경 BGM 사운드 정지

		if( g_nGoalCount == 0 && g_dwLimitedTime > 0 ) // 다음 스테이지로 넘어간다.
		{
			g_nGameState = GAME_SUCCEED;
			g_nTotalGrade += g_nGrade; // 전체 점수 가산
			g_dwSucceedStartTime = dwCurTime;	

			FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pEffectSound[3], 0, &pChannel ); // Success!! 이펙트 사운드
		}else{
			g_nGameState = GAME_FAILED;
			g_nGrade = 0;			
			g_sGameButton[3].bIsPush = FALSE;			
			g_sGameButton[4].bIsPush = FALSE;

			ShowCursor(TRUE);

			FMOD_System_PlaySound( g_pSystem, FMOD_CHANNEL_FREE, g_pLoopSound[2], 0, &g_pLoopChannel[2] ); // failed !! BGM 사운드
		}
		break;
	case GAME_SUCCEED:
		// Note: 다음 스테이지로 진행
		if( dwCurTime - g_dwSucceedStartTime >= g_dwSucceedTime )
		{
			g_nStage++;
			g_nGameState = GAME_INIT;
			Init( pd3dDevice );
			g_nGameState = GAME_LOADING;
			Init( pd3dDevice );
		}
		break;
	case GAME_FAILED:		
		ShowCursor(TRUE);
		for( i = 3 ; i < 5 ; i++ )
		{
			nButtonIndex = g_sGameButton[i].nIndex[g_sGameButton[i].bIsPush];			
			rtButton.left = g_sGameButton[i].nX;
			rtButton.top = g_sGameButton[i].nY;
			rtButton.right = rtButton.left + g_sButtonCutInfo.pCutRect[nButtonIndex].right - 
							g_sButtonCutInfo.pCutRect[nButtonIndex].left + 1;
			rtButton.bottom = rtButton.top + g_sButtonCutInfo.pCutRect[nButtonIndex].bottom - 
							g_sButtonCutInfo.pCutRect[nButtonIndex].top + 1;

			if( PtInRect( &rtButton, g_ptMouse ) )
				g_sGameButton[i].bIsPush = TRUE;
			else
				g_sGameButton[i].bIsPush = FALSE;
		}
		break;
	case GAME_RESULT:
		for( i = 3 ; i < 5 ; i++ )
		{
			nButtonIndex = g_sGameButton[2].nIndex[g_sGameButton[2].bIsPush];			
			rtButton.left = g_sGameButton[2].nX;
			rtButton.top = g_sGameButton[2].nY;
			rtButton.right = rtButton.left + g_sButtonCutInfo.pCutRect[nButtonIndex].right - 
							g_sButtonCutInfo.pCutRect[nButtonIndex].left + 1;
			rtButton.bottom = rtButton.top + g_sButtonCutInfo.pCutRect[nButtonIndex].bottom - 
							g_sButtonCutInfo.pCutRect[nButtonIndex].top + 1;

			if( PtInRect( &rtButton, g_ptMouse ) )
				g_sGameButton[2].bIsPush = TRUE;
			else
				g_sGameButton[2].bIsPush = FALSE;
		}
		break;
	case GAME_EXIT:		
		SendMessage( DXUTGetHWND(), WM_DESTROY, 0, 0 );
		break;
	}

	FMOD_System_Update(g_pSystem);
}

void Render()
{
	int nWidth, nHeight, nDestX, nDestY, nBackGroundIndex, i;
	int nTypeIndex, nButtonIndex, nX;
	POINT ptCenterXY;
	RECT rtCutRect;	
	int nMinute, nSecond, nLength;
	char string[100];
	D3DXVECTOR3 vPos;
	
	switch( g_nGameState )
	{
	case GAME_INIT:
		if( g_nStage == -1 )
		{
			// 초기 배경 화면 출력		
			g_pSpriteObject->Draw( g_pMainTexture, 0, 0, &D3DXVECTOR3(0, 0, 0), 
									D3DCOLOR_XRGB(255, 255, 255));	
			g_pSpriteObject->Draw( g_pMainBallTexture, 
								&g_sMainBallCutInfo.pCutRect[g_nMainBallAniIndex], 0, 
								&D3DXVECTOR3(350, 300, 0 ), D3DCOLOR_XRGB(255, 255, 255) );			
			
			// 버튼 출력 
			for( i = 0 ; i < 2 ; i++ )
			{
				nButtonIndex =  g_sGameButton[i].nIndex[ g_sGameButton[i].bIsPush ];
				g_pSpriteObject->Draw( g_pButtonTexture, &g_sButtonCutInfo.pCutRect[nButtonIndex], 
								0, &D3DXVECTOR3(g_sGameButton[i].nX, g_sGameButton[i].nY, 0), 
								D3DCOLOR_XRGB(255, 255, 255));				
			}	
		}		
		break;
	case GAME_LOADING:
		// 로딩 문자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[0];
		g_pSpriteObject->Draw( g_pTextTexture, &g_sTextCutInfo.pCutRect[0], 0, 
										&D3DXVECTOR3(550, 450, 0), 
										D3DCOLOR_XRGB(255, 255, 255));		

		// 로딩 바 출력
		rtCutRect = g_sTextCutInfo.pCutRect[1];	
		nWidth = rtCutRect.right - rtCutRect.left + 1;
		for( i = 0 ; i < g_nLoadingBarCount ; i++ )
		{
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(70 + (nWidth-1)*i, 510, 0), 
									D3DCOLOR_XRGB(255, 255, 255));			
		}
		break;

	case GAME_READY:
		// Stage 문자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[2];
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
								&D3DXVECTOR3(280, 230, 0), D3DCOLOR_XRGB(255, 255, 255));	

		// Stage 숫자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[ g_nStage + 4 ];
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
								&D3DXVECTOR3(490, 230, 0), D3DCOLOR_XRGB(255, 255, 255));			
		break;

	case GAME_RUN:
		
		//------------------------------------ 배경출력, 스테이지 인덱스에 따라 출력한다. ---------------------------------------- 		
		rtCutRect = g_sBackGroundCutInfo.pCutRect[g_nBGIndex];
		g_pSpriteObject->Draw(g_pBackGroundTexture, &rtCutRect, 0, 
							  &D3DXVECTOR3(g_nAdjustX, g_nAdjustY, 0), D3DCOLOR_XRGB(255, 255, 255));	

	//-------------------------------------- 골대 출력-------------------------------------
		rtCutRect = g_pGoalPostSprite->pCutRect[0];
		g_pSpriteObject->Draw(g_pGoalPostTexture, &rtCutRect, 0, 
							 &D3DXVECTOR3(g_ptGoalPost.x, g_ptGoalPost.y, 0), 
							D3DCOLOR_XRGB(255, 255, 255));		

	//------------------------------------ 적캐릭터 출력 -------------------------------------------	
		for(i = 0 ; i < g_nEnemyCount ; i++)
		{			
			nTypeIndex = g_pEnemyProperty[i].nTypeIndex;
			rtCutRect = g_pSprite[nTypeIndex]->pCutRect[g_pEnemyProperty[i].nAniSprIndex];
			// 중심좌표 적용
			ptCenterXY.x = g_pSprite[nTypeIndex]->pPosInfo[g_pEnemyProperty[i].nAniSprIndex].nCenterX;
			ptCenterXY.y = g_pSprite[nTypeIndex]->pPosInfo[g_pEnemyProperty[i].nAniSprIndex].nCenterY;
			g_pSpriteObject->Draw(g_pEnemyTexture[nTypeIndex], &rtCutRect,  &D3DXVECTOR3(ptCenterXY.x, ptCenterXY.y, 0 ),
							 &D3DXVECTOR3(g_pEnemyProperty[i].nX, g_pEnemyProperty[i].nY, 0), 
							 D3DCOLOR_XRGB(255, 255, 255));	
		}	

		////------------------------------------- 공 출력------------------------------------------
		rtCutRect = g_pBallSprite->pCutRect[g_sBall.nAniSprIndex];
		ptCenterXY.x = g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].nCenterX;
		ptCenterXY.y = g_pBallSprite->pPosInfo[g_sBall.nAniSprIndex].nCenterY;
		g_pSpriteObject->Draw(g_pBallTexture, &rtCutRect,  &D3DXVECTOR3(ptCenterXY.x, ptCenterXY.y, 0 ),
							 &D3DXVECTOR3(g_sBall.nX, g_sBall.nY, 0), 
							 D3DCOLOR_XRGB(255, 255, 255));		
				
		////-------------------------------------- 주인공 출력 -------------------------------------------
		rtCutRect = g_pPlayerSprite->pCutRect[g_sPlayer.nAniSprIndex];
		ptCenterXY.x = g_pPlayerSprite->pPosInfo[g_sPlayer.nAniSprIndex].nCenterX;
		ptCenterXY.y = g_pPlayerSprite->pPosInfo[g_sPlayer.nAniSprIndex].nCenterY;		
		g_pSpriteObject->Draw(g_pPlayerTexture, &rtCutRect,  
							 &D3DXVECTOR3(ptCenterXY.x, ptCenterXY.y, 0 ),
							 &D3DXVECTOR3(g_sPlayer.nX, g_sPlayer.nY, 0), 
							 D3DCOLOR_XRGB(255, 255, 255));			

		////------------------- 게임 정보 출력 => 총점, 스테이지, 점수, 목표 수등.....
		rtCutRect = g_sTextCutInfo.pCutRect[13]; // <- 총점 문자
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
								&D3DXVECTOR3(250, 30, 0), D3DCOLOR_XRGB(255, 255, 255));	

		// Note: 총점 출력
		sprintf( string, "%d", g_nTotalGrade );
		nLength = strlen( string ); // 자릿수를 구함
		for( i = 0 ; i < nLength ; i++ )
		{
			rtCutRect = g_sTextCutInfo.pCutRect[ string[i] - '0' + 16 ]; 			
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(310 + i*12, 31, 0), D3DCOLOR_XRGB(255, 255, 255));			
		}		

		// Note: 스테이지 출력	
		rtCutRect = g_sTextCutInfo.pCutRect[14]; // <- 스테이지 문자		
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(460, 30, 0), D3DCOLOR_XRGB(255, 255, 255));		

		sprintf( string, "%d", g_nStage + 1 );
		nLength = strlen( string ); // 자릿수를 구함
		for( i = 0 ; i < nLength ; i++ )
		{
			rtCutRect = g_sTextCutInfo.pCutRect[ string[i] - '0' + 16 ]; 
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(535 + i*12, 31, 0), D3DCOLOR_XRGB(255, 255, 255));			
		}		

		// Note: 제한시간과 골인 목표수, 득점을 문자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[15]; // <- 사이드 문자 출력		
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(584, 72, 0), D3DCOLOR_XRGB(255, 255, 255));	
		// Note: 스테이지 점수 출력
		sprintf( string, "%d", g_nGrade );
		nLength = strlen( string ); // 자릿수를 구함
		for( i = 0 ; i < nLength ; i++ )
		{
			rtCutRect = g_sTextCutInfo.pCutRect[ string[i] - '0' + 16 ]; 
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(680 + i*12, 74, 0), D3DCOLOR_XRGB(255, 255, 255));				
		}

		// Note: 제한 시간 숫자 출력
		nMinute =  ( g_dwLimitedTime / 1000 ) / 60; // 초로 환산해서 분과 초로 나눔 		
		nSecond = ( g_dwLimitedTime / 1000 ) % 60;

		rtCutRect = g_sTextCutInfo.pCutRect[ nMinute + 16 ]; // 17번째가 1 문자이며 분은 두 자리수는 게임에서 없다는 전제로 한다.
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(680, 111, 0), D3DCOLOR_XRGB(255, 255, 255));			

		rtCutRect = g_sTextCutInfo.pCutRect[ 26 ]; // 분과 초를 구분하기 위한 구분자 : 이다.
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(695, 113, 0), D3DCOLOR_XRGB(255, 255, 255));		

		sprintf( string, "%02d", nSecond ); // 초는 두자리를 기본으로 하고 문자로 나누고 인덱스로 변환하여 출력한다.
		rtCutRect = g_sTextCutInfo.pCutRect[ string[0] - '0' + 16 ]; // 17번째가 1 문자이며 분은 두 자리수는 게임에서 없다는 전제로 한다.
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(705, 111, 0), D3DCOLOR_XRGB(255, 255, 255));		

		rtCutRect = g_sTextCutInfo.pCutRect[ string[1] - '0' + 16 ]; // 17번째가 1 문자이며 분은 두 자리수는 게임에서 없다는 전제로 한다.
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(715, 111, 0), D3DCOLOR_XRGB(255, 255, 255));		

		// Note: 골인 목표 개수 출력 
		sprintf(string, "%d", g_nGoalCount);
		if( g_nGoalCount < 10 ) // 목표 골인 개수가 한 자릿수 일 때 
		{
			rtCutRect = g_sTextCutInfo.pCutRect[ string[0] - '0' + 16 ]; 
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(682, 152, 0), D3DCOLOR_XRGB(255, 255, 255));			
		}else{
			sprintf( string, "%d", g_nGoalCount ); // 골인 개수가 두 자릿수 일 때 
			rtCutRect = g_sTextCutInfo.pCutRect[ string[0] - '0' + 16 ]; // 17번째가 1 문자이며 분은 두 자리수는 게임에서 없다는 전제로 한다.		
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(682, 152, 0), D3DCOLOR_XRGB(255, 255, 255));		

			rtCutRect = g_sTextCutInfo.pCutRect[ string[1] - '0' + 16 ]; // 17번째가 1 문자이며 분은 두 자리수는 게임에서 없다는 전제로 한다.
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(694, 152, 0), D3DCOLOR_XRGB(255, 255, 255));				
		}	

		// Note: 골인 이펙트 출력
		if( g_EffectText.bLife )
		{
			rtCutRect = g_sTextCutInfo.pCutRect[40]; // 골인 문자			
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(600, 380, 0), D3DCOLOR_XRGB(255, 255, 255));				
		}

		break;
	case GAME_SUCCEED:
		// Note: 배경 출력
		nBackGroundIndex = g_nBackGroundIndex[g_nStage % 4]; // 배경 4 개 이므로 
		rtCutRect = g_sBackGroundCutInfo.pCutRect[nBackGroundIndex];	
		nWidth = g_sBackGroundCutInfo.pCutRect[nBackGroundIndex].right 
					- g_sBackGroundCutInfo.pCutRect[nBackGroundIndex].left + 1;
		nHeight = g_sBackGroundCutInfo.pCutRect[nBackGroundIndex].bottom 
					- g_sBackGroundCutInfo.pCutRect[nBackGroundIndex].top + 1;
		nDestX = (GAME_WIDTH - nWidth)/2 - 1;
		nDestY = (GAME_HEIGHT - nHeight)/2 - 1;
		g_pSpriteObject->Draw(g_pBackGroundTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(nDestX, nDestY, 0), D3DCOLOR_XRGB(255, 255, 255));	

		// Note: Succeed 문자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[41]; // 
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(180, 200, 0), D3DCOLOR_XRGB(255, 255, 255));		
		break;
	case GAME_FAILED:		
		// Note: 다시 도전 ? 문자 출력
		rtCutRect = g_sTextCutInfo.pCutRect[28]; // 		
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(200, 200, 0), D3DCOLOR_XRGB(255, 255, 255));	
		// 버튼 출력 
		for( i = 3 ; i < 5 ; i++ )
		{
			nButtonIndex =  g_sGameButton[i].nIndex[ g_sGameButton[i].bIsPush ];
			rtCutRect = g_sButtonCutInfo.pCutRect[nButtonIndex];
			g_pSpriteObject->Draw(g_pButtonTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(g_sGameButton[i].nX, g_sGameButton[i].nY, 0), D3DCOLOR_XRGB(255, 255, 255));
		}	

		break;
	case GAME_RESULT:
		// Note: 총점 출력
		rtCutRect = g_sTextCutInfo.pCutRect[29]; // 
		g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(246, 200, 0), D3DCOLOR_XRGB(255, 255, 255));			
		// Note: 최종 점수		
		sprintf( string, "%d", g_nTotalGrade);
		nLength = strlen( string ); // 자릿수를 구함
		nX = (GAME_WIDTH - nLength*45)/2;  // 숫자 자리수에 따라 처음 출력될 좌표 계산 GAME_WIDTH/2 - (nLength*45)/2 를 정리한 것임
		for( i = 0 ; i < nLength ; i++ )
		{
			rtCutRect = g_sTextCutInfo.pCutRect[ string[i] - '0' + 30 ]; 
			g_pSpriteObject->Draw(g_pTextTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(nX + i*45, 300, 0), D3DCOLOR_XRGB(255, 255, 255));			
		} 

		// 버튼 출력 		
		nButtonIndex =  g_sGameButton[2].nIndex[ g_sGameButton[2].bIsPush ];
		rtCutRect = g_sButtonCutInfo.pCutRect[nButtonIndex];		
		g_pSpriteObject->Draw(g_pButtonTexture, &rtCutRect, 0, 
									&D3DXVECTOR3(g_sGameButton[2].nX, g_sGameButton[2].nY, 0), D3DCOLOR_XRGB(255, 255, 255));		
		break;	
	}	
}

void Release()
{
	int i;
	ReleaseGenSprite(g_pBallSprite);
	ReleaseSprite(g_pPlayerSprite);	

	for( i = 0 ; i < 4 ; i++ )
		g_pEnemyTexture[i]->Release();		

	g_pBackGroundTexture->Release();
	g_pPlayerTexture->Release();
	g_pBallTexture->Release();
	g_pGoalPostTexture->Release();
	g_pTextTexture->Release();
	g_pMainTexture->Release();
	g_pButtonTexture->Release();
	g_pMainBallTexture->Release();

	g_pSpriteObject->Release();

	SoundRelease();
}


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, 
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	D3DXCreateSprite( pd3dDevice, &g_pSpriteObject );
	Init(pd3dDevice);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	g_pSpriteObject->OnResetDevice();
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{	
	Update( DXUTGetD3D9Device() );
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, 
									float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						  D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		g_pSpriteObject->Begin( D3DXSPRITE_ALPHABLEND );
		Render();
		g_pSpriteObject->End();
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	switch (uMsg)
	{	
	case WM_MOUSEMOVE: 
		g_ptMouse.x = GET_X_LPARAM(lParam); // LOWORD()와 같다.
		g_ptMouse.y = GET_Y_LPARAM(lParam); // HIWORD()와 같다. 
		break;

	case WM_LBUTTONDOWN:
		if( g_sGameButton[0].bIsPush )
		{
			g_nStage++;			
			Init( DXUTGetD3D9Device() );
			g_nGameState = GAME_LOADING;
			Init( DXUTGetD3D9Device() );
			g_sGameButton[0].bIsPush = FALSE;
			g_sGameButton[1].bIsPush = FALSE;
		}else if( g_sGameButton[1].bIsPush ) { // 메인 화면에서 종료
		    SendMessage(hWnd, WM_DESTROY, 0, 0 );		
		} else if( g_sGameButton[3].bIsPush ) { // 재 도전 Yes 선택			
			FMOD_Channel_Stop( g_pLoopChannel[2] ); // failed! GBM 사운드 정지
			g_nGameState = GAME_READY;
			ResetEnemy();
			ShowCursor(FALSE);
			Init( DXUTGetD3D9Device() );			
			g_dwOldReadyTime = GetTickCount();
			g_sGameButton[3].bIsPush = FALSE;
			g_sGameButton[4].bIsPush = FALSE;
		} else if( g_sGameButton[4].bIsPush ) { // 재 도전 No 선택 -> 총점 띄우기 게임 종료
			FMOD_Channel_Stop( g_pLoopChannel[2] ); // failed! GBM 사운드 정지
			
			g_nGameState = GAME_RESULT;
			g_sGameButton[4].bIsPush = FALSE;	
			
		} else if( g_sGameButton[2].bIsPush ) { // 게임 완전 종료
			g_nGameState = GAME_EXIT;			
		}
		break;
	}

    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	g_pSpriteObject->OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	Release();
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"FootBallProject" );
    DXUTCreateDevice( true, 800, 600 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}


