// FmodSound00.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
#include <string>
#include "FmodSound.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string strBGFileName[] = { "war.mp3", "wave.mp3" };
	string strEffectFileName = "play_fire.wav";
	int nKey;

	CFmodSound FmodSound;	
	FmodSound.CreateBGSound( 2, strBGFileName );
	FmodSound.CreateEffectSound( 1, &strEffectFileName );

	printf( "사운드 시스템 준비 완료\n" );
	printf( "키 1: 배경1 출력    키 2: 효과음 출력  키 3: 배경음 중지   키 q: 종료\n" );

	while( 1 )
	{
		nKey = _getch();
		if( nKey == '1' )
			FmodSound.PlaySoundBG( 1 );
		if( nKey == '2' )
			FmodSound.PlaySoundEffect(0);

		if( nKey == '3' )
			FmodSound.StopSoundBG( 1 );

		if( nKey == 'q' )
			break;

		FmodSound.Update();
	}

	FmodSound.ReleaseSound();

	return 0;
}

