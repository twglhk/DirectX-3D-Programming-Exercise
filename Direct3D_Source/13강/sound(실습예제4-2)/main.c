#include <stdio.h>
#include <conio.h>
#include <fmod.h>

FMOD_SYSTEM *g_System;     // FMOD system 변수선언
FMOD_SOUND  *g_Sound[2];   // 두개의 사운드 파일과 연결하기 위한 FMOD Sound
FMOD_CHANNEL *g_Channel;  

// Note: 초기화 
void Init()
{
	 // FMOD 초기화 및 사운드 파일 로딩
	 FMOD_System_Create(&g_System); 
     FMOD_System_Init( g_System, 32, FMOD_INIT_NORMAL,  NULL);  	  
     FMOD_System_CreateSound( g_System, "bgsound.wav", FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	 FMOD_System_CreateSound( g_System, "safe.wav", FMOD_DEFAULT, 0, &g_Sound[1]);

	 // 배경음 출력
	 FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[0], 0, &g_Channel);
}

// Note: 해제
void Release()
{
	int i;

	for( i =0 ; i < 2 ; i++ )
		FMOD_Sound_Release(g_Sound[i]); // FMOD sound 객체 해제
	
	FMOD_System_Close( g_System ); // FMOD system 객체 clsoe
	FMOD_System_Release( g_System ); // FMOD system 객체 해제
}


int main(void)
{
	int nKey;
	FMOD_CHANNEL *pChannel; 
	
	Init();

	printf( "사운드 출력\n" );

	while( 1 )
	{
		if( _kbhit() )
		{
			nKey = _getch();

			if( nKey == 's' )
				FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[1], 0, &pChannel); // 효과음 출력
		}				

		FMOD_System_Update( g_System );
	}

	Release();	
	return 0;
}