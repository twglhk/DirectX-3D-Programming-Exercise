#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "Screen.h"

typedef enum _GAME_STATE { INIT, READY, RUNNING, STOP, SUCCESS, FAILED, RESULT } GAME_STATE;
                                
GAME_STATE g_GameState = INIT;
int g_nGoal, g_nStage, g_GameStartTime;

char g_strMessage[100]; // 게임 상태를 저장하는 문자열
clock_t g_OldTime; // 게임 상태 전이를 위한 이전 시각 저장

void Init()
{       
	g_OldTime = clock();
}

void Update()
{
	clock_t CurTime = clock();	

	switch( g_GameState )
	{
	case INIT :
				if( g_nStage == 0 )
				{
					sprintf( g_strMessage, "%s", "게임 및 사운드 초기화" );
					if( CurTime - g_OldTime > 3000 )
					{
						g_OldTime = CurTime;
						g_nStage = 1;
					}
				}else{
					sprintf( g_strMessage, "[INIT] 게임 %d 스테이지 초기화", g_nStage );
					if( CurTime - g_OldTime > 3000 )
					{
						g_OldTime = CurTime;
						g_GameState = READY;
					}
				}
				break;
	case READY :	
				sprintf( g_strMessage, "[READY] %d 스테이지", g_nStage );
				if( CurTime - g_OldTime > 3000 )
				{
					g_OldTime = CurTime;
					g_GameState = RUNNING;
					g_GameStartTime = CurTime;
				}
				break;
	case RUNNING :				
				if( CurTime - g_GameStartTime > 10000 ) // Note: 제한 시간
				{
					g_GameState = STOP;					
				}else{			
					sprintf( g_strMessage, "[RUNNING] 제한 시간 : 10초  현재시간 : %d", 
														 ( CurTime - g_GameStartTime ) / 1000 );				
				}		
				break;
	case STOP :				
				if( g_nGoal == 1 )
					g_GameState = SUCCESS;
				else
				    g_GameState = FAILED;	
				break;
	case SUCCESS :
				sprintf( g_strMessage, "%s", "미션 성공" );
				if( CurTime - g_OldTime > 3000 )
				{
					g_OldTime = CurTime;
					g_GameState = INIT;
					++g_nStage;					
				}
				break;
	case FAILED :
				sprintf( g_strMessage, "%s", "미션 실패! 계속 하시겠습니까? <y/n> " );
				break;

	case RESULT: 
				sprintf( g_strMessage, "%s", "게임 결과 화면" );
				if( CurTime - g_OldTime > 3000 )
				{
					g_OldTime = CurTime;								
				}				
				break;
	}
}

void Render()
{	
	 clock_t CurTime = clock();
	 
     ScreenClear();

     // Note: 렌더링 시작	
	 ScreenPrint( 20, 10, g_strMessage );
			 
	 // Note: 렌더링 끝 
	 ScreenFlipping();
}

void Release()
{
}

int main(void)
{
	int nKey;

    ScreenInit();
    Init();        // 초기화
         
    while( 1 )
    { 
		if( g_GameState == RESULT )
			return ;

		if( _kbhit() )
		{
			nKey = _getch();

			if( nKey == 'q' )
			   break;

			switch( nKey )
            {
             case 's' : 
                        g_nGoal = 1;
                        break;
             case 'f' :
   	                    g_nGoal = 0;
                        break;
			 case 'y' :
			 case 'Y' :
						if( g_GameState == FAILED ) 
						{
							g_GameState = INIT;
							g_OldTime = clock();
						}
						break;
			 case 'n' :
			 case 'N' :
						if( g_GameState == FAILED )
						{
							g_GameState = RESULT;
							g_OldTime = clock();
						}
						break;
            } 
		}

        Update();    // 데이터 갱신
        Render();    // 화면 출력		
    }
     
    Release();   // 해제
    ScreenRelease();
    return 0;
}
