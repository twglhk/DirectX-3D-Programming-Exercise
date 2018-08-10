// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matMatrix;
	D3DXVECTOR3 v1( 2.0f, 2.0f, 2.0f );
	D3DXVECTOR4 v2;

	D3DXMatrixIdentity( &matMatrix );
	matMatrix = -1 * matMatrix;	

	for( int i = 0 ; i < 4 ; i ++ )
	{
		for( int j = 0 ; j < 4 ; j++)
		{
			printf( "%7.1f ", matMatrix(i, j) ); 
		}
		printf( "\n" );
	}
	
	
	D3DXVec3Transform( &v2, &v1, &matMatrix );	
	printf( "-----------------------------------------\n");
	printf( "변환 결과 : %5.1f %5.1f %5.1f %5.1f \n", v2.x, v2.y, v2.z, v2.w );
	return 0;
}

