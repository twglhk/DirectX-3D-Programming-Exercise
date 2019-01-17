#include "StdAfx.h"
#include "CGround.h"
#include <stdio.h>
#include <conio.h>

struct GROUNDVERTEX
{
	D3DXVECTOR3 vPos;
	DWORD		dwDiffuse;
};

#define D3DFVF_GROUNDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

CGround::CGround(void)
{
}


CGround::~CGround(void)
{
}

void CGround::Create(LPDIRECT3DDEVICE9 pd3dDevice, int nRow, int nCol, float fSize)
{
	m_nCol = nCol;
	m_nRow = nRow;

	m_dwNumVertices = (m_nCol + 1)*(m_nRow + 1); // 격자 +1의 행렬곱이 정점의 개수
	m_dwNumIndices = m_nCol * m_nRow * 6;
	m_pd3dDevice = pd3dDevice;

	GROUNDVERTEX *pGroundVertex = new GROUNDVERTEX[m_dwNumVertices];
	// Vertex buffer와 관련된 배열을 선언한다. 한번에 배열을 Vertex buffer에 복사할 수 있다.
	// 혹은 Buffer에 direct로 매 vertex를 집어넣는 방법이 있는데 이번엔 이렇게 사용한다.


	// Note: 시작 위치
	int nIndex = 0;
	D3DXVECTOR3 vPos0(-1.0f * m_nCol * fSize * 0.5f, 0.0f, m_nRow * fSize * 0.5f);
	for (int z = 0; z <= m_nRow; z++)
	{
		for (int x = 0; x <= m_nCol; x++)
		{
			pGroundVertex[nIndex].vPos.x = vPos0.x + (fSize * x);
			pGroundVertex[nIndex].vPos.y = 0.0f;
			pGroundVertex[nIndex].vPos.z = vPos0.z + -1.0f*(fSize * z);
			pGroundVertex[nIndex].dwDiffuse = D3DCOLOR_RGBA(255, 50, 255, 255);
			nIndex++;
		}
	}

	//Note: 버텍스 버퍼 생성 
	if (m_pd3dDevice->CreateVertexBuffer(m_dwNumVertices * sizeof(GROUNDVERTEX), 0, D3DFVF_GROUNDVERTEX, D3DPOOL_DEFAULT, &m_pVB, 0) != D3D_OK)
	{
		MessageBox(NULL, (LPCWSTR)"정정 버퍼 생성 Error", (LPCWSTR)"Error", MB_OK);
		return;
	}

	void *pVertices;
	if (m_pVB->Lock(0, 0, &pVertices, NULL) != D3D_OK) // 2번째 매개변수는 전체를 lock하는 값 0	
	{
		MessageBox(NULL, (LPCWSTR)"정정 버퍼 lock Error", (LPCWSTR)"Error", MB_OK);
		return;
	}

	memcpy(pVertices, pGroundVertex, m_dwNumVertices * sizeof(GROUNDVERTEX));
	m_pVB->Unlock();

	//Note: 인덱스 버퍼 생성
	WORD *pIndex = new WORD[m_dwNumIndices];
	nIndex = 0;
	for (int z = 0; z < m_nRow; z++)
	{
		for (int x = 0; x < m_nCol; x++)
		{
			pIndex[nIndex++] = WORD(z * (m_nCol + 1) + x);
			pIndex[nIndex++] = WORD((z + 1)*(m_nCol + 1) + x + 1);
			pIndex[nIndex++] = WORD((z + 1)*(m_nCol + 1) + x);

			pIndex[nIndex++] = WORD(z * (m_nCol + 1) + x);
			pIndex[nIndex++] = WORD(z * (m_nCol + 1) + x + 1);
			pIndex[nIndex++] = WORD((z + 1)*(m_nCol + 1) + x + 1);
		}
	}

	void *pIndices;
	m_pd3dDevice->CreateIndexBuffer(m_dwNumIndices * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	memcpy(pIndices, pIndex, sizeof(WORD)*m_dwNumIndices);
	m_pIB->Unlock();

	delete[] pIndex;
	delete[] pGroundVertex;
}

void CGround::OnRender()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(GROUNDVERTEX));
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->SetFVF(D3DFVF_GROUNDVERTEX);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwNumVertices, 0, m_dwNumIndices / 3);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CGround::OnRelease()
{
	m_pIB->Release();
	m_pVB->Release();
}