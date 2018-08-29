// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <d3dx9math.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matMatrix, matResult;
	D3DXMatrixRotationX(&matMatrix, 0.3f);

	// Rotated Matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << matMatrix.m[i][j] << " ";
		
		cout << endl;
	}

	cout << "=======================" << endl;

	// Inverse
	D3DXMatrixInverse(&matResult, NULL, &matMatrix);

	// Inversed matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << matResult.m[i][j] << " ";
		
		cout << endl;
	}

	D3DXMatrixInverse(&matResult, NULL, &matResult);
	// Inversed matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << matResult.m[i][j] << " ";

		cout << endl;
	}

	return 0;
}

