// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matMatrix, matResult;	
	D3DXMatrixRotationX( &matMatrix, 0.3f );

	printf( "------------ 회전 행렬 --------------\n");
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matMatrix(i, j)  );
		}

		printf( "\n" );
	}

	printf( "------------Inverse------------------\n");

	D3DXMatrixInverse( &matResult, NULL, &matMatrix );	
	
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matResult(i, j)  );
		}

		printf( "\n" );
	}

	printf( "--------------Inverse-----------------\n");

	D3DXMatrixInverse( &matResult, NULL, &matResult );	
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matResult(i, j) );
		}

		printf( "\n" );
	}	

	return 0;
}

