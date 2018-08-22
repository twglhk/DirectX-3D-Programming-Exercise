// 03-Vector(1).cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXVECTOR3 v1( 0.f, 3.0f, 0.0f );
	D3DXVECTOR3 v2( 3.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v3;
	D3DXVECTOR3 v4( 3.0f, 3.0f, 0.0f );
	float fLength;

	// 벡터의 합
	/*v3 = v1 + v2;
	printf( "합 : %f %f %f \n",  v3.x, v3.y, v3.z );

	D3DXVec3Add( &v3, &v1, &v2 );
	printf( "합 : %f %f %f \n",  v3.x, v3.y, v3.z );*/

	// 벡터의 뺄셈
	/*v3 = v1 - v2;

	printf( "뺄셈 : %f %f %f \n",  v3.x, v3.y, v3.z );

	D3DXVec3Subtract( &v3, &v1, &v2 );
	printf( "뺄셈 : %f %f %f \n",  v3.x, v3.y, v3.z );*/

	// 크기 
	/*fLength = D3DXVec3Length( &v4 );
	printf( "크기 : %f \n", fLength );*/

	// 벡터의 크기 변환 
	/*float fScale = 2.0f;
	D3DXVECTOR3 v5( 2.0f, 1.0f, 0.0f );

	D3DXVec3Scale( &v5, &v5, fScale );
	printf( "값과 벡터의 곱 : %f %f %f \n", v5.x, v5.y, v5.z );*/

	//// 벡터의 단위벡터
	D3DXVECTOR3 v6( 2.0f, 2.0f, 3.0f );
	D3DXVECTOR3 vResult;
	float fNormalize;
	
	D3DXVec3Normalize( &vResult, &v6 );
	printf( "단위벡터 : %f %f %f \n", vResult.x, vResult.y, vResult.z );
	fNormalize = D3DXVec3Length( &vResult );
	printf( "크기 : %f \n", fNormalize );

	return 0;
}

