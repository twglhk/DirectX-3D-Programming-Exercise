// 03-Vertices-Cos.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <d3dx9math.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	D3DXMATRIX matTemp, matResult;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matTemp(i, j) = float(4*i + j);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << matTemp.m[i][j] << " ";
		}
		cout << endl;
	}

	cout << "------------------------------" << endl;

	D3DXMatrixTranspose(&matResult, &matTemp);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << matResult.m[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}

