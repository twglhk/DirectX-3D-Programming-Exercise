// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matIdentity, matMatrix, matResult;
	D3DXMatrixIdentity( &matIdentity );	
	
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			matMatrix(i, j) = float( i*4 + j + 1);
		}	
	}

	D3DXMatrixMultiply( &matResult, &matMatrix, &matIdentity );
	//matResult = matMatrix * matIdentity;
	
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matResult.m[i][j] );
		}

		printf( "\n" );
	}

	return 0;
}

