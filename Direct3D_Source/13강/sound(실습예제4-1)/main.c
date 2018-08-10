
#include <stdio.h>
#include <conio.h>
#include <fmod.h>

FMOD_SYSTEM *g_System;  // Note: FMOD system 포인터 변수 선언
FMOD_SOUND  *g_Sound;   // Note: FMOD Sound 포인터 변수 선언

void Init()
{
	 // Note: FMOD 사운드 초기화 및 사운드 로딩
     FMOD_System_Create(&g_System); 
     FMOD_System_Init( g_System, 32, FMOD_INIT_NORMAL,  NULL);  	  
     FMOD_System_CreateSound( g_System, "run.wav", FMOD_DEFAULT, 0, &g_Sound);
}

void Release()
{
	 // Note: FMOD 해제
     FMOD_Sound_Release(g_Sound); 
     FMOD_System_Close( g_System ); 
     FMOD_System_Release( g_System ); 
}

int main(void)
{    	 
    int nKey;
    FMOD_CHANNEL *channel = NULL;
    float volume = 0.5f;
	
    Init();

    while( 1 )   
    {
       if( _kbhit() )  // 키 입력 감지   
       {
          nKey = _getch();   // 입력된 키 값을 키보드 버퍼로부터 가져옴
          if( nKey == 'q' )  // q 키가 눌리면 프로그램 종료, 키 입력중 가장 최우선 순위가 된다. 
             break;

          switch( nKey )
          {
           case 'p' : 
					FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound, 0, &channel);
					break;
		   case 's' :
					FMOD_Channel_Stop( channel );				
					break;
		   case 'u' : // 볼륨 업
					volume += 0.1f;
					if( volume > 1.0f )
						volume = 1.0f;
					FMOD_Channel_SetVolume( channel, volume );
					break;
		   case 'd' : // 볼륨 다운
					volume -= 0.1f;
					if( volume < 0.0f )
						volume = 0.0f;
					FMOD_Channel_SetVolume( channel, volume );
					break;
           }
        }

		FMOD_System_Update( g_System );		 	   // FMOD 업데이트	 
    }

    Release();
    return 0;
}

