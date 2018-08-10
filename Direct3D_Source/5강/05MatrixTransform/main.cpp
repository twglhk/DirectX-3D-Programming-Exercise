// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <d3dx9math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matMatrix;	
	
	//printf( "----------이동 행렬-------------------\n");
	//D3DXMatrixTranslation( &matMatrix, 0.5f, 0.5f, 0.5f );		
	//for( int i = 0 ; i < 4 ; i++ )
	//{
	//	for( int j = 0 ; j < 4 ; j++ )
	//	{
	//		printf( "%7.1f" , matMatrix(i, j) );
	//	}

	//	printf( "\n" );
	//}

	//printf( "-------------크기 행렬---------------------\n");
	//D3DXMatrixScaling( &matMatrix, 0.5f, 0.5f, 0.5f );
	//for( int i = 0 ; i < 4 ; i++ )
	//{
	//	for( int j = 0 ; j < 4 ; j++ )
	//	{
	//		printf( "%7.1f" , matMatrix(i, j) );
	//	}

	//	printf( "\n" );
	//}
	
	/*printf( "------------- X 축 회전 행렬---------------\n");
	D3DXMatrixRotationX( &matMatrix, D3DXToRadian( 30 ) );
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matMatrix(i, j) );
		}

		printf( "\n" );
	}*/

	printf( "------------- Y 축 회전 행렬---------------\n");
	D3DXMatrixRotationY( &matMatrix, D3DXToRadian( 30 ) );
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matMatrix(i, j) );
		}

		printf( "\n" );
	}
/*
	printf( "------------- Z 축 회전 행렬---------------\n");
	D3DXMatrixRotationZ( &matMatrix, D3DXToRadian( 30 ) );
	for( int i = 0 ; i < 4 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			printf( "%7.1f" , matMatrix(i, j) );
		}

		printf( "\n" );
	}*/

	return 0;
}

