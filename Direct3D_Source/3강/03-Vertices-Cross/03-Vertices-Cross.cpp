// 03-Vertices-Cross.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXVECTOR3 v1( 3.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v2( 0.0f, 0.0f, 3.0f );
	D3DXVECTOR3 vResult;

	D3DXVec3Cross( &vResult, &v1, &v2 );
	D3DXVec3Normalize( &vResult, &vResult );
	printf( "%f %f %f \n", vResult.x, vResult.y, vResult.z );
	return 0;
}

