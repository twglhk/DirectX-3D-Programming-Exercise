// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
		// 내적을 이용하여 사이각을 알아내는 부분
	D3DXVECTOR3 v7( 3.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v8( -3.0f, 0.0f, 0.0f);
	float fCos, fDot, fScale;

	fDot = D3DXVec3Dot( &v7, &v8 );
	fScale = D3DXVec3Length( &v7 ) * D3DXVec3Length( &v8 );
	fCos = fDot / fScale;
	printf( "라디안 : %f \n", fCos ); 
	return 0;
}

